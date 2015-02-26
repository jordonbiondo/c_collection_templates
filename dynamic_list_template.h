#ifndef DYNAMIC_LIST_TEMPLATE_H
#define DYNAMIC_LIST_TEMPLATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// MACRO_DEFINE define_dyn_list(chosen_type, prefix)

#ifndef DUMMY_TYPE
typedef float* dummy_type; // DUMMY_TYPE_DECLARATION
#else
typedef DUMMY_TYPE dummy_type;
#endif

/* header */

struct prefix_dyn_list;
struct prefix_dyn_list* prefix_dyn_list_create(unsigned long initial_capacity);
void prefix_dyn_list_destroy(struct prefix_dyn_list* list, void(*element_destroyer)(dummy_type));
unsigned long prefix_dyn_list_length(struct prefix_dyn_list* list);
unsigned long prefix_dyn_list_empty(struct prefix_dyn_list* list);
dummy_type prefix_dyn_list_get(struct prefix_dyn_list* list, unsigned long index);
dummy_type prefix_dyn_list_set(struct prefix_dyn_list* list, unsigned long index, dummy_type value);
dummy_type prefix_dyn_list_add(struct prefix_dyn_list* list, dummy_type value);
unsigned long prefix_dyn_list_index_of(struct prefix_dyn_list* list, dummy_type value);
unsigned long prefix_dyn_list_index_of_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type)));
dummy_type prefix_dyn_list_remove(struct prefix_dyn_list* list, unsigned long index);
dummy_type prefix_dyn_list_remove_at(struct prefix_dyn_list* list, unsigned long index);

void prefix_dyn_list_grow(struct prefix_dyn_list* list, unsigned int pre_allocated_size_increase);
void prefix__private_dyn_list_grow_internal(struct prefix_dyn_list* list, unsigned long new_size);
void prefix__private_dyn_list_grow(struct prefix_dyn_list* list);

/* end header */

/* implementation */

struct prefix_dyn_list {
  unsigned long size;
  dummy_type* data;
  struct {
    unsigned long real_size;
  } private;
};

struct prefix_dyn_list* prefix_dyn_list_create(unsigned long initial_capacity) {
  struct prefix_dyn_list* list = malloc(sizeof(struct prefix_dyn_list));
  if (!initial_capacity) {
    initial_capacity = 10;
  }
  list->private.real_size = initial_capacity;
  list->size = 0;
  list->data = malloc(sizeof(dummy_type) * initial_capacity);
  return list;
}

void prefix_dyn_list_destroy(struct prefix_dyn_list* list, void(*element_destroyer)(dummy_type)) {
  if (element_destroyer) {
    for (unsigned long i = 0; i < list->size; i++) {
      element_destroyer(list->data[i]);
    }
  }
  free(list->data);
  free(list);
}

unsigned long prefix_dyn_list_length(struct prefix_dyn_list* list) {
  return list->size;
}

unsigned long prefix_dyn_list_empty(struct prefix_dyn_list* list) {
  return list->size == 0;
}

dummy_type prefix_dyn_list_get(struct prefix_dyn_list* list, unsigned long index) {
  /* Actually access outside of the real arrays boundaries if index is not < list->size; */
  /* spoof a out of bounds error */
  return list->data[(index < list->size) ? index : list->private.real_size];
}

dummy_type prefix_dyn_list_set(struct prefix_dyn_list* list, unsigned long index, dummy_type value) {
  index = (index < list->size) ? index : list->private.real_size;
  return list->data[index] = value;
}

dummy_type prefix_dyn_list_add(struct prefix_dyn_list* list, dummy_type value) {
  list->data[list->size] = value;
  list->size++;
  if (list->size == list->private.real_size) {
    prefix__private_dyn_list_grow(list);
  }
  return value;
}


unsigned long prefix_dyn_list_index_of(struct prefix_dyn_list* list, dummy_type value) {
  for (unsigned long i = 0; i < list->size; i++) {
    if (list->data[i] == value) {
      return i;
    }
  }
  return -1;
}

unsigned long prefix_dyn_list_index_of_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type))) {
  for (unsigned long i = 0; i < list->size; i++) {
    if (equals(value, list->data[i])) {
      return i;
    }
  }
  return -1;
}

dummy_type prefix_dyn_list_remove(struct prefix_dyn_list* list, unsigned long index) {
  index = (index < list->size) ? index : list->private.real_size;
  dummy_type removed = list->data[index];
  if (index < (list->size - 1)) {
    memcpy(&list->data[index], &list->data[index+1], (sizeof(dummy_type) * (list->size - index - 1)));
  }
  list->size--;
  return removed;
}

void prefix_dyn_list_grow(struct prefix_dyn_list* list, unsigned int pre_allocated_size_increase) {
  unsigned long new_size = list->size + pre_allocated_size_increase;
  prefix__private_dyn_list_grow_internal(list, new_size);
}

/* end implementation */

/* private */

void prefix__private_dyn_list_grow_internal(struct prefix_dyn_list* list, unsigned long new_size) {
  dummy_type* new_data = realloc(list->data, sizeof(dummy_type) * new_size);
  list->data = new_data;
  list->private.real_size = new_size;
}

void prefix__private_dyn_list_grow(struct prefix_dyn_list* list) {
  unsigned long new_size = list->size * 1.7;
  prefix__private_dyn_list_grow_internal(list, new_size);
}

/* end private */

#endif
