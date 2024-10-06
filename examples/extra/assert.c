/*
* CLog Assert Example
*/

#include "clog/extra.h" // Extra include clog itself

int main() {
  clog_assert(2 + 2 == 4); // Should pass last time I checked
  clog_assert(2 + 2 == 5); // Should fail last time I checked

  return 0;
}
