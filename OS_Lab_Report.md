# Operating Systems Lab Report: Virtual Memory and Page Replacement

## Introduction
This report summarizes the implementation details and experimental outputs for three xv6 kernel and user-space memory management tasks: Page Table Walk Printing (Q1), Lazy Page Allocation via Page Fault Handling (Q2), and Page Replacement Algorithms Comparison (Q3).

---

## Question 1: Printing the Process Page Table (`vmprint`)
### Changes Implemented:
* **`vm.c`**: Added the `vmprint(pde_t *pgdir, int pid, char *name)` function which iterates through the first 512 entries of the page directory (strictly below `KERNBASE`), checks present flags (`PTE_P`), walks page table entries, and filters for valid user pages (`PTE_U`). It outputs the Virtual Address (VA), Physical Address (PA), and page flags (P, W, U), along with summary counts of valid user pages and page-table pages.
* **System Call Wiring**: Updated `syscall.h`, `syscall.c`, `sysproc.c`, `defs.h`, `user.h`, and `usys.S` to register and expose `vmprint()`.
* **Test Program (`pttest.c`)**: Created a user program that prints the page table at start-up, allocates memory via `sbrk(4 * 4096)`, and prints the page table again to verify growth.

### Output Observations:
The program successfully prints the initial process page table mapping. After calling `sbrk(4 * 4096)`, exactly four new Virtual-to-Physical address mappings appear, and the total valid user page count increases by 4, confirming accurate page table traversal.

---

## Question 2: Handling Page Faults (Lazy Allocation)
### Changes Implemented:
* **`sysproc.c`**: Modified `sys_sbrk()` so that when requesting positive memory (`n > 0`), it only increments `myproc()->sz` without calling `allocuvm()` immediately, deferring physical allocation.
* **`vm.c`**: Adjusted `copyuvm()` to safely skip unmapped lazy pages rather than throwing a kernel panic. Also exposed `mappages()` globally.
* **`trap.c`**: Added a handler for trap number 14 (`T_PGFLT`). It reads the fault address using `rcr2()`, allocates a zeroed physical page via `kalloc()` and `memset()`, maps it using `mappages()` with `PTE_W | PTE_U`, increments a page fault counter (`pgfaults`), and resumes execution. Out-of-bounds accesses correctly trigger a segmentation fault and kill the process.
* **`proc.h`**: Added `uint pgfaults` to `struct proc`.
* **System Call Wiring & Test (`faulttest.c`)**: Added `getfaults()` system call and wrote `faulttest.c` to verify that touching 5 distinct pages after a large `sbrk()` triggers exactly 5 page faults, and out-of-bounds writes safely kill only the faulting process.

### Output Observations:
`faulttest` reports zero immediate physical allocation during `sbrk()`, handles page faults lazily on touch, returns a precise fault count of 5, and safely terminates upon encountering an invalid memory access above `sz`.

---

## Question 3: Page Replacement Algorithms (FIFO and LRU)
### Changes Implemented:
* **`pagerepl.c`**: Implemented a standalone user program simulating FIFO and LRU page replacement using the reference string `{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}` across 3, 4, and 5 frames. It tracks hits, faults, frame contents, and calculates hit ratios.
* **`Makefile`**: Updated `UPROGS` to compile `pttest`, `faulttest`, and `pagerepl`.

### Output Observations:
* **FIFO (3 Frames)**: Resulted in **15 page faults**.
* **LRU (3 Frames)**: Resulted in **12 page faults** (demonstrating superior performance due to recency tracking).
* **Summary Table**: Successfully ran and tabulated performance metrics across 3, 4, and 5 frames for both algorithms.
