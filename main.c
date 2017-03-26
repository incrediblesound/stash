#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

const int SIZE = 1000000000;
const int MAX_ENTRIES = 250000;

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

struct stash * init_stash(){
  struct stash * s = malloc(sizeof(struct stash));
  s->entries = malloc(sizeof(struct stash_entry) * MAX_ENTRIES);
  s->memory = calloc(1, sizeof(char) * SIZE);
  s->index = 0;
  s->num_entries = 0;
  s->remaining = SIZE;
  return s;
}

//void *memcpy(void *dest, void *src, size_t count);

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
      s->entries[i].isFree = true;
      return true;
    }
  }
  return false;
}

struct person {
  int age;
  char name[20];
};

void store_people(struct stash * s, struct person * people[]){
  struct person john_copy = { .age = 21, .name = "John" };
  people[0] = store_in_stash(&john_copy, sizeof(struct person), s);

  struct person jane_copy = { .age = 13, .name = "Jane" };
  people[1] = store_in_stash(&jane_copy, sizeof(struct person), s);
}

int main(){
  struct stash * s = init_stash();
  struct person * people[2];
  store_people(s, people);
  printf("%d\n", people[0]->age);
  printf("%d\n", people[1]->age);
  bool freed = free_object(s, people[1]);
  struct person doug = { .age = 30, .name = "Doug" };
  people[1] = store_in_stash(&doug, sizeof(struct person), s);
  printf("%d\n", people[1]->age);
  printf("%d\n", s->num_entries);
  return 0;
}
