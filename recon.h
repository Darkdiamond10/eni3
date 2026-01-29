#ifndef RECON_H
#define RECON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char kernel_version[256];
    int has_sudo;
    int vulnerable_pkexec;
} system_info_t;

void audit_system(system_info_t *info);

#endif
