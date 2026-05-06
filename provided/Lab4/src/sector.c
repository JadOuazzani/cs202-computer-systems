/**
 * @file  sector.c
 * @brief block-level accessor functions
 *
 */

#include "sector.h"
#include "error.h"
#include "unixv6fs.h"

int sector_read(FILE *f, uint32_t sector, void *data)
{
    M_REQUIRE_NON_NULL(f);
    M_REQUIRE_NON_NULL(data);

    return ERR_NONE;
}

