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

    // Ouvre le fichier associé à l'inode (propage les erreurs : inode
    // hors limites, non alloué, etc.)
    int err = filev6_open(u, inr, &d->fv6);
    if (err != ERR_NONE) {
        return err;
    }

    // L'inode doit être un répertoire
    if (!(d->fv6.i_node.i_mode & IFDIR)) {
        return ERR_INVALID_DIRECTORY_INODE;
    }

    // Curseur au début
    d->cur  = 0;
    d->last = 0;

    return ERR_NONE;
}

int direntv6_readdir(struct directory_reader *d, char *name, uint16_t *child_inr)
{
    M_REQUIRE_NON_NULL(d);
    M_REQUIRE_NON_NULL(name);
    M_REQUIRE_NON_NULL(child_inr);

    // Tous les enfants du secteur en cache ont été consommés
    // → on charge le secteur suivant
    if (d->cur == d->last) {
        int n = filev6_readblock(&d->fv6, d->dirs);
        if (n < 0) {
            return n;                       // erreur de couche inférieure
        }
        if (n == 0) {
            return 0;                       // plus aucun enfant à lire
        }
        d->last += n / (int) sizeof(struct direntv6);
    }

    // Index de l'enfant courant dans le secteur mis en cache (dirs[])
    int idx = d->cur % DIRENTRIES_PER_SECTOR;

    // Copie du nom + null-terminaison obligatoire
    strncpy(name, d->dirs[idx].d_name, DIRENT_MAXLEN);
    name[DIRENT_MAXLEN] = '\0';

    *child_inr = d->dirs[idx].d_inumber;

    d->cur++;
    return 1;
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
