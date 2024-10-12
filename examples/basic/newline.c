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
    clog_set_append_newline(false);

    /* Some example logs: */
    clog_log(CLOG_LEVEL_INFO, "Newline added manually!\n");

    return 0;
}
