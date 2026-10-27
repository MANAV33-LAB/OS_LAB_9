#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "--- Before sbrk ---\n");
  vmprint();

  printf(1, "\nCalling sbrk(4 * 4096)...\n");
  sbrk(4 * 4096);

  printf(1, "\n--- After sbrk ---\n");
  vmprint();

  exit();
}
