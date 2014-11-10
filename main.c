#include <stdio.h>
#include "rsa.h"

int main() {
    random_init();
    keypair_t k;
    k = keypair_init_r(53);
    keypair_simulate_break(k);
    return 0;
}
