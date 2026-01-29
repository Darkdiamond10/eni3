#ifndef RECON_H
#define RECON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysinfo.h>

typedef struct {
    char kernel_version[256];
    int has_sudo; // Aún útil para saber si podemos pedir permisos educadamente
    int is_container; // Docker/LXC detection
    int num_cpus;
    unsigned long total_ram_mb;
} system_info_t;

void audit_system(system_info_t *info);
int is_sandboxed(); // 1 si detectamos análisis

#endif
