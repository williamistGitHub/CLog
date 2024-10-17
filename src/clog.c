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
#include <time.h>

#define MAX_OUT_LEN 2048

#define cstrlen(str) (sizeof(str) / sizeof(str[0]))

static clog_log_level_e g_log_level = CLOG_LEVEL_DEBUG;
static int g_append_newline = 1;
static FILE* g_log_file = NULL;
static int g_has_registered_atexit = 0;
static clog_log_callback_t g_log_callback = NULL;
static int g_log_formatted = 0;

void clog_set_log_level(clog_log_level_e level) {
    g_log_level = level;
}

void clog_set_append_newline(int append) {
    g_append_newline = append;
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

    if (filename == NULL) {
        return;
    }

    g_log_file = fopen(filename, "w");

    if (g_log_file == NULL) {
        clog_log(CLOG_LEVEL_ERROR, "Failed to open '%s'!%s", filename, g_append_newline == 1 ? "" : "\n");
        return;
    }

    if (!g_has_registered_atexit) {
        atexit(clog_close_log_file);
    }
}

void clog_set_log_callback(clog_log_callback_t callback, int useFormatted) {
    g_log_callback = callback;
    g_log_formatted = useFormatted;
}

void clog_log(clog_log_level_e level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    clog_logv(level, fmt, ap);

    va_end(ap);
}

void clog_logv(clog_log_level_e level, const char* fmt, va_list args) {    
    time_t now;
    struct tm* t;
    char timestamp[cstrlen("00:00:00")];

    char ansifmt[MAX_OUT_LEN];
    char cleanfmt[MAX_OUT_LEN];

    const char* levelansi;
    const char* levelstr;

    int printflen;

    if (level < g_log_level) {
        return;
    }

    if (g_log_callback != NULL && !g_log_formatted) {
        /* re-use cleanfmt, dont need more memory for this crap lol */
        printflen = vsnprintf(cleanfmt, MAX_OUT_LEN, fmt, args);
        if (printflen >= 0 && printflen < MAX_OUT_LEN) {
            cleanfmt[printflen] = '\0'; /* null terminate */
        }

        g_log_callback(level, cleanfmt, printflen);
    }

    /* format timestamp */
    now = time(NULL);
    t = localtime(&now);
    sprintf(timestamp, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

    memset(ansifmt, 0, sizeof(ansifmt));
    memset(cleanfmt, 0, sizeof(cleanfmt));

    switch (level) {
        case CLOG_LEVEL_DEBUG:
            levelansi = "\x1b[1;34m";
            levelstr = "DEBUG";
            break;
        case CLOG_LEVEL_INFO:
            levelansi = "\x1b[1;32m";
            levelstr = "INFO ";
            break;
        case CLOG_LEVEL_WARN:
            levelansi = "\x1b[1;33m";
            levelstr = "WARN ";
            break;
        case CLOG_LEVEL_ERROR:
            levelansi = "\x1b[1;31m";
            levelstr = "ERROR";
            break;
        default:
            clog_log(CLOG_LEVEL_WARN, "Unknown log level %d.%s", level, g_append_newline == 1 ? "" : "\n");
            return;
    }

    if (g_append_newline) {
        snprintf(ansifmt, MAX_OUT_LEN, "\x1b[0;37m[%s%s\x1b[0;37m] [%s] \x1b[0m%s\n", levelansi, levelstr, timestamp, fmt);
        printflen = snprintf(cleanfmt, MAX_OUT_LEN, "[%s] [%s] %s\n", levelstr, timestamp, fmt);
    } else {
        snprintf(ansifmt, MAX_OUT_LEN, "\x1b[0;37m[%s%s\x1b[0;37m] [%s] \x1b[0m%s", levelansi, levelstr, timestamp, fmt);
        printflen = snprintf(cleanfmt, MAX_OUT_LEN, "[%s] [%s] %s", levelstr, timestamp, fmt);
    }

    if (g_log_callback != NULL && g_log_formatted) {
        g_log_callback(level, cleanfmt, printflen);
    }

    if (level == CLOG_LEVEL_ERROR) {
        vfprintf(stderr, ansifmt, args);
    } else {
        vprintf(ansifmt, args);
    }

    if (g_log_file != NULL) {
        vfprintf(g_log_file, cleanfmt, args);
    }
}

