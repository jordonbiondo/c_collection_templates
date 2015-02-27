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

#ifndef CIRCULAR_BUFFER_TEMPLATE_H
#define CIRCULAR_BUFFER_TEMPLATE_H

#ifndef CCT_COMMON_INCLUDES_H
#include "cct_common_includes.h"
#endif

// MACRO_DEFINE  define_circular_buffer(chosen_type, prefix)

/* header */

struct prefix_cb;
struct prefix_cb * prefix_cb_create(unsigned long capacity, dummy_type default_value);
void prefix_cb_destroy(struct prefix_cb* buffer, void(*element_destroyer)(dummy_type));
bool prefix_cb_isempty(struct prefix_cb* buffer);
bool prefix_cb_isfull(struct prefix_cb* buffer);
bool prefix_cb_put(struct prefix_cb* buffer, dummy_type value);
dummy_type prefix_cb_get(struct prefix_cb* buffer);
unsigned long prefix_cb_capacity(struct prefix_cb* buffer);

/* end header */

/* implementation */

struct prefix_cb {
  unsigned long capacity;
  dummy_type default_value;
  dummy_type* values;
  unsigned long put_position;
  unsigned long get_position;
  unsigned long population;
};

struct prefix_cb * prefix_cb_create(unsigned long capacity, dummy_type default_value) {
  struct prefix_cb* buffer = cct_alloc(struct prefix_cb, 1);
  buffer->capacity = capacity;
  buffer->default_value = default_value;
  buffer->values = cct_alloc(dummy_type, capacity);
  buffer->get_position = 0;
  buffer->put_position = 0;
  buffer->population = 0;
  return buffer;
}

void prefix_cb_destroy(struct prefix_cb * buffer, void(*element_destroyer)(dummy_type)) {
  if (element_destroyer) {
    for (unsigned long i = 0; i < buffer->capacity; i++) {
      element_destroyer(buffer->values[i]);
    }
  }
  free(buffer->values);
  free(buffer);
}

bool prefix_cb_isempty(struct prefix_cb * buffer) {
  return buffer->population == 0;
}

bool prefix_cb_isfull(struct prefix_cb * buffer) {
  return buffer->population == buffer->capacity;
}

bool prefix_cb_put(struct prefix_cb * buffer, dummy_type value) {
  if (prefix_cb_isfull(buffer)) {
    return false;
  } else {
    buffer->population++;
    buffer->values[buffer->put_position] = value;
    buffer->put_position++;
    if (buffer->put_position == buffer->capacity) {
      buffer->put_position = 0;
    }
    return true;
  }
}

dummy_type prefix_cb_get(struct prefix_cb * buffer) {
  if (prefix_cb_isempty(buffer)) {
    return buffer->default_value;
  } else {
    buffer->population--;
    dummy_type output = buffer->values[buffer->get_position];
    buffer->get_position++;
    if (buffer->get_position == buffer->capacity) {
      buffer->get_position = 0;
    }
    return output;
  }
}

unsigned long prefix_cb_capacity(struct prefix_cb * buffer) {
  return buffer->capacity;
}

/* end implementation */

/* private */

/* end private */

// MACRO_END

#endif
