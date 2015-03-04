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

#include "lib/greatest.h"

GREATEST_MAIN_DEFS();

#include "linked_list_tests.h"
#include "dynamic_list_tests.h"
#include "circular_buffer_tests.h"
#include "hash_map_tests.h"

int main (int argc, char** argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(dynamic_list);
  RUN_SUITE(circular_buffer);
  RUN_SUITE(linked_list);
  RUN_SUITE(hash_map);
  GREATEST_MAIN_END();
}

