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
#include "../collection_templates.h"

define_dyn_list(int, test);

TEST dyn_list_create_test() {
  SKIP();
}

TEST dyn_list_destroy_test() {
  SKIP();
}

TEST dyn_list_length_test() {
  SKIP();
}

TEST dyn_list_empty_test() {
  SKIP();
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
