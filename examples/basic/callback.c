/*
 * CLog Callback Example
 *
 * This file is released into the public domain under CC0:
 *   https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "clog/clog.h"
#include "clog/extra.h"

#include <stdio.h>

void callback(clog_log_level_e level, char* message, int messageLen) {
    /* Note that logging here will cause a stack overflow, due do the callback
       itself being called. */
    printf("Callback called with level %s, message: '%s' (%d chars long)\n", clog_level_to_str(level), message, messageLen);
}

int main(void) {
    clog_set_log_callback(callback);
    clog_set_log_level(CLOG_LEVEL_INFO);

    clog_log(CLOG_LEVEL_INFO, "Sample log message.");

    clog_log(CLOG_LEVEL_DEBUG, "Note that callbacks do not get called for messages that do not get printed to stdout.");
}