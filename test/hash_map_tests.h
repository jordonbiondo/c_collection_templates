/*
 *  Copyright 2015 Jordon Biondo
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef HASH_MAP_TESTS_H
#define HASH_MAP_TESTS_H

#include "cct_test_common.h"

define_hash_map(char*, int, test, CCT_DEFAULT_STR_HASH, CCT_DEFAULT_STR_EQ)

typedef struct test_hash_map hash_t;
typedef struct test_hash_map_pair hash_pair_t;

TEST prefix_hash_map_create () {
  hash_t* hash;

  with_spoofed_oom {
    hash = test_hash_map_create(0);
    ASSERT(hash == NULL);
  }

  hash = test_hash_map_create(0);
  ASSERT(hash != NULL);
  ASSERT(test_hash_map_count(hash) == 0);
  ASSERT(hash->private.capacity > 0);
  ASSERT(hash->private.equals_func == &CCT_DEFAULT_STR_EQ);
  ASSERT(hash->private.hash_func == &CCT_DEFAULT_STR_HASH);
  ASSERT(hash->private.rehash_threshold > 0 && hash->private.rehash_threshold < 1);
  ASSERT(hash->private.rehash_threshold > 0 && hash->private.rehash_threshold < 1);
  ASSERT(hash->private.data != NULL);
  test_hash_map_destroy(hash, NULL, NULL);
  PASS();
}

int destroyed_keys = 0;

void my_key_destroyer(char* key) {
  free(key);
  destroyed_keys++;
}

int destroyed_data_sum = 0;

void my_data_destroyer(int data) {
  destroyed_data_sum += data;
}

TEST prefix_hash_map_destroy () {
  hash_t* hash = test_hash_map_create(0);
  char* key1 = malloc(sizeof(char) * 2);
  strcpy(key1, "a");
  char* key2 = malloc(sizeof(char) * 2);
  strcpy(key1, "b");
  test_hash_map_put(hash, key1, 1000);
  test_hash_map_put(hash, key2, 1);
  test_hash_map_destroy(hash, my_key_destroyer, my_data_destroyer);
  ASSERT(destroyed_keys == 2);
  ASSERT(destroyed_data_sum == 1001);
  destroyed_keys = 0;
  destroyed_data_sum = 0;
  PASS();
}

TEST prefix_hash_map_put () {
  size_t initial = 10;
  hash_t* hash = test_hash_map_create(initial);
  ASSERT(hash->private.capacity = initial);
  ASSERT(hash->private.rehash_threshold == .75);
  ASSERT(test_hash_map_put(hash, "1", 1) == true);
  ASSERT(test_hash_map_put(hash, "2", 2) == true);
  ASSERT(test_hash_map_put(hash, "3", 3) == true);
  ASSERT(test_hash_map_put(hash, "4", 4) == true);
  ASSERT(test_hash_map_put(hash, "5", 5) == true);
  ASSERT(test_hash_map_put(hash, "6", 6) == true);
  ASSERT(test_hash_map_put(hash, "7", 7) == true);
  ASSERT(hash->private.capacity == initial);
  ASSERT(test_hash_map_put(hash, "8", 8) == true);
  ASSERT(hash->private.capacity > initial);
  ASSERT(test_hash_map_put(hash, "9", 9) == true);
  ASSERT(test_hash_map_put(hash, "10", 10) == true);
  ASSERT(test_hash_map_count(hash) == 10);

  test_hash_map_destroy(hash, NULL, NULL);

  hash = test_hash_map_create(3);
  hash->private.rehash_threshold = .5; // will rehash after second insert
  ASSERT(test_hash_map_put(hash, "4", 3) == true);

  with_spoofed_oom {
    ASSERT(test_hash_map_put(hash, "4", 4) == true); // no rehash needed for existing key
    ASSERT(test_hash_map_put(hash, "5", 5) == false); // rehash needed and should fail
  }

  ASSERT(test_hash_map_put(hash, "6", 6) == true);
  ASSERT(test_hash_map_put(hash, "7", 7) == true);
  ASSERT(test_hash_map_get(hash, "4") == 4);
  ASSERT(test_hash_map_get(hash, "6") == 6);
  ASSERT(test_hash_map_get(hash, "7") == 7);
  test_hash_map_destroy(hash, NULL, NULL);
  PASS();
}

TEST prefix_hash_map_get () {
  hash_t* hash = test_hash_map_create(5);

  for (char i = 'a'; i < 'z'; i++) {
    char* s = malloc(sizeof(char) * 2);
    s[0] = i;
    s[1] = '\0';
    test_hash_map_put(hash, s, i);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    ASSERT(test_hash_map_get(hash, s) == i);
  }

  ASSERT(test_hash_map_get(hash, "not found") == (int)0);
  test_hash_map_destroy(hash, my_key_destroyer, NULL);

  PASS();
}

TEST prefix_hash_map_contains () {
  hash_t* hash = test_hash_map_create(5);

  for (char i = 'a'; i < 'z'; i++) {
    char* s = malloc(sizeof(char) * 2);
    s[0] = i;
    s[1] = '\0';
    test_hash_map_put(hash, s, i);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    ASSERT(test_hash_map_contains(hash, s) == true);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, i, '\0'};
    ASSERT(test_hash_map_contains(hash, s) == false);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    test_hash_map_remove(hash, s);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    ASSERT(test_hash_map_contains(hash, s) == false);
  }

  test_hash_map_destroy(hash, my_key_destroyer, NULL);

  PASS();
}

TEST prefix_hash_map_remove () {
  hash_t* hash = test_hash_map_create(5);

  for (char i = 'a'; i < 'z'; i++) {
    char* s = malloc(sizeof(char) * 2);
    s[0] = i;
    s[1] = '\0';
    test_hash_map_put(hash, s, i);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    ASSERT(test_hash_map_contains(hash, s) == true);
    hash_pair_t p = test_hash_map_remove(hash, s);
    ASSERT(!p.empty);
    ASSERT(strcmp(p.key, s) == 0);
    ASSERT(p.data == i);
    ASSERT(test_hash_map_contains(hash, s) == false);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    ASSERT(test_hash_map_contains(hash, s) == false);
  }

  for (char i = 'a'; i < 'z'; i++) {
    char s[] = {i, '\0'};
    hash_pair_t p = test_hash_map_remove(hash, s);
    ASSERT(p.empty);
  }

  PASS();
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
