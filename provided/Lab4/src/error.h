#pragma once

/**
 * @file error.h
 * @brief file system error codes
 *
 * @author J.-C. Chappelier, E. Bugnion
 * @date 2017-2026
 */

#pragma once

#include <assert.h>
#include <stdio.h> // for fprintf

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief filesytem internal error codes.
 *
 */
#define ERR_NONE 0

enum error_codes {
    ERR_FIRST = -128, // not an actual error but to set the first error number
    ERR_INVALID_COMMAND,
    ERR_NOMEM,
    ERR_IO,
    ERR_BAD_BOOT_SECTOR,
    ERR_INODE_OUT_OF_RANGE,
    ERR_FILENAME_TOO_LONG,
    ERR_INVALID_DIRECTORY_INODE,
    ERR_UNALLOCATED_INODE,
    ERR_FILENAME_ALREADY_EXISTS,
    ERR_BITMAP_FULL,
    ERR_FILE_TOO_LARGE,
    ERR_OFFSET_OUT_OF_RANGE,
    ERR_BAD_PARAMETER,
    ERR_NO_SUCH_FILE,
    ERR_NOT_IMPLEMENTED,
    ERR_LAST // not an actual error but to have e.g. the total number of errors
};

/*
 * Helpers (macros)
 */

#ifdef DEBUG
#define debug_printf(fmt, ...) \
    fprintf(stderr, "### function %s in %s:%d " fmt, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define debug_var(var, fmt) debug_printf(#var ": " fmt "\n", var)
#define debug_str(var)      debug_var(var, "%s")
#define debug_int(var)      debug_var(var, "%d")
#define debug_err(fmt, ...) debug_printf("ERROR: " fmt, __VA_ARGS__)
#define debug_ret_err(err)  do { debug_err("code %d\n", err); return err; } while(0)
#else
#define debug_printf(fmt, ...) \
    do {} while(0)
#endif

#define M_REQUIRE_NON_NULL(arg) \
    do { \
        if (arg == NULL) { \
            debug_printf("ERROR: parameter %s is NULL when calling  %s() (defined in %s)\n", \
                        #arg, __func__, __FILE__); \
            return ERR_BAD_PARAMETER; \
        } \
    } while(0)


#define pps_printf printf

/**
* @brief filesystem internal error messages. defined in error.c
*
*/
extern const char * const ERR_MESSAGES[];

/**
 * @brief yields the error message corresponding to an error code
 */
#define ERR_MSG(err)                                                           \
    (ERR_FIRST < (err) && (err) < ERR_LAST ? ERR_MESSAGES[(err) - ERR_FIRST]   \
                                           : "Invalid error code")

#ifdef __cplusplus
}
#endif
