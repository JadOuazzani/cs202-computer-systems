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

    return ERR_NOT_IMPLEMENTED;
}

int umountv6(struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);

    return ERR_NOT_IMPLEMENTED;
}
