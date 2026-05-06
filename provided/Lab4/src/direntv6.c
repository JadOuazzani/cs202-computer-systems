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

int direntv6_opendir(const struct unix_filesystem *u, uint16_t inr, struct directory_reader *d)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(d);

    return ERR_NONE;
}

int direntv6_readdir(struct directory_reader *d, char *name, uint16_t *child_inr)
{
    M_REQUIRE_NON_NULL(d);
    M_REQUIRE_NON_NULL(name);
    M_REQUIRE_NON_NULL(child_inr);

    return 1;
}

static int direntv6_find(struct directory_reader *d, const char *name, size_t name_len)
{
    M_REQUIRE_NON_NULL(d);
    M_REQUIRE_NON_NULL(name);

    return ERR_NO_SUCH_FILE;
}

static int direntv6_dirlookup_core(const struct unix_filesystem *u, uint16_t parent_inr, const char *entry, size_t entry_len)
{

    return ERR_NOT_IMPLEMENTED;
}

int direntv6_dirlookup(const struct unix_filesystem *u, uint16_t parent_inr, const char *entry)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(entry);

    return direntv6_dirlookup_core(u, parent_inr, entry, strlen(entry));
}

int direntv6_print_tree(const struct unix_filesystem *u, uint16_t inr, const char *prefix)
{
    M_REQUIRE_NON_NULL(u);
    M_REQUIRE_NON_NULL(prefix);

    return ERR_NONE;
}
