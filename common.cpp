#include "common.h"
#include <openssl/md5.h> 
#include <openssl/sha.h>

struct timeval nowtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  tv;
}

char* md5(const char* data, int size, char* output) {
    unsigned char md[16];
    MD5((const unsigned char*)data, size, md);
    for (int i = 0; i < 16; ++i) {
        sprintf(output + (i*2),  "%02X" , md[i]);
    }   
    output[32] = '\0';
    return output;
}

char* sha1(const char* data, int size, char* output) {
    return (char*)SHA1((const unsigned char*)data, size, (unsigned char*)output);  
}
