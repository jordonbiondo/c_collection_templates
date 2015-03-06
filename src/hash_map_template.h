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
size_t hash_fn(key_type key);
bool equals_fn(key_type a, key_type b);
#endif

size_t CCT_DEFAULT_STR_HASH(char* str) {
  size_t val = 0;
  int offset = 0;
  while (*str) {
    size_t t = *str;
    val ^= (t << (offset * 8));
    offset++;
    if (offset == sizeof(size_t)) {
      offset = 0;
    }
    str++;
  }
  return val;
}

bool CCT_DEFAULT_STR_EQ(char* a, char* b) {
  return strcmp(a, b) == 0;
}

const double CCT_HASH_GROWTH_SCALE =  1.7;

/* Loop over every key value pair in map, It is NOT safe to modify the map in the body.
 */
#define hash_map_for_each(prefix, map_var, key_type, key_var, data_type, value_var) \
  for (size_t __hash_for_each_i = 0; __hash_for_each_i < map_var->private.capacity; __hash_for_each_i++) \
    for (struct prefix##_hash_map_pair pair = map_var->private.data[__hash_for_each_i]; !pair.empty; pair.empty = true) \
      for (key_type key_var = pair.key; key != (key_type)0; key = (key_type)0) \
        for (data_type value_var = pair.data; value != (data_type)0; value = (data_type)0)

// MACRO_DEFINE define_hash_map(key_type, data_type, prefix, hash_fn, equals_fn)

/* header */

struct prefix_hash_map;
struct prefix_hash_map* prefix_hash_map_create(size_t initial_capacity);
void prefix_hash_map_destroy(struct prefix_hash_map* map, void(*key_destroyer)(key_type), void(*data_destroyer)(data_type));
size_t prefix_hash_map_count(struct prefix_hash_map* map);
bool prefix_hash_map_put(struct prefix_hash_map* map, key_type key, data_type data);
data_type prefix_hash_map_get(struct prefix_hash_map* map, key_type key);
bool prefix_hash_map_contains(struct prefix_hash_map* map, key_type key);
struct prefix_hash_map_pair prefix_hash_map_remove(struct prefix_hash_map* map, key_type key);

bool prefix__private_hash_map_maybe_rehash(struct prefix_hash_map* map);
size_t prefix__private_hash_map_hash_index(struct prefix_hash_map* map, key_type key);
struct prefix_hash_map_pair* prefix__private_hash_map_pair_for(struct prefix_hash_map* map, key_type key);

/* end header */

/* implementation */

struct prefix_hash_map {
  struct prefix_hash_map_private_data {
    /* Hash function mapping a key to a semi-unique(ish) size_t .
     */
    size_t (*hash_func)(key_type);
    /* equals function that determines if a key is equal to another
     */
    bool (*equals_func)(key_type, key_type);

    /* The allocated capacity of pairs
     */
    size_t capacity;

    /* The number of non empty pairs
     */
    size_t population;

    /* If population/capcity is greather than this: re hash
     */
    double rehash_threshold;

    /* The allocated space for data
     */
    struct prefix_hash_map_pair {
      bool empty;
      key_type key;
      data_type data;
    }* data;
  } private;
};

struct prefix_hash_map* prefix_hash_map_create(size_t initial_capacity) {
  struct prefix_hash_map* map = cct_alloc(struct prefix_hash_map, 1);
  if (map == NULL) {
    return NULL;
  }
  map->private.capacity = (initial_capacity) ? initial_capacity : 75;
  map->private.population = 0;
  map->private.hash_func = hash_fn;
  map->private.rehash_threshold = .75;
  map->private.equals_func = equals_fn;
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
  free(map->private.data);
  free(map);
}

size_t prefix_hash_map_count(struct prefix_hash_map* map) {
  return map->private.population;
}

bool prefix_hash_map_put(struct prefix_hash_map* map, key_type key, data_type data) {
  struct prefix_hash_map_pair* pair = prefix__private_hash_map_pair_for(map, key);

  if (pair == NULL) {
    /* this should NEVER happen */
    printf("- Fatal Error: \n- defined hash must not have grown, it is full! Dying...\n");
    exit(-1);
  }

  if (pair->empty) {
    pair->empty = false;
    map->private.population++;
  }

  /* TODO: should user be responsible for cleaning old data before overwrite?  */
  /* data_type old_data = pair->data; */
  pair->data = data;
  pair->key = key;

  bool ok = prefix__private_hash_map_maybe_rehash(map);
  return ok;
}

data_type prefix_hash_map_get(struct prefix_hash_map* map, key_type key) {
  struct prefix_hash_map_pair* pair = prefix__private_hash_map_pair_for(map, key);
  return pair->data;
}

bool prefix_hash_map_contains(struct prefix_hash_map* map, key_type key) {
  struct prefix_hash_map_pair* pair = prefix__private_hash_map_pair_for(map, key);
  return !pair->empty;
}

struct prefix_hash_map_pair prefix_hash_map_remove(struct prefix_hash_map* map, key_type key) {
  struct prefix_hash_map_pair* pair = prefix__private_hash_map_pair_for(map, key);
  struct prefix_hash_map_pair removed_pair = *pair;
  struct prefix_hash_map_pair empty_pair = {.empty = true};
  *pair = empty_pair;

  return removed_pair;
}

/* end implementation */

/* private */

bool prefix__private_hash_map_maybe_rehash(struct prefix_hash_map* map) {
  if (((double)map->private.population) / ((double)map->private.capacity) < map->private.rehash_threshold) {
    return true;
  }

  size_t pre_rehash_population = map->private.population;
  struct prefix_hash_map_pair* pre_rehash_tenants = cct_alloc(struct prefix_hash_map_pair, pre_rehash_population);

  if (pre_rehash_tenants == NULL) {
    return false;
  }

  size_t pos = 0;
  for (size_t i = 0; i < map->private.capacity; i++) {
    if (!map->private.data[i].empty) {
      pre_rehash_tenants[pos++] = map->private.data[i];
    }
  }

  size_t new_capacity = map->private.capacity * CCT_HASH_GROWTH_SCALE;
  struct prefix_hash_map_pair* new_data = realloc(map->private.data, sizeof(struct prefix_hash_map_pair) * new_capacity);

  if (new_data == NULL) {
    free(pre_rehash_tenants);
    return false;
  }

  map->private.data = new_data;
  map->private.capacity = new_capacity;
  map->private.population = 0;

  struct prefix_hash_map_pair empty = {.empty = true};
  for (size_t i = 0; i < map->private.capacity; i++) {
    map->private.data[i] = empty;
  }

  for (size_t i = 0; i < pre_rehash_population; i++) {
    struct prefix_hash_map_pair* pair = prefix__private_hash_map_pair_for(map, pre_rehash_tenants[i].key);
    if (pair == NULL) {
      printf("- Fatal Error: \n- defined hash must not have grown, it is full! Dying...\n");
      exit(-1);
    }

    *pair = pre_rehash_tenants[i];
  }
  map->private.population = pre_rehash_population;
  free(pre_rehash_tenants);
  return true;
}

size_t prefix__private_hash_map_hash_index(struct prefix_hash_map* map, key_type key) {
  return map->private.hash_func(key) % map->private.capacity;
}

struct prefix_hash_map_pair* prefix__private_hash_map_pair_for(struct prefix_hash_map* map, key_type key) {
  size_t data_index = prefix__private_hash_map_hash_index(map, key);
  size_t start_index = data_index;
  struct prefix_hash_map_pair* pair = map->private.data + data_index;
  bool found = false;

  if (pair->empty || map->private.equals_func(key, pair->key)) {
    return pair;
  }

  while (!found) {
    data_index = (data_index == (map->private.capacity - 1)) ? 0 : data_index + 1;

    if (data_index == start_index) {
      return NULL;
    }

    pair = map->private.data + data_index;
    if (pair->empty || map->private.equals_func(key, pair->key)) {
      found = true;
    }
  }
  return pair;
}

/* end private */

// MACRO_END

#endif
