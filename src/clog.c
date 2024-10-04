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
static FILE* g_log_file = NULL;
static int g_has_registered_atexit = 0;

void clog_set_log_level(clog_log_level_e level) {
    g_log_level = level;
}

static void clog_close_log_file(void) {
    if (g_log_file != NULL) {
        fflush(g_log_file);
        fclose(g_log_file);

        g_log_file = NULL;
    }
}

void clog_set_log_file(const char* filename) {
    clog_close_log_file();

    g_log_file = fopen(filename, "w");

    if (g_log_file == NULL) {
        clog_log(CLOG_LEVEL_ERROR, "Failed to open '%s'!", filename);
        return;
    }

    if (!g_has_registered_atexit) {
        atexit(clog_close_log_file);
    }
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

    char ansifmt[cstrlen(LOG_LEVEL_PREFIX) + cstrlen(LOG_LEVEL_DEBUG) +
                    cstrlen(LOG_LEVEL_SUFFIX) + MAX_FMT_LEN];
    char cleanfmt[cstrlen("[     ] ") + MAX_FMT_LEN];

    memset(ansifmt, 0, sizeof(ansifmt));
    memset(cleanfmt, 0, sizeof(cleanfmt));

    strncat(ansifmt, LOG_LEVEL_PREFIX, sizeof(LOG_LEVEL_PREFIX));
    strncat(cleanfmt, "[", 1);
    
    switch (level) {
        case CLOG_LEVEL_DEBUG:
            strncat(ansifmt, LOG_LEVEL_DEBUG, sizeof(LOG_LEVEL_DEBUG));
            strncat(cleanfmt, "DEBUG", 5);
            break;
        case CLOG_LEVEL_INFO:
            strncat(ansifmt, LOG_LEVEL_INFO, sizeof(LOG_LEVEL_INFO));
            strncat(cleanfmt, "INFO ", 5);
            break;
        case CLOG_LEVEL_WARN:
            strncat(ansifmt, LOG_LEVEL_WARN, sizeof(LOG_LEVEL_WARN));
            strncat(cleanfmt, "WARN ", 5);
            break;
        case CLOG_LEVEL_ERROR:
            strncat(ansifmt, LOG_LEVEL_ERROR, sizeof(LOG_LEVEL_ERROR));
            strncat(cleanfmt, "ERROR", 5);
            break;
    }

    strncat(ansifmt, LOG_LEVEL_SUFFIX, sizeof(LOG_LEVEL_SUFFIX));
    strncat(cleanfmt, "] ", 2);

    strncat(ansifmt, fmt, fmtlen);
    strncat(cleanfmt, fmt, fmtlen);

    strncat(ansifmt, "\n", 1);
    strncat(cleanfmt, "\n", 1);

    if (level == CLOG_LEVEL_ERROR) {
        vfprintf(stderr, ansifmt, args);
    } else {
        vprintf(ansifmt, args);
    }

    if (g_log_file != NULL) {
        vfprintf(g_log_file, cleanfmt, args);
    }
}