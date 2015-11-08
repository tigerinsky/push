#include "common.h"
#include <stdio.h>                               // vsnprintf
#include <string.h>                              // strlen
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

inline int string_printf_impl(std::string& output, 
                              const char* format,
                              va_list args) {
    const int write_point = output.size();
    int remaining = output.capacity() - write_point;
    output.resize(output.capacity());
    va_list copied_args;
    va_copy(copied_args, args);
    int bytes_used = vsnprintf(&output[write_point], 
                               remaining, 
                               format,
                               copied_args);
    va_end(copied_args);
    if (bytes_used < 0) {
        return -1;
    } else if (bytes_used < remaining) {
    // There was enough room, just shrink and return.
        output.resize(write_point + bytes_used);
    } else {
        output.resize(write_point + bytes_used + 1);
        remaining = bytes_used + 1;
        bytes_used = vsnprintf(&output[write_point], remaining, format, args);
        if (bytes_used + 1 != remaining) {
            return -1;
        }
        output.resize(write_point + bytes_used);
    }
    return 0;
}

std::string string_printf(const char* format, ...) {
    std::string ret;
    ret.reserve(std::max(32UL, strlen(format) * 2));
    va_list ap;
    va_start(ap, format);
    if (string_printf_impl(ret, format, ap) != 0) {
        ret.clear();
    }
    va_end(ap);
    return ret;
}

int string_appendf(std::string* output, const char* format, ...) {
                   va_list ap;
                   va_start(ap, format);
    const size_t old_size = output->size();
    const int rc = string_printf_impl(*output, format, ap);
    if (rc != 0) {
        output->resize(old_size);
    }
    va_end(ap);
    return rc;
}

int string_vappendf(std::string* output, const char* format, va_list args) {
    const size_t old_size = output->size();
    const int rc = string_printf_impl(*output, format, args);
    if (rc == 0) {
        return 0;
    }
    output->resize(old_size);
    return rc;
}

int string_printf(std::string* output, const char* format, ...) {
                  va_list ap;
                  va_start(ap, format);
                  output->clear();
    const int rc = string_printf_impl(*output, format, ap);
    if (rc != 0) {
        output->clear();
    }
    va_end(ap);
    return rc;
};

int string_vprintf(std::string* output, 
                   const char* format, 
                   va_list args) {
    output->clear();
    const int rc = string_printf_impl(*output, format, args);
    if (rc == 0) {
        return 0;
    }
    output->clear();
    return rc;
};

