#include <stdio.h>
#include <stdlib.h>
#include "stash.h"

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
