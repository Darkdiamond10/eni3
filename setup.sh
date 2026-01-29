#!/bin/bash

# ENI Loader Environment Setup
# "Preparando el laboratorio..."

echo "[*] Verificando privilegios..."
if [ "$EUID" -ne 0 ]; then
  echo "[-] Por favor, ejecuta como root (sudo ./setup.sh) para instalar las dependencias."
  exit 1
fi

echo "[*] Actualizando repositorios..."
apt-get update

echo "[*] Instalando herramientas de compilación y librerías SSL..."
# libssl-dev es CRÍTICO para network.c (openssl/ssl.h)
apt-get install -y build-essential libssl-dev python3

if [ $? -eq 0 ]; then
    echo "[+] Dependencias instaladas. Entorno listo para compilar."
    echo "[*] Ejecuta 'make' para generar el payload."
else
    echo "[-] Hubo un error instalando las dependencias."
fi
