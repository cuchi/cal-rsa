#include <stdio.h>
#include "rsa.h"

int main() {
    random_init();
    keypair_t k;
    k = keypair_init(8);
    keypair_info(k);
    return 0;
}
