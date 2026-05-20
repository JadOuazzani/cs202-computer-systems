/**
 * @file mount.c
 * @brief mounts the filesystem
 *
 */

#include <string.h> // memset()
#include <inttypes.h>

#include "error.h"
#include "mount.h"
#include "sector.h"
#include "util.h" // for TO_BE_IMPLEMENTED() -- remove later

int mountv6(const char *filename, struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(filename);
    M_REQUIRE_NON_NULL(u);

    memset(u, 0, sizeof(*u));

    u->f = fopen(filename, "rb"); 
    if (u->f == NULL) {
        return ERR_IO;
    }   

    uint8_t boot[SECTOR_SIZE];

    if (sector_read(u->f, BOOTBLOCK_SECTOR, boot) != ERR_NONE){
        return ERR_IO; 
    }

    if (boot[BOOTBLOCK_MAGIC_NUM_OFFSET] != BOOTBLOCK_MAGIC_NUM){
        fclose(u->f); 
        return ERR_BAD_BOOT_SECTOR; 
    }

    if (sector_read(u->f, SUPERBLOCK_SECTOR, &u->s) != ERR_NONE){
        fclose(u->f); 
        return ERR_IO; 
    }

    return ERR_NONE;
}

int umountv6(struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(u->f);

    if (fclose(u->f) != 0){
        return ERR_IO; 
    }

    return ERR_NONE;
}
