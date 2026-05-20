/**
 * @file direntv6.c
 * @brief directory layer
 *
 */

#include <string.h>
#include <stdlib.h>

#include "direntv6.h"
#include "error.h"
#include "inode.h"
#include "sector.h"

int direntv6_opendir(struct unix_filesystem *u, uint16_t inr, struct directory_reader *d)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(d);

    return ERR_NOT_IMPLEMENTED;
}

int direntv6_readdir(struct directory_reader *d, char *name, uint16_t *child_inr)
{
    M_REQUIRE_NON_NULL(d);
    M_REQUIRE_NON_NULL(name);
    M_REQUIRE_NON_NULL(child_inr);

    return ERR_NOT_IMPLEMENTED;
}

static int direntv6_dirlookup_core(struct unix_filesystem *u, uint16_t parent_inr, const char *entry, size_t entry_len)
{

    return ERR_NOT_IMPLEMENTED;
}

int direntv6_dirlookup(struct unix_filesystem *u, uint16_t parent_inr, const char *entry)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(entry);

    return direntv6_dirlookup_core(u, parent_inr, entry, strlen(entry));
}

int direntv6_print_tree(struct unix_filesystem *u, uint16_t inr, const char *prefix)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(prefix);

    return ERR_NOT_IMPLEMENTED;
}
