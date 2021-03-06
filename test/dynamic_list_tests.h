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

#ifndef DYNAMIC_LIST_TESTS_H
#define DYNAMIC_LIST_TESTS_H

#include "cct_test_common.h"

define_dyn_list(int, test)

typedef struct test_dyn_list tdl;

TEST dyn_list_create_test() {
  size_t initial_capacity = 20;
  tdl* list = test_dyn_list_create(initial_capacity);
  ASSERTm("The new list has size 0", list->size == 0);
  ASSERTm("The new list has capacity set", list->private.real_size == initial_capacity);
  ASSERTm("New list is not null.", list != NULL);
  test_dyn_list_destroy(list, NULL);

  with_spoofed_oom {
    tdl* list2 = test_dyn_list_create(0);
    ASSERTm("New list is null if allocation fails", list2 == NULL);
  }

  list = test_dyn_list_create(0);
  ASSERTm("The new list given 0 capacity uses a default", list->private.real_size > 0);
  test_dyn_list_destroy(list, NULL);

  PASS();
}

int destroy_test_sum = 0;
int destroy_test_count = 0;
void tdl_destroyer(int x) {
  destroy_test_count++;
  destroy_test_sum += x;
}

TEST dyn_list_destroy_test() {
  tdl* list = test_dyn_list_create(10);
  destroy_test_sum = 0;
  destroy_test_count = 0;

  test_dyn_list_add(list, 1);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 1000);
  test_dyn_list_destroy(list, tdl_destroyer);
  ASSERTm("destroyer method called for each element.", destroy_test_sum == 1111);
  ASSERTm("destroyer method called only for used allocated space.", destroy_test_count == 4);

  destroy_test_sum = 0;
  destroy_test_count = 0;
  PASS();
}

TEST dyn_list_length_test() {
  tdl* list = test_dyn_list_create(10);
  ASSERT(test_dyn_list_length(list) == 0);
  test_dyn_list_add(list, 1);
  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_length(list) == 2);
  test_dyn_list_insert(list, 1, 1);
  ASSERT(test_dyn_list_length(list) == 3);
  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_length(list) == 2);
  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_empty_test() {
  tdl* list = test_dyn_list_create(0);
  ASSERTm("a new list is empty.", test_dyn_list_empty(list) == true);
  test_dyn_list_add(list, 100);
  ASSERT(test_dyn_list_empty(list) == false);
  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_empty(list) == true);
  PASS();
}

TEST dyn_list_get_test() {
  tdl* list = test_dyn_list_create(0);
  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_get(list, 0) == 100);
  ASSERT(test_dyn_list_get(list, 1) == 10);
  ASSERT(test_dyn_list_get(list, 2) == 1);
  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_set_test() {
  tdl* list = test_dyn_list_create(0);
  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_get(list, 0) == 100);
  ASSERT(test_dyn_list_get(list, 1) == 10);
  ASSERT(test_dyn_list_get(list, 2) == 1);
  test_dyn_list_set(list, 0, 99);
  test_dyn_list_set(list, 1, 999);
  test_dyn_list_set(list, 2, 9999);
  ASSERT(test_dyn_list_get(list, 0) == 99);
  ASSERT(test_dyn_list_get(list, 1) == 999);
  ASSERT(test_dyn_list_get(list, 2) == 9999);
  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_add_test() {
  size_t start_capacity = 5;
  tdl* list = test_dyn_list_create(start_capacity);
  for (size_t i = 0; i < (start_capacity - 1); i++) {
    test_dyn_list_add(list, i);
  }

  ASSERT(test_dyn_list_length(list) == 4);
  ASSERT(list->size == (list->private.real_size - 1));
  ASSERTm("list does not grow before hitting capacity", list->private.real_size == start_capacity);

  test_dyn_list_add(list, 4);

  ASSERT(test_dyn_list_length(list) == 5);
  ASSERTm("The list has grown and there is more room now.", list->size < (list->private.real_size - 1));
  ASSERTm("The list grows beyond initial capacity once full.",
          list->private.real_size > start_capacity);

  ASSERT(test_dyn_list_get(list, 0) == 0);
  ASSERT(test_dyn_list_get(list, 1) == 1);
  ASSERT(test_dyn_list_get(list, 2) == 2);
  ASSERT(test_dyn_list_get(list, 3) == 3);
  ASSERT(test_dyn_list_get(list, 4) == 4);
  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_index_of_test() {
  tdl* list = test_dyn_list_create(0);

  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_index_of(list, 100) == 0);
  ASSERT(test_dyn_list_index_of(list, 10) == 1);
  ASSERT(test_dyn_list_index_of(list, 1) == 2);

  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_index_of(list, 1) == 2);

  test_dyn_list_set(list, 0, 99);
  test_dyn_list_set(list, 1, 999);
  test_dyn_list_set(list, 2, 9999);
  test_dyn_list_set(list, 3, 99999);

  ASSERT(test_dyn_list_index_of(list, 100) == -1);
  ASSERT(test_dyn_list_index_of(list, 10) == -1);
  ASSERT(test_dyn_list_index_of(list, 1) == -1);
  ASSERT(test_dyn_list_index_of(list, 99) == 0);
  ASSERT(test_dyn_list_index_of(list, 999) == 1);
  ASSERT(test_dyn_list_index_of(list, 9999) == 2);

  test_dyn_list_insert(list, 1, 9999);
  ASSERT(test_dyn_list_index_of(list, 9999) == 1);

  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_index_of(list, 99) == -1);

  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_contains_test() {
  tdl* list = test_dyn_list_create(0);

  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 1);
  ASSERT(test_dyn_list_contains(list, 100) == true);
  ASSERT(test_dyn_list_contains(list, 10) == true);
  ASSERT(test_dyn_list_contains(list, 1) == true);
  ASSERT(test_dyn_list_contains(list, 222) == false);

  test_dyn_list_set(list, 0, 99);
  test_dyn_list_set(list, 1, 999);
  test_dyn_list_set(list, 2, 9999);
  ASSERT(test_dyn_list_contains(list, 100) == false);
  ASSERT(test_dyn_list_contains(list, 10) == false);
  ASSERT(test_dyn_list_contains(list, 1) == false);
  ASSERT(test_dyn_list_contains(list, 99) == true);
  ASSERT(test_dyn_list_contains(list, 999) == true);
  ASSERT(test_dyn_list_contains(list, 9999) == true);

  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_contains(list, 99) == false);

  test_dyn_list_destroy(list, NULL);
  PASS();
}

bool mod_10_equals(int a, int b) {
  return (a % 10) == (b % 10);
}

TEST dyn_list_index_of_equal_test() {
  tdl* list = test_dyn_list_create(0);

  test_dyn_list_add(list, 19);
  test_dyn_list_add(list, 18);
  test_dyn_list_add(list, 17);
  test_dyn_list_add(list, 29);

  ASSERT(test_dyn_list_index_of_equal(list, 19, mod_10_equals) == 0);
  ASSERT(test_dyn_list_index_of_equal(list, 99, mod_10_equals) == 0);
  ASSERT(test_dyn_list_index_of_equal(list, 88, mod_10_equals) == 1);
  ASSERT(test_dyn_list_index_of_equal(list, 77, mod_10_equals) == 2);
  ASSERT(test_dyn_list_index_of_equal(list, 29, mod_10_equals) == 0);

  ASSERT(test_dyn_list_index_of_equal(list, 11, mod_10_equals) == -1);
  ASSERT(test_dyn_list_index_of_equal(list, 22, mod_10_equals) == -1);

  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_index_of_equal(list, 99, mod_10_equals) == 2);

  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_contains_equal_test() {
  tdl* list = test_dyn_list_create(0);

  test_dyn_list_add(list, 19);
  test_dyn_list_add(list, 18);
  test_dyn_list_add(list, 17);
  test_dyn_list_add(list, 29);

  ASSERT(test_dyn_list_contains_equal(list, 19, mod_10_equals) == true);
  ASSERT(test_dyn_list_contains_equal(list, 99, mod_10_equals) == true);
  ASSERT(test_dyn_list_contains_equal(list, 88, mod_10_equals) == true);
  ASSERT(test_dyn_list_contains_equal(list, 77, mod_10_equals) == true);
  ASSERT(test_dyn_list_contains_equal(list, 29, mod_10_equals) == true);

  ASSERT(test_dyn_list_contains_equal(list, 11, mod_10_equals) == false);
  ASSERT(test_dyn_list_contains_equal(list, 22, mod_10_equals) == false);

  test_dyn_list_remove(list, 0);
  ASSERT(test_dyn_list_contains_equal(list, 99, mod_10_equals) == true);

  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_remove_test() {
  tdl* list = test_dyn_list_create(0);
  test_dyn_list_add(list, 19);
  test_dyn_list_add(list, 18);
  test_dyn_list_add(list, 17);
  test_dyn_list_add(list, 16);
  ASSERT(test_dyn_list_length(list) == 4);
  ASSERT(test_dyn_list_remove(list, 0) == 19);
  ASSERT(test_dyn_list_remove(list, 0) == 18);
  ASSERT(test_dyn_list_remove(list, 0) == 17);
  ASSERT(test_dyn_list_remove(list, 0) == 16);
  ASSERT(test_dyn_list_length(list) == 0);
  test_dyn_list_destroy(list, NULL);
  PASS();
}

TEST dyn_list_clear_test () {
  tdl* list = test_dyn_list_create(10);
  destroy_test_sum = 0;
  destroy_test_count = 0;

  test_dyn_list_add(list, 1);
  test_dyn_list_add(list, 10);
  test_dyn_list_add(list, 100);
  test_dyn_list_add(list, 1000);
  test_dyn_list_clear(list, tdl_destroyer);
  ASSERTm("destroyer method called for each element.", destroy_test_sum == 1111);
  ASSERTm("destroyer method called only for used allocated space.", destroy_test_count == 4);
  ASSERT(test_dyn_list_length(list) == 0);
  ASSERT(test_dyn_list_add(list, 1) == true);
  destroy_test_sum = 0;
  destroy_test_count = 0;
  PASS();
}

TEST dyn_list_grow_test() {
  tdl* list = test_dyn_list_create(10);
  ASSERT(list->private.real_size == 10);
  test_dyn_list_grow(list, 10);
  ASSERT(list->private.real_size == 20);

  with_spoofed_oom {
    // can insert 10 without need to grow
    for (int i = 0; i < 10; i++) {
      test_dyn_list_add(list, i);
    }
  }
  ASSERT(list->size == 10);
  ASSERT(list->private.real_size == 20);

  test_dyn_list_destroy(list, NULL);
  PASS();
}

SUITE(dynamic_list) {
  RUN_TEST(dyn_list_create_test);
  RUN_TEST(dyn_list_destroy_test);
  RUN_TEST(dyn_list_length_test);
  RUN_TEST(dyn_list_empty_test);
  RUN_TEST(dyn_list_get_test);
  RUN_TEST(dyn_list_set_test);
  RUN_TEST(dyn_list_add_test);
  RUN_TEST(dyn_list_index_of_test);
  RUN_TEST(dyn_list_contains_test);
  RUN_TEST(dyn_list_index_of_equal_test);
  RUN_TEST(dyn_list_contains_equal_test);
  RUN_TEST(dyn_list_remove_test);
  RUN_TEST(dyn_list_clear_test);
  RUN_TEST(dyn_list_grow_test);
}

#endif
