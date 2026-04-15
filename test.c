#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>

int main(){

    mpz_t a, b, p, q, n, et_term1, et_term2, et, e, d, c, m, m_deciphered;
    gmp_randstate_t state;

    mpz_inits(a, b, p, q, n, et_term1, et_term2, et, e ,d, c, m, m_deciphered,NULL);

    gmp_randinit_default(state);
    gmp_randseed_ui(state, 12345);

    unsigned long bits = 1024;

    mpz_set_ui(a, 0);
    while(mpz_cmp_ui(a, 0) == 0){
        mpz_urandomb(a, state, bits);
    }
    mpz_nextprime(p, a);
    
    mpz_set_ui(b, 0);
    while(mpz_cmp_ui(b, 0) == 0){
        mpz_urandomb(b, state, bits);
    }
    mpz_nextprime(q, b);

    mpz_mul(n, p, q);

    mpz_sub_ui(et_term1, p, 1);
    mpz_sub_ui(et_term2, q, 1);

    mpz_mul(et, et_term1, et_term2);

    mpz_set_ui(e, 65537);

    int result = mpz_invert(d, e, et);
    if(result != 0){
        printf("Success computing the inverse d, so that e*d = 1 mod phi(n)\n");
    } else {
        printf("e and phi(n) are not coprime\n");
        return 0;
    }

    // --------------------Encrypt the message--------------------
    const char message[68] = "Get get get get\nGot got got got\nBlood rush to my\nHead lit hot lock\n";
    const char message_hex[135];
    for(int i=0;i<66;i++){
        snprintf(message_hex+i*2, 3, "%02x", message[i]);
    }
    mpz_set_str(m, message_hex, 16);
    mpz_powm(c, m, e, n);

    // --------------------Decrypt the message--------------------
    mpz_powm(m_deciphered, c, d, n);
    
    size_t count;
    size_t n_bits = mpz_sizeinbase(m_deciphered, 2);
    size_t n_bytes = (n_bits + 7) / 8;

    unsigned char* buffer = malloc(n_bytes+1);

    mpz_export(buffer, &count, 1, 1, 1, 0, m_deciphered);

    buffer[count] = '\0';
    printf("message deciphered:\n%s", buffer);
    return 0;
}