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

    if (fseek(f, (long)sector*SECTOR_SIZE, SEEK_SET) != 0){
        return ERR_IO; 
    }

    if (fread(data, SECTOR_SIZE, 1, f) != 1){
        return ERR_IO; 
    }

    return ERR_NONE;
}

