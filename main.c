#include <unistd.h>
#include <stdio.h>
#include "rsa.h"

int main(int argc, char* argv[]) {
    bool i = false,
         o = false,
         e = false,
         d = false,
         k = false;
    int c;
    char* input_path = NULL;
    char* output_path = NULL;
    char* key_path = NULL;
    FILE* input_file;
    FILE* output_file;
    FILE* key_file;
    mpz_t kn, ke, kd;
    keypair_t kp;
    while ((c = getopt(argc, argv, "i:o:edk:")) != -1) {
        switch (c) {
            case 'i':
                i = true;
                input_path = optarg;
                break;
            case 'o':
                o = true;
                output_path = optarg;
                break;
            case 'e':
                e = true;
                break;
            case 'd':
                d = true;
                break;
            case 'k':
                k = true;
                key_path = optarg;
                break;
            default:
                abort();
        }
    }
    if (!e && !d || e && d) {
        fprintf(stderr, "You must select EITHER e or d.\n");
        return 1;
    }
    if (i && o && k) {
        input_file = fopen(input_path, "rb");
        output_file = fopen(output_path, "wb");
        key_file = fopen(key_path, "r");
        if (input_file && output_file && key_file) {
            gmp_fscanf(key_file, "%Zd %Zd %Zd", kn, ke, kd);
            fclose(key_file);
            kp = keypair_init_p(kn, ke, kd);
        }
        if (e) {
            keypair_file_encrypt(kp, input_file, output_file);
        } else {
            keypair_file_decrypt(kp, input_file, output_file);
        }
        fclose(input_file);
        fclose(output_file);
        mpz_clears(kn, ke, kd, NULL);
        keypair_free(kp);
    } else {
        return 1;
    }
}
