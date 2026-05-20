/**
 * @file  inode.c
 * @brief inode functions
 *
 */

#include "mount.h"
#include "sector.h"
#include <inttypes.h>
#include <stdio.h>
#include "unixv6fs.h"
#include "inode.h"
#include "error.h"
#include <string.h>

int inode_read(const struct unix_filesystem *u, uint16_t inr, struct inode *inode)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(inode);

    return ERR_NOT_IMPLEMENTED;
}

int inode_scan_print(const struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);

    return ERR_NOT_IMPLEMENTED;
}

int inode_findsector(const struct unix_filesystem *u, const struct inode *i, int32_t file_sec_off)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(i);

    return ERR_NOT_IMPLEMENTED;
}
