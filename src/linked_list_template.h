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

#ifndef LINKED_LIST_TEMPLATE_H
#define LINKED_LIST_TEMPLATE_H

#ifndef CCT_COMMON_INCLUDES_H
#include "cct_common_includes.h"
#endif

// MACRO_DEFINE define_linked_list(chosen_type, prefix)

/* header */

struct prefix_linked_list;
struct prefix_linked_list_node;
struct prefix_linked_list* prefix_linked_list_create();
void prefix_linked_list_destroy(struct prefix_linked_list* list, void(*element_destroyer)(dummy_type));
size_t prefix_linked_list_length(struct prefix_linked_list* list);
bool prefix_linked_list_empty(struct prefix_linked_list* list);
dummy_type prefix_linked_list_get(struct prefix_linked_list* list, size_t index);
dummy_type prefix_linked_list_peek(struct prefix_linked_list* list);
dummy_type prefix_linked_list_pop(struct prefix_linked_list* list);
dummy_type prefix_linked_list_set(struct prefix_linked_list* list, size_t index, dummy_type value);
bool prefix_linked_list_insert(struct prefix_linked_list* list, size_t index, dummy_type value);
bool prefix_linked_list_push(struct prefix_linked_list* list, dummy_type value);
bool prefix_linked_list_append(struct prefix_linked_list* list, dummy_type value);
long prefix_linked_list_index_of(struct prefix_linked_list* list, dummy_type value);
long prefix_linked_list_index_of_equal(struct prefix_linked_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type)));
dummy_type prefix_linked_list_remove(struct prefix_linked_list* list, size_t index);

struct prefix_linked_list_node* prefix__private_linked_list_node_create(dummy_type value);

/* end header */

/* implementation */

struct prefix_linked_list {
  struct prefix_linked_list_node* head;
  struct prefix_linked_list_private_data {
    size_t size;
  } private;
};

struct prefix_linked_list_node {
  dummy_type data;
  struct prefix_linked_list_node* next;
};

struct prefix_linked_list* prefix_linked_list_create() {
  struct prefix_linked_list* list = cct_alloc(struct prefix_linked_list, 1);
  if (list == NULL) {
    return NULL;
  }
  list->private.size = 0;
  list->head = NULL;
  return list;
}

void prefix_linked_list_destroy(struct prefix_linked_list* list, void(*element_destroyer)(dummy_type)) {
  for (struct prefix_linked_list_node* node = list->head; node != NULL;) {
    if (element_destroyer) {
      element_destroyer(node->data);
    }
    struct prefix_linked_list_node* next_node = node->next;
    free(node);
    node = next_node;
  }
  free(list);
}

size_t prefix_linked_list_length(struct prefix_linked_list* list) {
  return list->private.size;
}

bool prefix_linked_list_empty(struct prefix_linked_list* list) {
  return list->private.size == 0;
}

dummy_type prefix_linked_list_get(struct prefix_linked_list* list, size_t index) {
  struct prefix_linked_list_node* node = list->head;
  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }
  return node->data;
}

dummy_type prefix_linked_list_peek(struct prefix_linked_list* list) {
  struct prefix_linked_list_node* node = list->head;
  return node->data;
}

dummy_type prefix_linked_list_pop(struct prefix_linked_list* list) {
  return prefix_linked_list_remove(list, 0);
}

dummy_type prefix_linked_list_set(struct prefix_linked_list* list, size_t index, dummy_type value) {
  struct prefix_linked_list_node* node = list->head;
  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }
  dummy_type previous_value = node->data;
  node->data = value;
  return previous_value;
}

bool prefix_linked_list_insert(struct prefix_linked_list* list, size_t index, dummy_type value) {
  struct prefix_linked_list_node* new_node = prefix__private_linked_list_node_create(value);
  if (new_node == NULL) {
    return false;
  }

  struct prefix_linked_list_node** parent_pointer;

  if (index == 0) {
    parent_pointer = &list->head;
  } else {
    struct prefix_linked_list_node* parent = list->head;
    for (size_t i = 0; i < (index - 1); i++) {
      parent = parent->next;
    }
    parent_pointer = &parent->next;
  }

  struct prefix_linked_list_node* previous_next_node = *parent_pointer;
  *parent_pointer = new_node;
  new_node->next = previous_next_node;
  list->private.size++;
  return true;
}


bool prefix_linked_list_push(struct prefix_linked_list* list, dummy_type value) {
  return prefix_linked_list_insert(list, 0, value);
}

bool prefix_linked_list_append(struct prefix_linked_list* list, dummy_type value) {
  return prefix_linked_list_insert(list, list->private.size, value);
}


long prefix_linked_list_index_of(struct prefix_linked_list* list, dummy_type value) {
  long i = 0;
  for (struct prefix_linked_list_node* node = list->head; node != NULL; node = node->next) {
    if (node->data == value) {
      return i;
    }
    i++;
  }
  return -1;
}

long prefix_linked_list_index_of_equal(struct prefix_linked_list* list, dummy_type value, bool(*equals(dummy_type, dummy_type))) {
  long i = 0;
  for (struct prefix_linked_list_node* node = list->head; node != NULL; node = node->next) {
    if (equals(value, node->data)) {
      return i;
    }
    i++;
  }
  return -1;
}

dummy_type prefix_linked_list_remove(struct prefix_linked_list* list, size_t index) {
  struct prefix_linked_list_node** parent_pointer;
  if (index == 0) {
    parent_pointer = &list->head;
  } else {
    struct prefix_linked_list_node* parent = list->head;
    for (size_t i = 0; i < (index - 1); i++) {
      parent = parent->next;
    }
    parent_pointer = &parent->next;
  }

  struct prefix_linked_list_node* new_child = (*parent_pointer)->next;
  dummy_type removed = (*parent_pointer)->data;
  free(*parent_pointer);
  *parent_pointer = new_child;
  list->private.size--;
  return removed;
}

/* end implementation */

/* private */

struct prefix_linked_list_node* prefix__private_linked_list_node_create(dummy_type value) {
  struct prefix_linked_list_node* new_node = cct_alloc(struct prefix_linked_list_node, 1);
  if (new_node != NULL) {
    new_node->data = value;
    new_node->next = NULL;
  }
  return new_node;
}

/* end private */

// MACRO_END

#endif
