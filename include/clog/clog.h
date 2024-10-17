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

#ifndef CLOG_H
#define CLOG_H

#include <stdarg.h>

/**
 * @brief Log levels supported by CLog.
 * 
 */
typedef enum {
    CLOG_LEVEL_DEBUG = 0,
    CLOG_LEVEL_INFO = 1,
    CLOG_LEVEL_WARN = 2,
    CLOG_LEVEL_ERROR = 3
} clog_log_level_e;

/**
 * @brief Booleans for use with CLog.
 * 
 */
typedef enum {
    CLOG_FALSE = 0,
    CLOG_TRUE = 1
} clog_bool_e;

typedef void (*clog_log_callback_t)(clog_log_level_e level, char* message, int messageLen);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets the current log level. Any messages "lower" than this will not be logged.
 * 
 * @param level The new log level.
 */
void clog_set_log_level(clog_log_level_e level);
/**
 * @brief Sets whether a newline should automatically be appended to all logs.
 *
 * @param append Should we append `\\n`?
 */
void clog_set_append_newline(clog_bool_e append);
/**
 * @brief Sets the current log file. Messages will be logged here as well if it is not null.
 * 
 * @param filename New log filename, `NULL` is accepted and means no log file.
 */
void clog_set_log_file(const char* filename);
/**
 * @brief Sets a callback to be called whenever a log message happens. Gets the log level as well as the log message.
 * 
 * Note that logging inside the callback without proper safeguards will cause infinite recursion and a stack overflow.
 * 
 * @param callback The callback to set, or NULL to remove an already set callback.
 */
void clog_set_log_callback(clog_log_callback_t callback);

/**
 * @brief Log, using varargs.
 * 
 * @param level Log level to use.
 * @param fmt `printf`-style format string to log. `\\n` is appended depending on clog_set_append_newline().
 * @param ... Arguments to the format string.
 */
void clog_log(clog_log_level_e level, const char* fmt, ...)
#ifdef __GNUC__
__attribute__((format(printf, 2, 3)))
#endif
;

/**
 * @brief Log, using `va_list`.
 * 
 * @param level Log level to use.
 * @param fmt `printf`-style format string to log. \\n is appended automatically.
 * @param args Arguments to the format string.
 */
void clog_logv(clog_log_level_e level, const char* fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif
