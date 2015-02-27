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

struct prefix_cbuffer;
struct prefix_cbuffer * prefix_cbuffer_create(size_t capacity, dummy_type default_value);
void prefix_cbuffer_destroy(struct prefix_cbuffer* buffer, void(*element_destroyer)(dummy_type));
bool prefix_cbuffer_empty(struct prefix_cbuffer* buffer);
bool prefix_cbuffer_full(struct prefix_cbuffer* buffer);
bool prefix_cbuffer_enqeue(struct prefix_cbuffer* buffer, dummy_type value);
dummy_type prefix_cbuffer_pop(struct prefix_cbuffer* buffer);
dummy_type prefix_cbuffer_peek(struct prefix_cbuffer * buffer);
size_t prefix_cbuffer_capacity(struct prefix_cbuffer* buffer);

/* end header */

/* implementation */

struct prefix_cbuffer {
  size_t capacity;
  dummy_type default_value;
  dummy_type* values;
  size_t population;
  struct prefix_cbuffer_private_data {
    size_t put_position;
    size_t get_position;
  } private;
};

struct prefix_cbuffer * prefix_cbuffer_create(size_t capacity, dummy_type default_value) {
  struct prefix_cbuffer* buffer = cct_alloc(struct prefix_cbuffer, 1);
  buffer->capacity = capacity;
  buffer->default_value = default_value;
  buffer->values = cct_alloc(dummy_type, capacity);
  buffer->private.get_position = 0;
  buffer->private.put_position = 0;
  buffer->population = 0;
  return buffer;
}

void prefix_cbuffer_destroy(struct prefix_cbuffer * buffer, void(*element_destroyer)(dummy_type)) {
  if (element_destroyer) {
    for (size_t i = 0; i < buffer->capacity; i++) {
      element_destroyer(buffer->values[i]);
    }
  }
  free(buffer->values);
  free(buffer);
}

bool prefix_cbuffer_empty(struct prefix_cbuffer * buffer) {
  return buffer->population == 0;
}

bool prefix_cbuffer_full(struct prefix_cbuffer * buffer) {
  return buffer->population == buffer->capacity;
}

bool prefix_cbuffer_enqueue(struct prefix_cbuffer * buffer, dummy_type value) {
  if (prefix_cbuffer_full(buffer)) {
    return false;
  } else {
    buffer->population++;
    buffer->values[buffer->private.put_position] = value;
    buffer->private.put_position++;
    if (buffer->private.put_position == buffer->capacity) {
      buffer->private.put_position = 0;
    }
    return true;
  }
}

dummy_type prefix_cbuffer_pop(struct prefix_cbuffer * buffer) {
  if (prefix_cbuffer_empty(buffer)) {
    return buffer->default_value;
  } else {
    buffer->population--;
    dummy_type output = buffer->values[buffer->private.get_position];
    buffer->private.get_position++;
    if (buffer->private.get_position == buffer->capacity) {
      buffer->private.get_position = 0;
    }
    return output;
  }
}

dummy_type prefix_cbuffer_peek(struct prefix_cbuffer * buffer) {
  if (prefix_cbuffer_empty(buffer)) {
    return buffer->default_value;
  } else {
    return buffer->values[buffer->private.get_position];
  }
}

size_t prefix_cbuffer_capacity(struct prefix_cbuffer * buffer) {
  return buffer->capacity;
}

/* end implementation */

/* private */

/* end private */

// MACRO_END

#endif
