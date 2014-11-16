#ifndef RSA_H
#define RSA_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct keypair* keypair_t;
typedef char byte;

struct keypair {
    mpz_t n;
    mpz_t e;
    mpz_t d;
    int size;
};

static gmp_randstate_t RS;

void random_init();
void euclides(mpz_t, const mpz_t, const mpz_t);
void euclides_ext(const mpz_t, const mpz_t, mpz_t, mpz_t, mpz_t);
int mod_inv(mpz_t, const mpz_t, const mpz_t);
void phi_n(mpz_t, const mpz_t, const mpz_t);
bool is_prime(const mpz_t, int);
void keypair_gen(int size, keypair_t k);
keypair_t keypair_init_r(int size);
keypair_t keypair_init_p(const mpz_t n, const mpz_t e, const mpz_t d);
void keypair_info(keypair_t k);
void keypair_simulate_break(keypair_t k);
void keypair_file_encrypt(keypair_t k, FILE* in_file, FILE* out_file);
void keypair_file_decrypt(keypair_t k, FILE* in_file, FILE* out_file);

#endif
