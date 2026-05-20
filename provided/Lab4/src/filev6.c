/**
 * @file filev6.c
 * @brief file part of inode/file layer
 *
 */

#include <string.h> // memcpy()
#include "util.h"
#include "error.h"
#include "filev6.h"
#include "inode.h"
#include "sector.h"

int filev6_open(struct unix_filesystem *u, uint16_t inr, struct filev6 *fv6)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(fv6);

    fv6->u        = u;
    fv6->i_number = inr;
    fv6->offset   = 0;

    int err = ERR_NONE;
    if ((err = inode_read(u, inr, &fv6->i_node)) != ERR_NONE) {
        debug_printf("Could not read from inode %d: %s (%d)\n", inr, ERR_MESSAGES[err - ERR_FIRST], err);
    }

    return err;
}

int filev6_readblock(struct filev6 *fv6, void *buf)
{
    M_REQUIRE_NON_NULL(fv6);
    M_REQUIRE_NON_NULL(buf);

    return ERR_NOT_IMPLEMENTED;
}
