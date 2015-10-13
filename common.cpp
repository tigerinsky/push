#include "common.h"
#include <openssl/md5.h> 

struct timeval nowtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  tv;
}

void md5(const char* data, int size, char* output) {
    unsigned char md[16];
    MD5((const unsigned char*)data, size, md);
    for (int i = 0; i < 16; ++i) {
        sprintf(output + (i*2),  "%02X" , md[i]);
    }   
}

