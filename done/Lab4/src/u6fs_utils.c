/**
 * @file u6fs_utils.c
 * @brief Utilities (mostly dump) for UV6 filesystem
 */

#include <string.h> // memset
#include <inttypes.h>
#include <openssl/sha.h>
#include "mount.h"
#include "sector.h"
#include "error.h"
#include "u6fs_utils.h"
#include "inode.h"
#include "filev6.h"
#include "util.h"

int utils_print_superblock(struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);
    pps_printf("**********FS SUPERBLOCK START**********\n");
    pps_printf("%-20s: %" PRIu16 "\n", "s_isize",       u->s.s_isize      );
    pps_printf("%-20s: %" PRIu16 "\n", "s_fsize",       u->s.s_fsize      );
    pps_printf("%-20s: %" PRIu16 "\n", "s_fbmsize",     u->s.s_fbmsize    );
    pps_printf("%-20s: %" PRIu16 "\n", "s_ibmsize",     u->s.s_ibmsize    );
    pps_printf("%-20s: %" PRIu16 "\n", "s_inode_start", u->s.s_inode_start);
    pps_printf("%-20s: %" PRIu16 "\n", "s_block_start", u->s.s_block_start);
    pps_printf("%-20s: %" PRIu16 "\n", "s_fbm_start",   u->s.s_fbm_start  );
    pps_printf("%-20s: %" PRIu16 "\n", "s_ibm_start",   u->s.s_ibm_start  );
    pps_printf("%-20s: %" PRIu8 "\n", "s_flock",        u->s.s_flock      );
    pps_printf("%-20s: %" PRIu8 "\n", "s_ilock",        u->s.s_ilock      );
    pps_printf("%-20s: %" PRIu8 "\n", "s_fmod",         u->s.s_fmod       );
    pps_printf("%-20s: %" PRIu8 "\n", "s_ronly",        u->s.s_ronly      );
    pps_printf("%-20s: [%" PRIu16 "] %" PRIu16 "\n", "s_time", u->s.s_time[0], u->s.s_time[1]);
    pps_printf("**********FS SUPERBLOCK END**********\n");
    return ERR_NONE;
}

static void utils_print_SHA_buffer(unsigned char *buffer, size_t len)
{
    unsigned char sha[SHA256_DIGEST_LENGTH];
    SHA256(buffer, len, sha);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        pps_printf("%02x", sha[i]);
    }
    pps_printf("\n");
}

int utils_print_inode(const struct inode *in)
{
    pps_printf("**********FS INODE START**********\n");

    if (in) {
        pps_printf("%s: %" PRIu16 "\n", "i_mode", in->i_mode);
        pps_printf("%s: %" PRIu8 "\n", "i_nlink", in->i_nlink);
        pps_printf("%s: %" PRIu8 "\n", "i_uid", in->i_uid);
        pps_printf("%s: %" PRIu8 "\n", "i_gid", in->i_gid);
        pps_printf("%s: %" PRIu8 "\n", "i_size0", in->i_size0);
        pps_printf("%s: %" PRIu16 "\n", "i_size1", in->i_size1);
        pps_printf("%s: %" PRIu32 "\n", "size", inode_getsize(in));
    } else {
        pps_printf("NULL ptr\n");
    }

    pps_printf("**********FS INODE END************\n");

    return ERR_NONE;
}

int utils_cat_first_sector(struct unix_filesystem *u, uint16_t inr)
{
    M_REQUIRE_NON_NULL(u);

    struct filev6 fv6;
    int err = filev6_open(u, inr, &fv6);
    if (err != ERR_NONE) {
        pps_printf("filev6_open failed for inode #%d.\n", inr);
        return err;
    }

    pps_printf("\nPrinting inode #%d:\n", inr);
    if ((err = utils_print_inode(&fv6.i_node)) != ERR_NONE) {
        return err;
    }

    if (fv6.i_node.i_mode & IFDIR) {
        pps_printf("which is a directory.\n");
    } else {
        pps_printf("the first sector of data of which contains:\n");

        char buf[SECTOR_SIZE + 1];
        memset(buf, 0, sizeof(buf));

        int n = filev6_readblock(&fv6, buf);
        if (n < 0) return n;

        buf[n] = '\0';          // null-terminé pour %s
        pps_printf("%s", buf);
        pps_printf("----\n");
    }

    return ERR_NONE;
}

int utils_print_shafile(struct unix_filesystem *u, uint16_t inr)
{
    M_REQUIRE_NON_NULL(u);

    pps_printf("SHA inode %d: ", inr);

    return ERR_NOT_IMPLEMENTED;
}

int utils_print_sha_allfiles(struct unix_filesystem *u)
{
    M_REQUIRE_NON_NULL(u);

    pps_printf("Listing inodes SHA\n");

    int err = ERR_NONE;
    for (uint32_t i = ROOT_INUMBER; i < u->s.s_isize * INODES_PER_SECTOR; ++i) {
        err = utils_print_shafile(u, (uint16_t) i);
        if (err != ERR_NONE && err != ERR_UNALLOCATED_INODE) {
            return err;
        }
    }

    return ERR_NONE;
}
