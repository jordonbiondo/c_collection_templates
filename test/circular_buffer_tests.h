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

#ifndef CIRCULAR_BUFFER_TESTS_H
#define CIRCULAR_BUFFER_TESTS_H

#include "lib/greatest.h"
#include "cct_test_common.h"
#include "../collection_templates.h"

define_circular_buffer(int, test)

typedef struct test_cbuffer cbuffer;

TEST cbuffer_create_test() {
  size_t capacity = 10;
  int default_value = -1;

  cbuffer* buffer = test_cbuffer_create(capacity, default_value);
  ASSERT(buffer != NULL);
  ASSERT(buffer->values != NULL);
  ASSERT(buffer->capacity == capacity);
  ASSERT(buffer->default_value == default_value);
  ASSERT(buffer->population == 0);
  test_cbuffer_destroy(buffer, NULL);

  with_spoofed_oom {
    cbuffer* buffer2 = test_cbuffer_create(capacity, default_value);
    ASSERT(buffer2 == NULL);
  }

  PASS();
}

int destroyed_int_sum = 0;
int destroyed_int_count = 0;

void cbuffer_int_destroyer(int a) {
  destroyed_int_sum += a;
  destroyed_int_count++;
}

TEST cbuffer_destroy_test() {
  cbuffer* buffer = test_cbuffer_create(10, -1);
  test_cbuffer_enqueue(buffer, 100);
  test_cbuffer_enqueue(buffer, 10);
  test_cbuffer_enqueue(buffer, 1);
  test_cbuffer_destroy(buffer, cbuffer_int_destroyer);
  ASSERT(destroyed_int_count == 3);
  ASSERT(destroyed_int_sum == 111);
  PASS();
}
TEST cbuffer_empty_test() {
  SKIP();
}
TEST cbuffer_full_test() {
  SKIP();
}
TEST cbuffer_enqeue_test() {
  SKIP();
}
TEST cbuffer_pop_test() {
  SKIP();
}
TEST cbuffer_peek_test() {
  SKIP();
}
TEST cbuffer_capacity_test() {
  SKIP();
}

SUITE(circular_buffer) {
  RUN_TEST(cbuffer_create_test);
  RUN_TEST(cbuffer_destroy_test);
  RUN_TEST(cbuffer_empty_test);
  RUN_TEST(cbuffer_full_test);
  RUN_TEST(cbuffer_enqeue_test);
  RUN_TEST(cbuffer_pop_test);
  RUN_TEST(cbuffer_peek_test);
  RUN_TEST(cbuffer_capacity_test);
}

#endif
