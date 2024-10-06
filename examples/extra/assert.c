/*
* CLog Assert Example
*
* This file is released into the public domain under CC0:
*   https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "clog/extra.h" /* Extra includes clog itself */

int main() {
  clog_assert(2 + 2 == 4); /* Should pass last time I checked */
  clog_assert(2 + 2 == 5); /* Should fail last time I checked */

  return 0;
}
