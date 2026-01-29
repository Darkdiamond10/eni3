#ifndef MEMEXEC_H
#define MEMEXEC_H

#include <stddef.h>

int execute_from_mem(const unsigned char *payload, size_t payload_len, char *const argv[], char *const envp[]);

#endif
