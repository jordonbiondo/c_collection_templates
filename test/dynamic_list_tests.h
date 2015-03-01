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

#include "lib/greatest.h"
#include "cct_test_common.h"
#include "../collection_templates.h"

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
  SKIP();
}

TEST dyn_list_set_test() {
  SKIP();
}

TEST dyn_list_add_test() {
  SKIP();
}

TEST dyn_list_index_of_test() {
  SKIP();
}

TEST dyn_list_contains_test() {
  SKIP();
}

TEST dyn_list_index_of_equal_test() {
  SKIP();
}

TEST dyn_list_contains_equal_test() {
  SKIP();
}

TEST dyn_list_remove_test() {
  SKIP();
}

TEST dyn_list_grow_test() {
  SKIP();
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
  RUN_TEST(dyn_list_grow_test);

}

#endif