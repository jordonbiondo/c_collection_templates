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
bool equals_fn(key_type a, key_type b);
#endif

// MACRO_DEFINE define_hash_map(key_type, data_type, prefix, hash_fn, equals_fn)

/* header */

struct prefix_hash_map;
struct prefix_hash_map* prefix_hash_map_create(size_t initial_capacity);
void prefix_hash_map_destroy(struct prefix_hash_map* map, void(*key_destroyer)(key_type), void(*data_destroyer)(data_type));
bool prefix_hash_map_put(struct prefix_hash_map* map, key_type key, data_type data);
data_type prefix_hash_map_get(struct prefix_hash_map* map, key_type key);
bool prefix_hash_map_contains(struct prefix_hash_map* map, key_type key);
struct prefix_hash_map_pair prefix_hash_map_remove(struct prefix_hash_map* map, key_type key);
/* bool prefix_hash_map_contains_data(struct prefix_hash_map* map, data_type data); */
/* key_type prefix_hash_map_key_of(struct prefix_hash_map* map, data_type data); */

bool prefix__private_hash_map_maybe_rehash(struct prefix_hash_map* map);
size_t prefix__private_hash_map_hash_index(struct prefix_hash_map* map, key_type key);
struct prefix_hash_map_pair* prefix__private_hash_map_pair_for(struct prefix_hash_map* map, key_type key);

/* end header */

/* implementation */

struct prefix_hash_map {
  struct prefix_hash_map_private_data {
    /* Hash function mapping a key to a semi-unique(ish) unsigned int.
     */
    unsigned int(*hash_func)(key_type);
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
    bool ok = prefix__private_hash_map_maybe_rehash(map);
    if (!ok) {
      /* couldn't rehash */
    }
  }

  /* TODO: should user be responsible for cleaning old data before overwrite?  */
  data_type old_data = pair->data;
  pair->data = data;
  pair->key = key;

  return false;
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

  pair->empty = true;
  pair->data = (data_type)NULL;
  pair->key = (key_type)NULL;

  return removed_pair;
}

/* bool prefix_hash_map_contains_data(struct prefix_hash_map* map, data_type data) { */

/*   /\* maybe not do this... *\/ */

/*   /\* for (size_t i = 0; i < map->private.capacity; i++) { *\/ */
/*   /\*   if ((!map->private.data[i].empty) && map->private.data[i].data == data) { *\/ */

/*   /\*   } *\/ */
/*   /\* } *\/ */
/*   data = data + 0; */
/*   map = map + 0; */
/*   return false; */
/* } */

/* key_type prefix_hash_map_key_of(struct prefix_hash_map* map, data_type data) { */

/*   /\* maybe not do this... *\/ */
/*   data = data + 0; */
/*   map = map + 0; */
/*   return NULL; */
/* } */

/* end implementation */

/* private */

bool prefix__private_hash_map_maybe_rehash(struct prefix_hash_map* map) {
  if (((double)map->private.population) / ((double)map->private.capacity) >= map->private.rehash_threshold) {
    /* do rehash here */
    return false;
  } else {
    return true;
  }
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
