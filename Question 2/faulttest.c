#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "Testing Lazy Allocation...\n");
  
  uint sz = (uint)sbrk(0);
  printf(1, "Current size: %d\n", sz);
  
  sbrk(32 * 4096);
  printf(1, "Called sbrk(32 * 4096), size is now %d\n", (uint)sbrk(0));
  
  char *p = (char*)sz;
  p[0] = 'a';
  p[4096] = 'b';
  p[8192] = 'c';
  p[12288] = 'd';
  p[16384] = 'e';
  
  printf(1, "Faults handled: %d (Expected: 5)\n", getfaults());
  
  printf(1, "\nTriggering out of bounds access (should segfault and kill process)...\n");
  p[35 * 4096] = 'z';
  
  printf(1, "If you see this, it failed!\n");
  exit();
}
