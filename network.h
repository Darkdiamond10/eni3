#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

unsigned char* download_stage(const char *host, const char *path, size_t *out_len);

#endif
