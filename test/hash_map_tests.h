#ifndef HASH_MAP_TESTS_H
#define HASH_MAP_TESTS_H

#include "lib/greatest.h"
#include "cct_test_common.h"
#include "../collection_templates.h"

unsigned int test_str_hash_func(char* str) {
  unsigned int val = 0;
  int times = sizeof(val) / sizeof(char);
  int offset = 0;
  while (*str) {
    val ^= ((unsigned int)*str << offset * 8);
    offset++;
    if (offset == times) {
      offset = 0;
    }
    str++;
  }
  return val;
}

bool test_str_eq_func(char* a, char* b) {
  while(*a && *b && *a == *b) {
    a++;
    b++;
  }
  return (*a == *b);
}


define_hash_map(char*, int, my, test_str_hash_func, test_str_eq_func)

typedef struct my_hash_map hash;
typedef struct my_hash_map_pair hash_pair;

TEST prefix_hash_map_create () {
  SKIP();
}

TEST prefix_hash_map_destroy () {
  SKIP();
}

TEST prefix_hash_map_put () {
  SKIP();
}

TEST prefix_hash_map_get () {
  SKIP();
}

TEST prefix_hash_map_contains () {
  SKIP();
}

TEST prefix_hash_map_remove () {
  SKIP();
}

SUITE(hash_map) {
  RUN_TEST(prefix_hash_map_create);
  RUN_TEST(prefix_hash_map_destroy);
  RUN_TEST(prefix_hash_map_put);
  RUN_TEST(prefix_hash_map_get);
  RUN_TEST(prefix_hash_map_contains);
  RUN_TEST(prefix_hash_map_remove);
}
#endif
