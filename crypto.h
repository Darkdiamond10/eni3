#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

void xor_payload(unsigned char *data, size_t len, const char *key);

#endif
