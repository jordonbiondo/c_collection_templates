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

#ifndef DYNAMIC_LIST_TEMPLATE_H
#define DYNAMIC_LIST_TEMPLATE_H

#ifndef CCT_COMMON_INCLUDES_H
#include "cct_common_includes.h"
#endif

// MACRO_DEFINE define_dyn_list(chosen_type, prefix)

/* header */

struct prefix_dyn_list;
struct prefix_dyn_list* prefix_dyn_list_create(size_t initial_capacity);
void prefix_dyn_list_destroy(struct prefix_dyn_list* list, void(*element_destroyer)(dummy_type));
long prefix_dyn_list_length(struct prefix_dyn_list* list);
bool prefix_dyn_list_empty(struct prefix_dyn_list* list);
dummy_type prefix_dyn_list_get(struct prefix_dyn_list* list, unsigned index);
dummy_type prefix_dyn_list_set(struct prefix_dyn_list* list, unsigned index, dummy_type value);
bool prefix_dyn_list_add(struct prefix_dyn_list* list, dummy_type value);

bool prefix_dyn_list_insert(struct prefix_dyn_list* list, unsigned index, dummy_type value);
bool prefix_dyn_list_push_front(struct prefix_dyn_list* list, dummy_type value);
bool prefix_dyn_list_push_back(struct prefix_dyn_list* list, dummy_type value);
dummy_type prefix_dyn_list_pop_front(struct prefix_dyn_list* list);
dummy_type prefix_dyn_list_pop_back(struct prefix_dyn_list* list);
dummy_type prefix_dyn_list_peek_back(struct prefix_dyn_list* list);
dummy_type prefix_dyn_list_peek_front(struct prefix_dyn_list* list);

long prefix_dyn_list_index_of(struct prefix_dyn_list* list, dummy_type value);
bool prefix_dyn_list_contains(struct prefix_dyn_list* list, dummy_type value);
long prefix_dyn_list_index_of_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type)));
bool prefix_dyn_list_contains_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type)));
dummy_type prefix_dyn_list_remove(struct prefix_dyn_list* list, unsigned index);
bool prefix_dyn_list_grow(struct prefix_dyn_list* list, size_t pre_allocated_size_increase);

bool prefix__private_dyn_list_grow_internal(struct prefix_dyn_list* list, size_t new_size);
bool prefix__private_dyn_list_grow(struct prefix_dyn_list* list);

/* end header */

/* implementation */

/* Struct for a dynamic list
 * @size foobar
 */
struct prefix_dyn_list {
  size_t size;
  dummy_type* data;
  struct prefix_dyn_list_private_data {
    size_t real_size;
  } private;
};

/* Create a new dynamic list for this type.
 * @initial_capacity Initial allocated capacity, pass zero to use the default value.
 *
 * @return a pointer to a new dynamic list or NULL if allocation fails.
 */
struct prefix_dyn_list* prefix_dyn_list_create(size_t initial_capacity) {
  struct prefix_dyn_list* list = cct_alloc(struct prefix_dyn_list, 1);
  if (list == NULL) {
    return NULL;
  }
  if (!initial_capacity) {
    initial_capacity = 10;
  }
  list->private.real_size = initial_capacity;
  list->size = 0;
  list->data = cct_alloc(dummy_type, initial_capacity);
  if (list->data == NULL) {
    return NULL;
  }
  return list;
}

/* Free a dynamic list and all of its data.
 * @list The dynamic list to free.
 * @element_destroyer If not NULL, each element of list's data will be passed
 * through this function which you can use free or otherwise cleanup each
 * individual element before its reference is lost.
 */
void prefix_dyn_list_destroy(struct prefix_dyn_list* list, void(*element_destroyer)(dummy_type)) {
  if (element_destroyer) {
    for (size_t i = 0; i < list->size; i++) {
      element_destroyer(list->data[i]);
    }
  }
  free(list->data);
  free(list);
}

/* The number of elements in list.
 *
 * @return the number of elements in list
 */
long prefix_dyn_list_length(struct prefix_dyn_list* list) {
  return list->size;
}

/* Get whether or not a list is empty.
 *
 * @return true if the list is empty else false.
 */
bool prefix_dyn_list_empty(struct prefix_dyn_list* list) {
  return list->size == 0;
}

/* Get the value of list at a given index.
 * @index the index at which to retrieve the stored value in list
 *
 * Get the value of list at a given index.
 * Does no bound checking, it is your responsibility to ensure index is
 * within the bounds of the list. See also: <prefix_dyn_list_length>
 *
 * @return the value of list at index
 */
dummy_type prefix_dyn_list_get(struct prefix_dyn_list* list, unsigned index) {
  return list->data[index];
}

/* Replace the value of list at a given index with a new value;
 * @index the index at which to retrieve the stored value in list
 * @value the value to place at index
 *
 * Replace the value of list at a given index.
 * Does no bound checking, it is your responsibility to ensure index is
 * within the bounds of the list. See also: <prefix_dyn_list_length>
 *
 * @return the previous value at index that has been replaced.
 */
dummy_type prefix_dyn_list_set(struct prefix_dyn_list* list, unsigned index, dummy_type value) {
  dummy_type previous_value = list->data[index];
  list->data[index] = value;
  return previous_value;
}

/* Append a value to a dynamic list.
 * @value the value to append to the list
 *
 * Append a value to a dynamic list.
 * If the list has reached capacity it reallocate itself with more space.
 *
 * @return true if value was added successfully, or false if
 * allocation was necessary and failed.
 */
bool prefix_dyn_list_add(struct prefix_dyn_list* list, dummy_type value) {
  list->data[list->size] = value;
  list->size++;
  if (list->size == list->private.real_size) {
    return prefix__private_dyn_list_grow(list);
  }
  return true;
}

bool prefix_dyn_list_insert(struct prefix_dyn_list* list, unsigned index, dummy_type value) {
  if (index != (list->size - 1)) {
    memmove(&list->data[index+1], &list->data[index], (sizeof(dummy_type) * (list->size - index)));
  }

  list->data[index] = value;
  list->size++;

  if (list->size == list->private.real_size) {
    return prefix__private_dyn_list_grow(list);
  }

  return true;
}

bool prefix_dyn_list_push_front(struct prefix_dyn_list* list, dummy_type value) {
  return prefix_dyn_list_insert(list, 0, value);
}

/* Push a value onto the back of the dynamic list
 * @value the value to append to the list
 *
 * Push a value onto the back of the dynamic list
 * This is just a wrapper for <prefix_dyn_list_add>
 */
bool prefix_dyn_list_push_back(struct prefix_dyn_list* list, dummy_type value) {
  return prefix_dyn_list_add(list, value);
}

dummy_type prefix_dyn_list_pop_front(struct prefix_dyn_list* list) {
  return prefix_dyn_list_remove(list, 0);
}

dummy_type prefix_dyn_list_pop_back(struct prefix_dyn_list* list) {
  return prefix_dyn_list_remove(list, list->size - 1);
}

dummy_type prefix_dyn_list_peek_back(struct prefix_dyn_list* list) {
  return prefix_dyn_list_get(list, list->size - 1);
}

dummy_type prefix_dyn_list_peek_front(struct prefix_dyn_list* list) {
  return prefix_dyn_list_get(list, 0);
}

/* Return the index of the first element in a list == to a given value
 * @value the value to find in the list
 *
 * Return the index of the first element in a list that is == to a given value
 * If == is not sufficient to check for equality use <prefix_dyn_list_index_of_equal>
 *
 * @return the first index of an element == to value in list or -1 if value is not found.
 */
long prefix_dyn_list_index_of(struct prefix_dyn_list* list, dummy_type value) {
  for (size_t i = 0; i < list->size; i++) {
    if (list->data[i] == value) {
      return i;
    }
  }
  return -1;
}

/* Find out if a list contains an element == to a given value.
 * @value the value to look for in the list
 *
 * Find out if a list contains an element == to a given value.
 * If == is not sufficient to check for equality use <prefix_dyn_list_contains_equal>
 *
 * @return true if list contains an element == to the given value
 */
bool prefix_dyn_list_contains(struct prefix_dyn_list* list, dummy_type value) {
  return prefix_dyn_list_index_of(list, value) > 0;
}

/* Return the index of the first element in a list where a equallity function returns true.
 * @value the value to find in the list
 * @equals a equallity checking function used to check if a value in the list is equal
 * to the given value
 *
 * Return the index of the first element in a list where a equallity function returns true.
 * The function will be called with the given value and each element of the list in order
 * until it returns true to signify the values are equal.
 * If == will be  sufficient to check for equality use <prefix_dyn_list_index_of>
 *
 * @return the first index of an element == to value in list or -1 if value is not found.
 */
long prefix_dyn_list_index_of_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type))) {
  for (size_t i = 0; i < list->size; i++) {
    if (equals(value, list->data[i])) {
      return i;
    }
  }
  return -1;
}

/* Find out if a list contains an element equal to a given value using a equallity checking function.
 * @value the value to find in the list
 *
 * Find out if a list contains an element such that equals(value, element) returns true
 * If == is sufficient to check for equality use <prefix_dyn_list_contains>
 *
 * @return true if list contains an element such that equals(value, element) returns true.
 */
bool prefix_dyn_list_contains_equal(struct prefix_dyn_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type))) {
  return prefix_dyn_list_index_of_equal(list, value, equals) > 0;
}


/* Remove the element of list at an index.
 * @index the index of the element to remove from the list
 *
 * Remove the element of list at an index.
 * Does no bound checking, it is your responsibility to ensure index is
 * within the bounds of the list. See also: <prefix_dyn_list_length>
 * All elements to the right of index are shifted left to fill the whole;
 *
 * @return the value removed from the index
 */
dummy_type prefix_dyn_list_remove(struct prefix_dyn_list* list, unsigned index) {
  dummy_type removed = list->data[index];
  if (index < (list->size - 1)) {
    memmove(&list->data[index], &list->data[index+1], (sizeof(dummy_type) * (list->size - index - 1)));
  }
  list->size--;
  return removed;
}

/* Manually increase a lists allocated capacity.
 * @pre_allocated_size_increase the number of elements to allocate additionally
 * memory for.
 *
 * Manually increase a lists allocated capacity.
 * Typically a lists capacity will automatically grow by 60% when it becomes full
 * but this can be usefull if you know you are going to be adding large amounts
 * of data and want to minimumize reallocations.
 *
 * @return true if allocation succeeds, false if not
 */
bool prefix_dyn_list_grow(struct prefix_dyn_list* list, size_t pre_allocated_size_increase) {
  size_t new_size = list->size + pre_allocated_size_increase;
  return prefix__private_dyn_list_grow_internal(list, new_size);
}

/* end implementation */

/* private */

/* Internal list growing method, do not use.
 */
bool prefix__private_dyn_list_grow_internal(struct prefix_dyn_list* list, size_t new_size) {
  dummy_type* new_data = realloc(list->data, sizeof(dummy_type) * new_size);
  if (new_data == NULL) {
    return false;
  } else {
    list->data = new_data;
    list->private.real_size = new_size;
    return true;
  }
}

/* Internal list reallocation method, grows a lists capacity by a factor
 */
bool prefix__private_dyn_list_grow(struct prefix_dyn_list* list) {
  size_t new_size = list->size * 1.6;
  return prefix__private_dyn_list_grow_internal(list, new_size);
}

/* end private */

// MACRO_END

#endif
