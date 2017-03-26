#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stash.h"

const int SIZE = 1000000000;
const int MAX_ENTRIES = 250000;

struct stash * init_stash(){
  struct stash * s = malloc(sizeof(struct stash));
  s->entries = malloc(sizeof(struct stash_entry) * MAX_ENTRIES);
  s->memory = calloc(1, sizeof(char) * SIZE);
  s->index = 0;
  s->num_entries = 0;
  s->remaining = SIZE;
  return s;
}

void * store_in_stash(void *object, size_t size, struct stash * s){
  if(size > s->remaining || s->num_entries >= MAX_ENTRIES){
    return NULL;
  }
  bool found = false;
  uint32_t index = 0;
  for(uint32_t i = 0; i < s->num_entries; i++){
    if(s->entries[i].size == size && s->entries[i].isFree){
      found = true;
      index = i;
    }
  }
  if(found){
    s->entries[index].isFree = false;
    void * dest = s->entries[index].pntr;
    void * result = memcpy(dest, object, size);
    return result;
  } else {
    char * dest = &s->memory[s->index];
    void * result = memcpy(dest, object, size);
    struct stash_entry entry = {
      .pntr = result,
      .start = s->index,
      .size = size,
      .isFree = false
    };
    s->entries[s->num_entries] = entry;
    s->num_entries++;
    s->index += size;
    s->remaining -= size;
    return result;
  }
}

bool free_object(struct stash * s, void * object){
  for(uint32_t i = 0; i < s->num_entries; i++){
    if((int)object == (int)s->entries[i].pntr){
      if(i == s->num_entries-1){
        s->num_entries--;
      } else {
        s->entries[i].isFree = true;
        return true;
      }
    }
  }
  return false;
}
