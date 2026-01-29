#include "crypto.h"
#include <string.h>

void xor_payload(unsigned char *data, size_t len, const char *key) {
    if (!key || len == 0) return;
    size_t key_len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[i % key_len];
    }
}
