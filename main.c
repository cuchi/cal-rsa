#include <stdio.h>
#include "rsa.h"

int main() {
    random_init();
    keypair_t k;
    int i, j;
    for (i = 8; i <= 256; i++) {
        for (j = 0; j < 10; j++) {
            k = keypair_init_r(i);
            keypair_simulate_break(k);
            keypair_free(k);
        }
    }
    return 0;
}
