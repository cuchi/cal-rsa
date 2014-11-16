#include <stdio.h>
#include <unistd.h>
#include "rsa.h"

int main(int argc, char* argv[]) {
    int c;
    unsigned int b;
    keypair_t kp;
    c = getopt(argc, argv, "b:");
    if (c == 'b') {
        b = atoi(optarg);
        if (b < 32) {
            printf("Too short!\n");
            return 1;
        } else {
            random_init();
            kp = keypair_init_r(b);
            keypair_raw_info(kp);
            keypair_free(kp);
            return 0;
        }
    } else {
        printf("\n### Usage:\n");
        printf("%s -b size_in_bits\n\n", argv[0]);
        return 1;
    }
    return 0;
}
