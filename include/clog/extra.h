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

#ifndef CLOG_EXTRA_H
#define CLOG_EXTRA_H

#include <stdlib.h>
#include "clog/clog.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A standin for the regular C assert macro
 *
 * @param EX The condition to check
 */
#define clog_assert(EX) (void)((EX) || (clog_log(CLOG_LEVEL_ERROR, "ASSERT FAILED: %s at %s, line %d", #EX, __FILE__, __LINE__), exit(1), 0)) /* POSIX says that assert should exit */

/**
 * @brief Returns a string representation of the log level. The string is not padded with spaces, and ranges in length.
 * 
 * Returns "UNKNOWN" for an unknown level.
 * 
 * @param level The level to convert.
 * @return Level string, in all capital letters. Do not free. 
 */
inline const char* clog_level_to_str(clog_log_level_e level) {
    switch (level) {
        case CLOG_LEVEL_DEBUG: return "DEBUG";
        case CLOG_LEVEL_INFO: return "INFO";
        case CLOG_LEVEL_WARN: return "WARN";
        case CLOG_LEVEL_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

#ifdef __cplusplus
}
#endif
#endif
