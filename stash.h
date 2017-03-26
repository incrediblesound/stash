#ifndef _STASH_H
#define _STASH_H

#include <stdbool.h>
#include <stdint.h>

extern const int SIZE;
extern const int MAX_ENTRIES;

struct stash_entry {
  void * pntr;
  uint32_t start;
  uint32_t size;
  bool isFree;
};

struct stash {
  char * memory;
  uint32_t index;
  uint32_t remaining;
  uint32_t num_entries;
  struct stash_entry * entries;
};

struct stash * init_stash();
void * store_in_stash(void *, size_t, struct stash *);
bool free_object(struct stash *, void *);

#endif
