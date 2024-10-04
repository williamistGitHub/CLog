/* 
 * This file is a part of CLog.
 *
 * (C) 2024 williamist
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:

 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 */

#include "clog/clog.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* if your string is longer than 1024, you have other problems. */
#define MAX_FMT_LEN 1024

#define LOG_LEVEL_PREFIX "\x1b[0;37m["

#define LOG_LEVEL_DEBUG "\x1b[1;34mDEBUG"
#define LOG_LEVEL_INFO  "\x1b[1;32mINFO "
#define LOG_LEVEL_WARN  "\x1b[1;33mWARN "
#define LOG_LEVEL_ERROR "\x1b[1;31mERROR"

#define LOG_LEVEL_SUFFIX "\x1b[0;37m] \x1b[0m"

#define cstrlen(str) (sizeof(str) / sizeof(str[0]))

static clog_log_level_e g_log_level = CLOG_LEVEL_DEBUG;

void clog_set_log_level(clog_log_level_e level) {
    g_log_level = level;
}

void clog_log(clog_log_level_e level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    clog_logv(level, fmt, ap);

    va_end(ap);
}

void clog_logv(clog_log_level_e level, const char* fmt, va_list args) {
    if (level < g_log_level) {
        return;
    }

    /* build final string from level and fmt */
    size_t fmtlen = strnlen_s(fmt, MAX_FMT_LEN); 

    char finalfmt[cstrlen(LOG_LEVEL_PREFIX) + cstrlen(LOG_LEVEL_DEBUG) +
                    cstrlen(LOG_LEVEL_SUFFIX) + MAX_FMT_LEN];

    memset(finalfmt, 0, sizeof(finalfmt));

    strncat(finalfmt, LOG_LEVEL_PREFIX, sizeof(LOG_LEVEL_PREFIX));
    
    switch (level) {
        case CLOG_LEVEL_DEBUG:
            strncat(finalfmt, LOG_LEVEL_DEBUG, sizeof(LOG_LEVEL_DEBUG));
            break;
        case CLOG_LEVEL_INFO:
            strncat(finalfmt, LOG_LEVEL_INFO, sizeof(LOG_LEVEL_INFO));
            break;
        case CLOG_LEVEL_WARN:
            strncat(finalfmt, LOG_LEVEL_WARN, sizeof(LOG_LEVEL_WARN));
            break;
        case CLOG_LEVEL_ERROR:
            strncat(finalfmt, LOG_LEVEL_ERROR, sizeof(LOG_LEVEL_ERROR));
            break;
    }

    strncat(finalfmt, LOG_LEVEL_SUFFIX, sizeof(LOG_LEVEL_SUFFIX));

    strncat(finalfmt, fmt, fmtlen);
    strncat(finalfmt, "\n", 1);

    vprintf(finalfmt, args);
}