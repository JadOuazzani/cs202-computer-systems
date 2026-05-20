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

    int32_t size = (int32_t) inode_getsize(&fv6->i_node);

    // Fin de fichier déjà atteinte
    if (fv6->offset >= size) {
        return 0;
    }

    // Quel secteur du fichier on doit lire ?
    int32_t file_sec_off = fv6->offset / SECTOR_SIZE;

    // Trouve le numéro de secteur disque correspondant
    int sector = inode_findsector(fv6->u, &fv6->i_node, file_sec_off);
    if (sector < 0) {
        return sector;  // propage l'erreur
    }

    // Lit ce secteur dans buf
    int err = sector_read(fv6->u->f, (uint32_t) sector, buf);
    if (err != ERR_NONE) {
        return err;
    }

    // Combien d'octets sont réellement utiles ?
    // (< SECTOR_SIZE si c'est le dernier secteur du fichier)
    int32_t bytes_left = size - fv6->offset;
    int32_t bytes_read = (bytes_left < SECTOR_SIZE) ? bytes_left : SECTOR_SIZE;

    fv6->offset += bytes_read;
    return (int) bytes_read;
}