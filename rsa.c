#include "rsa.h"

void random_init() {
    gmp_randinit_default(RS);
    gmp_randseed_ui(RS, time(NULL));
}

/*
 * Algoritmo de Euclides
 * maior divisor comum
 */
void euclides(mpz_t r, const mpz_t _a, const mpz_t _b) {
    mpz_t a, b;
    mpz_inits(a, b, NULL);
    mpz_set(a, _a);
    mpz_set(b, _b);
    if (mpz_cmp(a, b) < 0) {
        mpz_swap(a, b);
    }
    do {
        mpz_mod(a, a, b);
        mpz_swap(a, b);
    } while (mpz_cmp_si(b, 0) != 0);
    mpz_set(r, a);
    mpz_clears(a, b, NULL);
}

void euclidesExt(const mpz_t _a, const mpz_t _b, mpz_t _gcd, mpz_t _x, mpz_t _y) {
    mpz_t a, b, x, y, u, v, q, r, m, n;
    mpz_inits(a, b, x, y, u, v, q, r, m, n, NULL);
    mpz_set_si(y, 1);
    mpz_set_si(u, 1);
    mpz_set(a, _a);
    mpz_set(b, _b);
    while (mpz_cmp_si(a, 0) != 0) {
        mpz_tdiv_q(q, b, a);
        mpz_mod(r, b, a);
        mpz_mul(m, u, q);
        mpz_sub(m, x, m);
        mpz_mul(n, v, q);
        mpz_sub(n, y, n);
        mpz_set(b, a);
        mpz_set(a, r);
        mpz_set(x, u);
        mpz_set(y, v);
        mpz_set(u, m);
        mpz_set(v, n);
    }
    mpz_set(_gcd, b);
    mpz_set(_x, x);
    mpz_set(_y, y);
    mpz_clears(a, b, x, y, u, v, q, r, m, n, NULL);
}

int modInv(mpz_t mod_inv, const mpz_t a, const mpz_t m) {
    int ret;
    mpz_t gcd, x, y;
    mpz_inits(gcd, x, y, NULL);
    euclidesExt(a, m, gcd, x, y);
    if (mpz_cmp_si(gcd, 1) != 0) {
        ret = 0;
    } else {
        mpz_mod(mod_inv, x, m);
        ret = 1;
    }
    mpz_clears(gcd, x, y, NULL);
    return ret;
}

void totient(mpz_t t, const mpz_t _p, const mpz_t _q) {
    mpz_t p, q;
    mpz_inits(p, q, NULL);
    mpz_set(p, _p);
    mpz_set(q, _q);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(t, p, q);
    mpz_clears(p, q, NULL);
}

/*
void keypair(const mpz_t p, const mpz_t q, const mpz_t e, mpz_t n, mpz_t private) {
    mpz_t tot;
    mpz_init(tot);
    mpz_mul(n, p, q);
    totient(p, q, tot);
    modInv(e, tot, private);
    mpz_clear(tot);
}
*/

bool isPrime(const mpz_t n, int it) {
    mpz_t s, s_mod, rnd, temp_n, temp_s, mod;
    // Se n < 2
    int cmp_2 = mpz_cmp_si(n, 2);
    if (cmp_2 < 0) {
        return false;
    } else if (cmp_2 == 0) {
        return true;
    }
    mpz_init(s);
    // Se n != 2
    if (mpz_cmp_si(n, 2) != 0) {
        // s = n % 2
        mpz_mod_ui(s, n, 2);
        // Se s == 0
        if (mpz_cmp_si(s, 0) == 0) {
            mpz_clears(s, NULL);
            return false;
        }
    }
    // s = n - 1
    mpz_sub_ui(s, n, 1);
    mpz_inits(s_mod, rnd, temp_n, temp_s, mod, NULL);
    mpz_mod_ui(s_mod, s, 2);
    // Enquanto s mod 2 == 0
    while (mpz_cmp_si(s_mod, 0) == 0) {
        // s = s / 2
        mpz_tdiv_q_ui(s, s, 2);
        mpz_mod_ui(s_mod, s, 2);
    }
    // Iterações
    while (it--) {
        mpz_set(temp_n, n);
        mpz_sub_ui(temp_n, temp_n, 1);
        mpz_urandomm(rnd, RS, temp_n);
        mpz_add_ui(rnd, rnd, 1);
        mpz_set(temp_s, s);
        mpz_powm_sec(mod, rnd, temp_s, n);
        while (mpz_cmp(temp_s, temp_n) && mpz_cmp_si(mod, 1) && mpz_cmp(mod, temp_n)) {
            mpz_mul(mod, mod, mod);
            mpz_mod(mod, mod, n);
            mpz_mul_si(temp_s, temp_s, 2);
        }
        if (mpz_cmp(mod, temp_n) && mpz_even_p(temp_s)) {
            mpz_clears(s, s_mod, rnd, temp_n, temp_s, mod, NULL);
            return false;
        }
    }
    mpz_clears(s, s_mod, rnd, temp_n, temp_s, mod, NULL);
    return true;
}

//void keypair_gen(const int size, mpz_t n, mpz_t e, mpz_t d) {
void keypair_gen(int size, keypair_t k) {
    mpz_t p, q, phi, phi_aux, gcd, p1, p2, min_q, max_q;
    mpz_inits(p, q, phi, phi_aux, gcd, p1, p2, min_q, max_q, NULL);
    mpz_ui_pow_ui(p1, 2, size - 1);
    mpz_ui_pow_ui(p2, 2, size);
    do {
        do {
            mpz_urandomb(p, RS, size-1);
        } while (!isPrime(p, 64));
        mpz_fdiv_q(min_q, p1, p);
        mpz_fdiv_q(max_q, p2, p);
        mpz_sub(max_q, max_q, min_q);
        do {
            mpz_urandomm(q, RS, max_q);
            mpz_add(q, q, min_q);
            mpz_add_ui(q, q, 1);
        } while (!isPrime(q, 64));
        mpz_mul(k->n, p, q);
        totient(phi, p, q);
        mpz_sub_ui(phi_aux, phi, 1);
        do {
            //mpz_set_si(k->e, 65536); // ... or a random from 0 to (phi - 2)
            mpz_urandomm(k->e, RS, phi);
            mpz_add_ui(k->e, k->e, 1);
            // (e, phi) must be coprimes (gcd == 1):
            euclides(gcd, k->e, phi);
        } while (mpz_cmp_si(gcd, 1) != 0);
        modInv(k->d, k->e, phi);
    } while (mpz_cmp(k->e, k->d) == 0);
    mpz_clears(p, q, phi, phi_aux, gcd, p1, p2, min_q, max_q, NULL);
}

keypair_t keypair_init(int size) {
    keypair_t k = (keypair_t) malloc(sizeof(struct keypair));
    k->size = size;
    mpz_inits(k->n, k->e, k->d, NULL);
    keypair_gen(k->size, k);
    return k;
}

void keypair_info(keypair_t k) {
    gmp_printf("Size (in bits): %d\n", k->size);
    gmp_printf("Modulus: %Zd\n", k->n);
    gmp_printf("Public expoent: %Zd\n", k->e);
    gmp_printf("Private expoent: %Zd\n", k->d);
}
