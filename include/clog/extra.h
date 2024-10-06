#ifndef CLOG_EXTRA_H
#define CLOG_EXTRA_H

#include <stdlib.h>
#include "clog.h"

#ifdef __cplusplus
extern "C" {
#endif

void __clog_assert(const char *msg, const char *file, int line) {
    clog_log(CLOG_LEVEL_ERROR, "%s at %s, line %d", msg, file, line);
    exit(1); // POSIX says that assert should exit
}

/**
 * @brief A standin for the regular C assert macro
 *
 * @param EX The condition to check
 */
#define clog_assert(EX) (void)((EX) || ((__clog_assert(#EX, __FILE__, __LINE__)), 0))

#ifdef __cplusplus
}
#endif
#endif
