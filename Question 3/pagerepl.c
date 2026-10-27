#include "types.h"
#include "stat.h"
#include "user.h"

int ref_str[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
int n_ref = 20;

void print_frames(int *frames, int n) {
  int i;
  for(i=0; i<n; i++) {
    if(frames[i] == -1) printf(1, "[ ] ");
    else printf(1, "[%d] ", frames[i]);
  }
}

int run_fifo(int n_frames, int detailed) {
  int frames[10];
  int i, j;
  int faults = 0;
  int fifo_idx = 0;
  int page, hit;

  for(i=0; i<n_frames; i++) frames[i] = -1;

  if(detailed) printf(1, "\n--- FIFO with %d frames ---\n", n_frames);
  for(i=0; i<n_ref; i++) {
    page = ref_str[i];
    hit = 0;
    for(j=0; j<n_frames; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    if(!hit) {
      faults++;
      frames[fifo_idx] = page;
      fifo_idx = (fifo_idx + 1) % n_frames;
    }
    if(detailed) {
      printf(1, "Ref: %d -> ", page);
      print_frames(frames, n_frames);
      if(hit) printf(1, "Hit\n");
      else printf(1, "Fault\n");
    }
  }
  if(detailed) printf(1, "Total FIFO Faults: %d\n", faults);
  return faults;
}

int run_lru(int n_frames, int detailed) {
  int frames[10];
  int time[10];
  int i, j;
  int faults = 0;
  int current_time = 0;
  int page, hit, lru_idx, min_time;

  for(i=0; i<n_frames; i++) { frames[i] = -1; time[i] = 0; }

  if(detailed) printf(1, "\n--- LRU with %d frames ---\n", n_frames);
  for(i=0; i<n_ref; i++) {
    current_time++;
    page = ref_str[i];
    hit = 0;
    for(j=0; j<n_frames; j++) {
      if(frames[j] == page) {
        hit = 1;
        time[j] = current_time;
        break;
      }
    }
    if(!hit) {
      faults++;
      lru_idx = 0;
      min_time = current_time + 1;
      for(j=0; j<n_frames; j++) {
        if(frames[j] == -1) { lru_idx = j; break; } 
        if(time[j] < min_time) {
          min_time = time[j];
          lru_idx = j;
        }
      }
      frames[lru_idx] = page;
      time[lru_idx] = current_time;
    }
    if(detailed) {
      printf(1, "Ref: %d -> ", page);
      print_frames(frames, n_frames);
      if(hit) printf(1, "Hit\n");
      else printf(1, "Fault\n");
    }
  }
  if(detailed) printf(1, "Total LRU Faults: %d\n", faults);
  return faults;
}

int main(void) {
  int f, fifo_f, lru_f, fifo_hit_pct, lru_hit_pct;

  run_fifo(3, 1);
  run_lru(3, 1);

  printf(1, "\n--- Performance Table ---\n");
  printf(1, "Frames | FIFO Faults | FIFO Hit(pct) | LRU Faults | LRU Hit(pct)\n");
  for(f=3; f<=5; f++) {
    fifo_f = run_fifo(f, 0);
    lru_f = run_lru(f, 0);
    fifo_hit_pct = ((n_ref - fifo_f) * 100) / n_ref;
    lru_hit_pct = ((n_ref - lru_f) * 100) / n_ref;
    printf(1, "   %d   |      %d     |      %d       |     %d     |      %d\n",
           f, fifo_f, fifo_hit_pct, lru_f, lru_hit_pct);
  }
  exit();
}
