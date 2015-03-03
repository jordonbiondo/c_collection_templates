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

#ifndef HASH_MAP_TEMPLATE_H
#define HASH_MAP_TEMPLATE_H

#ifndef CCT_COMMON_INCLUDES_H
#include "cct_common_includes.h"
#endif

#ifndef COLLECTION_TEMPLATES_COMPILED_HEADER
typedef char* key_type;
typedef int data_type;
unsigned int hash_fn(key_type key);
bool equals_fn(data_type a, data_type b);
#endif

// MACRO_DEFINE define_hash_map(key_type, data_type, prefix, hash_fn, equals_fn)

/* header */

struct prefix_hash_map;
struct prefix_hash_map* prefix_hash_map_create(size_t initial_capacity);
void prefix_hash_map_destroy(struct prefix_hash_map* map, void(*key_destroyer)(key_type), void(*data_destroyer)(data_type));
bool prefix_hash_map_put(struct prefix_hash_map* map, key_type key, data_type data);
data_type prefix_hash_map_get(struct prefix_hash_map* map, key_type key);
bool prefix_hash_map_contains(struct prefix_hash_map* map, key_type key);
data_type prefix_hash_map_remove(struct prefix_hash_map* map, key_type key);
bool prefix_hash_map_contains_data(struct prefix_hash_map* map, data_type data);
key_type prefix_hash_map_key_of(struct prefix_hash_map* map, data_type data);

/* end header */

/* implementation */

struct prefix_hash_map {
  struct prefix_hash_map_private_data {
    unsigned int(*hash_fn)(key_type);
    bool (*equals_fn)(data_type, data_type);
    size_t capacity;
    double rehash_threshold;
    struct prefix_hash_map_pair {
      bool empty;
      key_type key;
      data_type data;
    }* data;
  } private;
};

struct prefix_hash_map* prefix_hash_map_create(size_t initial_capacity) {
  struct prefix_hash_map* map = cct_alloc(struct prefix_hash_map, 1);
  map->private.capacity = (initial_capacity) ? initial_capacity : 75;
  map->private.hash_fn = hash_fn;
  map->private.rehash_threshold = .75;
  map->private.equals_fn = equals_fn;
  map->private.data = cct_alloc(struct prefix_hash_map_pair, map->private.capacity);
  for (size_t i = 0; i < map->private.capacity; i++) {
    map->private.data[i].empty = true;
  }
  return map;
}

void prefix_hash_map_destroy(struct prefix_hash_map* map, void(*key_destroyer)(key_type), void(*data_destroyer)(data_type)) {
  for (size_t i = 0; i < map->private.capacity; i++) {
    struct prefix_hash_map_pair node = map->private.data[i];
    if (!node.empty) {
      if (key_destroyer != NULL) {
        key_destroyer(node.key); /* maybe do this in define, need for remove too! or maybe return pair... */
      }
      if (data_destroyer != NULL) {
        data_destroyer(node.data);
      }
    }
  }
}

bool prefix_hash_map_put(struct prefix_hash_map* map, key_type key, data_type data) {
  size_t data_index = map->private.hash_fn(key) % map->private.capacity;
  size_t last_index = (data_index == 0) ? (map->private.capacity - 1) : data_index - 1;
  struct prefix_hash_map_pair pair = map->private.data[data_index];

  /* foobar ! */
  /* while (!pair.empty && data_index != last_index) { */
  /*   data_index = (data_index == (map->private.capacity - 1)) ? 0 : data_index + 1; */
  /*   pair = map->private.data[data_index]; */
  /* } */

  /* if (data_index == last_index) */

  return false;
}

data_type prefix_hash_map_get(struct prefix_hash_map* map, key_type key) {
  return 0;
}

bool prefix_hash_map_contains(struct prefix_hash_map* map, key_type key) {
  return false;
}

data_type prefix_hash_map_remove(struct prefix_hash_map* map, key_type key) {
  return 0;
}

bool prefix_hash_map_contains_data(struct prefix_hash_map* map, data_type data) {
  return false;
}

key_type prefix_hash_map_key_of(struct prefix_hash_map* map, data_type data) {
  return NULL;
}

/* end implementation */

/* private */

/* end private */

// MACRO_END

#endif
