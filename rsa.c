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
            mpz_urandomb(p, RS, size/2);
        } while (!isPrime(p, 64) || mpz_cmp_si(p, 2) == 0);
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
            mpz_set_si(k->e, 65536); // ... or a random from 0 to (phi - 2)
            //mpz_urandomm(k->e, RS, phi);
            mpz_add_ui(k->e, k->e, 1);
            // (e, phi) must be coprimes (gcd == 1):
            euclides(gcd, k->e, phi);
        } while (mpz_cmp_si(gcd, 1) != 0);
        modInv(k->d, k->e, phi);
    } while (mpz_cmp(k->e, k->d) == 0);
    mpz_clears(p, q, phi, phi_aux, gcd, p1, p2, min_q, max_q, NULL);
}

keypair_t keypair_init_r(int size) {
    keypair_t k = (keypair_t) malloc(sizeof(struct keypair));
    k->size = size;
    mpz_inits(k->n, k->e, k->d, NULL);
    keypair_gen(k->size, k);
    return k;
}

keypair_t keypair_init_p(const mpz_t n, const mpz_t e, const mpz_t d) {
    keypair_t k = (keypair_t) malloc(sizeof(struct keypair));
    mpz_inits(k->n, k->e, k->d, NULL);
    mpz_set(k->n, n);
    mpz_set(k->e, e);
    mpz_set(k->d, d);
    k->size = mpz_sizeinbase(k->n, 2);
    return k;
}

void keypair_free(keypair_t k) {
    mpz_clears(k->n, k->e, k->d, NULL);
    free(k);
}

void keypair_info(keypair_t k) {
    gmp_printf("Size (in bits): %d\n", k->size);
    gmp_printf("Modulus: %Zd\n", k->n);
    gmp_printf("Public expoent: %Zd\n", k->e);
    gmp_printf("Private expoent: %Zd\n", k->d);
}

void keypair_simulate_break(keypair_t k) {
    struct timeval t0, t1;
    double time;
    mpz_t p, q, mod, phi, d;
    mpz_inits(p, q, mod, phi, d, NULL);
    gettimeofday(&t0, 0);
    mpz_root(p, k->n, 2);
    if (mpz_even_p(p)) {
        mpz_add_ui(p, p, 1);
    }
    while (true) {
        mpz_mod(mod, k->n, p);
        if (mpz_cmp_si(mod, 0) == 0) {
            mpz_divexact(q, k->n, p);
            totient(phi, p, q);
            modInv(d, k->e, phi);
            if (mpz_cmp(k->d, d) == 0) {
                gettimeofday(&t1, 0);
				time = (t1.tv_sec - t0.tv_sec) + 0.000001 * (t1.tv_usec - t0.tv_usec);
				gmp_printf("%3d %lf\n", k->size, time);
                mpz_clears(p, q, mod, phi, d, NULL);
                return;
            }
        }
        mpz_sub_ui(p, p, 2);
    }
}

void keypair_file_encrypt(keypair_t k, FILE* in_file, FILE* out_file) {
    long file_size;
    byte* in_stream;
    byte* out_stream;
    mpz_t in_data, out_data, n_aux, p;
    int data_size;
    int word_size;
    int i, j;

    fseek(in_file, 0, SEEK_END);
    file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    in_stream = (byte*) malloc(file_size);
    fread(in_stream, 1, file_size, in_file);

    mpz_inits(in_data, out_data, NULL);
    mpz_import(in_data, file_size, 1, 1, 0, 0, in_stream);
    free(in_stream);
    data_size = mpz_sizeinbase(in_data, 2);
    word_size = k->size - 1;
    data_size += word_size;
    data_size -= data_size % word_size;

    mpz_inits(n_aux, p, NULL);
    for (i = (data_size / word_size) - 1; i >= 0; i--) {
        mpz_set_si(n_aux, 0);
        for (j = word_size - 1; j >= 0; j--) {
            if (mpz_tstbit(in_data, i * word_size + j)) {
                mpz_setbit(n_aux, j);
            }
        }
        mpz_powm_sec(n_aux, n_aux, k->e, k->n);
        mpz_ui_pow_ui(p, 2, i * k->size);
        mpz_mul(n_aux, n_aux, p);
        mpz_add(out_data, out_data, n_aux);
    }
    mpz_clear(in_data);

    data_size = mpz_sizeinbase(out_data, 2);
    data_size += 8;
    data_size -= data_size % 8;
    out_stream = (byte*) malloc(data_size/8);
    for (i = (data_size / 8) - 1; i >= 0; i--) {
        mpz_set_si(n_aux, 0);
        for (j = 7; j >= 0; j--) {
            if (mpz_tstbit(out_data, i * 8 + j)) {
                mpz_setbit(n_aux, j);
            }
        }
        out_stream[i] = (byte) mpz_get_ui(n_aux);
    }
    mpz_clear(out_data);

    fseek(out_file, 0, SEEK_SET);
    fwrite(out_stream, 1, data_size / 8, out_file);
    free(out_stream);
    mpz_clears(n_aux, p, NULL);
}
