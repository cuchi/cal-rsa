#ifndef RSA_H
#define RSA_H

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct keypair* keypair_t;

struct keypair {
    mpz_t n;
    mpz_t e;
    mpz_t d;
    int size;
};

gmp_randstate_t RS;

void random_init();
void euclides(mpz_t, const mpz_t, const mpz_t);
void euclidesExt(const mpz_t, const mpz_t, mpz_t, mpz_t, mpz_t);
int modInv(mpz_t, const mpz_t, const mpz_t);
void totient(mpz_t, const mpz_t, const mpz_t);
bool isPrime(const mpz_t, int);
void keypair_gen(int size, keypair_t k);
keypair_t keypair_init(int size);
void keypair_info(keypair_t k);

#endif
