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
#include "../collection_templates.h"

define_circular_buffer(int, test);

TEST cbuffer_create_test() {
  SKIP();
}
TEST cbuffer_destroy_test() {
  SKIP();
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
