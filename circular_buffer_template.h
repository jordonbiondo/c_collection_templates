#ifndef CIRCULAR_BUFFER_TEMPLATE_H
#define CIRCULAR_BUFFER_TEMPLATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// MACRO_DEFINE  define_circular_buffer(dummy_type, prefix, element_destroyer)

#ifndef DUMMY_TYPE
typedef float* dummy_type; // DUMMY_TYPE_DECLARATION
#else
typedef DUMMY_TYPE dummy_type;
#endif

struct prefixcb * prefixcb_create(unsigned long capacity, dummy_type default_value);
void prefixcb_destroy(struct prefixcb *);
bool prefixcb_isempty(struct prefixcb *);
bool prefixcb_isfull(struct prefixcb *);
bool prefixcb_put(struct prefixcb *, dummy_type);
dummy_type prefixcb_get(struct prefixcb *);
unsigned long prefixcb_capacity(struct prefixcb *);

struct prefix_cb {
  unsigned long capacity;
  dummy_type default_value;
  dummy_type* values;
  unsigned long put_position;
  unsigned long get_position;
  unsigned long population;
};

struct prefix_cb * prefix_cb_create(unsigned long capacity, dummy_type default_value) {
  struct prefix_cb* buffer = malloc(sizeof(struct prefix_cb));
  buffer->capacity = capacity;
  buffer->default_value = default_value;
  buffer->values = malloc(sizeof(dummy_type) * capacity);
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


#endif
