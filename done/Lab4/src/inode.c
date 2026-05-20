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

    if (inr < ROOT_INUMBER || inr >= u->s.s_isize * INODES_PER_SECTOR) {
        return ERR_INODE_OUT_OF_RANGE;
    }

    uint32_t sector = u->s.s_inode_start + (inr / INODES_PER_SECTOR) ; 
    struct inode_sector isector; 
    int err = sector_read(u->f, sector, &isector); 
    if (err != ERR_NONE) return err; 

    *inode = isector.inodes[inr % INODES_PER_SECTOR]; 

    if (!(inode->i_mode & IALLOC)) {
        return ERR_UNALLOCATED_INODE;
    }
    
    return ERR_NONE;
}

int inode_scan_print(const struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);
    for (uint16_t inr = ROOT_INUMBER; inr < u->s.s_isize * INODES_PER_SECTOR; inr++){
        struct inode inode; 
        int err = inode_read(u, inr, &inode);
        if (err == ERR_UNALLOCATED_INODE) continue;
        if (err != ERR_NONE) return err; 
        pps_printf("inode %3" PRIu16 " (%s) len %4d\n", inr, (inode.i_mode & IFDIR) ? SHORT_DIR_NAME : SHORT_FIL_NAME , inode_getsize(&inode)); 
    }
    
    return ERR_NONE;
}

int inode_findsector(const struct unix_filesystem *u, const struct inode *i, int32_t file_sec_off)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(i);

    return ERR_NOT_IMPLEMENTED;
}
