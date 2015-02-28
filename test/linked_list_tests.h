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

#ifndef LINKED_LIST_TESTS_linked_list_H
#define LINKED_LIST_TESTS_linked_list_H

#include "lib/greatest.h"
#include "../collection_templates.h"

bool spoof_oom = false;

#ifdef cct_alloc
#undef cct_alloc
#define cct_alloc(type, count) ((spoof_oom) ? NULL : ((type*)(malloc(sizeof(type) * (count)))))
#endif

#define with_spoofed_oom for(spoof_oom = true; spoof_oom; spoof_oom = false)

define_linked_list(int, test)

typedef struct test_linked_list ill;

TEST linked_list_create_test () {
  ill* list = test_linked_list_create();
  ASSERTm("New list is not null.", list != NULL);
  with_spoofed_oom {
    ill* list2 = test_linked_list_create();
    ASSERTm("New list is null if allocation fails", list2 == NULL);
  }
  ASSERTm("New list has size 0", list->private.size == 0);
  test_linked_list_destroy(list, NULL);
  PASS();
}

int destroy_test_value = 0;
void my_int_destroyer(int x) {
  destroy_test_value += x;
}

TEST linked_list_destroy_test () {
  ill* list = test_linked_list_create();

  destroy_test_value = 0;
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 10);
  test_linked_list_push(list, 100);
  test_linked_list_push(list, 1000);
  test_linked_list_destroy(list, my_int_destroyer);
  ASSERTm("destroyer method called for each element.", destroy_test_value == 1111);
  destroy_test_value = 0;
  PASS();
}

TEST linked_list_length_test () {
  ill* list = test_linked_list_create();
  ASSERT(test_linked_list_length(list) == 0);
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 1);
  ASSERT(test_linked_list_length(list) == 2);
  test_linked_list_insert(list, 1, 1);
  ASSERT(test_linked_list_length(list) == 3);
  test_linked_list_remove(list, 0);
  ASSERT(test_linked_list_length(list) == 2);
  test_linked_list_destroy(list, my_int_destroyer);
  PASS();
}

TEST linked_list_empty_test () {
  ill* list = test_linked_list_create();
  ASSERT(test_linked_list_empty(list));
  test_linked_list_push(list, 1);
  ASSERT(!test_linked_list_empty(list));
  test_linked_list_push(list, 1);
  ASSERT(!test_linked_list_empty(list));
  test_linked_list_pop(list);
  ASSERT(!test_linked_list_empty(list));
  test_linked_list_pop(list);
  ASSERT(test_linked_list_empty(list));
  test_linked_list_destroy(list, my_int_destroyer);
  PASS();
}

TEST linked_list_get_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 10);
  test_linked_list_push(list, 100);
  ASSERT(test_linked_list_get(list, 0) == 100);
  ASSERT(test_linked_list_get(list, 1) == 10);
  ASSERT(test_linked_list_get(list, 2) == 1);
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_peek_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 10);
  test_linked_list_push(list, 100);
  ASSERT(test_linked_list_peek(list) == test_linked_list_get(list, 0));
  ASSERT(test_linked_list_peek(list) == 100);
  test_linked_list_pop(list);
  ASSERT(test_linked_list_peek(list) == test_linked_list_get(list, 0));
  ASSERT(test_linked_list_peek(list) == 10);
  test_linked_list_push(list, 99);
  ASSERT(test_linked_list_peek(list) == test_linked_list_get(list, 0));
  ASSERT(test_linked_list_peek(list) == 99);
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_pop_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 10);
  test_linked_list_push(list, 100);
  ASSERT(test_linked_list_pop(list) == 100);
  ASSERT(test_linked_list_pop(list) == 10);
  ASSERT(test_linked_list_pop(list) == 1);
  ASSERT(test_linked_list_empty(list));
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_set_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 1);
  test_linked_list_push(list, 2);
  test_linked_list_push(list, 3);
  ASSERT(test_linked_list_set(list, 0, 99) == 3);
  ASSERT(test_linked_list_set(list, 1, 88) == 2);
  ASSERT(test_linked_list_set(list, 2, 77) == 1);
  ASSERT(test_linked_list_get(list, 0) == 99);
  ASSERT(test_linked_list_get(list, 1) == 88);
  ASSERT(test_linked_list_get(list, 2) == 77);
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_insert_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 3);
  test_linked_list_push(list, 1);
  ASSERT(test_linked_list_insert(list, 1, 2) == true);
  ASSERT(test_linked_list_get(list, 0) == 1);
  ASSERT(test_linked_list_get(list, 1) == 2);
  ASSERT(test_linked_list_get(list, 2) == 3);

  with_spoofed_oom {
    ASSERT(test_linked_list_insert(list, 1, 2) == false);
  }

  ASSERT(test_linked_list_get(list, 0) == 1);
  ASSERT(test_linked_list_get(list, 1) == 2);
  ASSERT(test_linked_list_get(list, 2) == 3);
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_push_test () {
  ill* list = test_linked_list_create();
  ASSERT(test_linked_list_push(list, 3) == true);
  ASSERT(test_linked_list_push(list, 2) == true);
  ASSERT(test_linked_list_push(list, 1) == true);
  ASSERT(test_linked_list_get(list, 0) == 1);
  ASSERT(test_linked_list_get(list, 1) == 2);
  ASSERT(test_linked_list_get(list, 2) == 3);

  with_spoofed_oom {
    ASSERT(test_linked_list_insert(list, 1, 2) == false);
  }
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_append_test () {
  ill* list = test_linked_list_create();
  test_linked_list_push(list, 1);
  ASSERT(test_linked_list_append(list, 2) == true);
  ASSERT(test_linked_list_append(list, 3) == true);
  ASSERT(test_linked_list_get(list, 0) == 1);
  ASSERT(test_linked_list_get(list, 1) == 2);
  ASSERT(test_linked_list_get(list, 2) == 3);
  ASSERT(test_linked_list_length(list) == 3);

  with_spoofed_oom {
    ASSERT(test_linked_list_append(list, 100) == false);
  }
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_index_of_test () {
  ill* list = test_linked_list_create();

  test_linked_list_push(list, 100);
  test_linked_list_push(list, 1000);
  test_linked_list_push(list, 50);
  test_linked_list_push(list, 500);
  test_linked_list_push(list, 1);

  ASSERT((size_t)(test_linked_list_index_of(list, 100)) == (test_linked_list_length(list) - 1));
  ASSERT(test_linked_list_index_of(list, 1000) == 3);
  ASSERT(test_linked_list_index_of(list, 50) == 2);
  ASSERT(test_linked_list_index_of(list, 1) == 0);
  ASSERT(test_linked_list_index_of(list, 9999) < 0);
  test_linked_list_destroy(list, NULL);
  PASS();
}

bool evens_odds_equal(int a, int b) {
  return (a % 2) == (b % 2);
}

TEST linked_list_index_of_equal_test () {
  ill* list = test_linked_list_create();

  test_linked_list_push(list, 2);
  test_linked_list_push(list, 9);
  test_linked_list_push(list, 11);

  ASSERT(test_linked_list_index_of_equal(list, 4, evens_odds_equal) == 2);
  ASSERT(test_linked_list_index_of_equal(list, 99, evens_odds_equal) == 0);
  test_linked_list_remove(list, 2);
  ASSERT(test_linked_list_index_of_equal(list, 50, evens_odds_equal) < 0);
  test_linked_list_destroy(list, NULL);
  PASS();
}

TEST linked_list_remove_test () {
  ill* list = test_linked_list_create();

  test_linked_list_push(list, 10);
  test_linked_list_push(list, 5);
  test_linked_list_push(list, 1);

  ASSERT(test_linked_list_index_of(list, 1) == 0);
  ASSERT(test_linked_list_index_of(list, 10) == 2);
  test_linked_list_remove(list, 1);
  ASSERT(test_linked_list_index_of(list, 10) == 1);
  ASSERT(test_linked_list_index_of(list, 1) == 0);
  ASSERT(test_linked_list_index_of(list, 5) < 0);
  test_linked_list_destroy(list, NULL);
  PASS();
}

SUITE(linked_list) {
  RUN_TEST(linked_list_create_test);
  RUN_TEST(linked_list_destroy_test);
  RUN_TEST(linked_list_length_test);
  RUN_TEST(linked_list_empty_test);
  RUN_TEST(linked_list_get_test);
  RUN_TEST(linked_list_peek_test);
  RUN_TEST(linked_list_pop_test);
  RUN_TEST(linked_list_set_test);
  RUN_TEST(linked_list_insert_test);
  RUN_TEST(linked_list_push_test);
  RUN_TEST(linked_list_append_test);
  RUN_TEST(linked_list_index_of_test);
  RUN_TEST(linked_list_index_of_equal_test);
  RUN_TEST(linked_list_remove_test);
}

#endif
