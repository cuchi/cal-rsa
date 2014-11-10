#include <stdio.h>
#include "rsa.h"

int main() {
    //FILE* in;
    //FILE* out;
    //in = fopen("teste.in", "rb");
    //out = fopen("teste.out", "wb");
    random_init();
    keypair_t k;
    //mpz_t n, e, d;
    //mpz_inits(n, e, d, NULL);
    //gmp_scanf("%Zd %Zd %Zd", n, e, d);
    k = keypair_init_r(53);
    keypair_info(k);
    keypair_simulate_break(k);
    //keypair_file_encrypt(k, in, out);
    return 0;
}
