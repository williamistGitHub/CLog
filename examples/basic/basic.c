/*
 * CLog Basic Example
 *
 * This file is released into the public domain under CC0:
 *   https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "clog/clog.h"

int main() {
    /* Initialize CLog. */
    clog_set_log_level(CLOG_LEVEL_INFO);
    clog_set_log_file("test.log");

    /* Some example logs: */
    clog_log(CLOG_LEVEL_INFO, "Hello from CLog!");

    /* As the log level is set to CLOG_LEVEL_INFO, this message does not log. */
    clog_log(CLOG_LEVEL_DEBUG, "This message should not display.");

    clog_log(CLOG_LEVEL_WARN, "This is a warning.");
    clog_log(CLOG_LEVEL_ERROR, "This is an error.");

    /* You can change the log level at runtime. */
    clog_set_log_level(CLOG_LEVEL_DEBUG);

    /* Logging supports printf-style formatting. */
    int test = 69;
    clog_log(CLOG_LEVEL_DEBUG, "Debug should log now: %d", test);

    return 0;
}