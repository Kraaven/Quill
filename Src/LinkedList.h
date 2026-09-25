#pragma once

#include <stdlib.h>

#define CONCAT(a, b) a##_##b

#define LIST_DEFINATION(type)                                                  \
                                                                               \
  typedef struct CONCAT(Node, type) {                                          \
    type *value;                                                               \
    struct CONCAT(Node, type) * next;                                          \
  } CONCAT(Node, type);                                                        \
                                                                               \
  typedef struct CONCAT(List, type) {                                          \
    struct CONCAT(Node, type) * head;                                          \
    size_t size;                                                               \
  } CONCAT(List, type);                                                        \
                                                                               \
  static inline CONCAT(List, type) * CONCAT(List_Create, type)() {             \
    CONCAT(List, type) *list =                                                 \
        (CONCAT(List, type) *)malloc(sizeof(CONCAT(List, type)));              \
    list->head = NULL;                                                         \
    list->size = 0;                                                            \
    return list;                                                               \
  }                                                                            \
                                                                               \
  static inline void CONCAT(List_Insert, type)(CONCAT(List, type) * list,      \
                                               type * value) {                 \
    CONCAT(Node, type) *NewNode =                                              \
        (CONCAT(Node, type) *)malloc(sizeof(CONCAT(Node, type)));              \
    NewNode->value = value;                                                    \
    NewNode->next = NULL;                                                      \
                                                                               \
    if (list->head == NULL) {                                                  \
      list->head = NewNode;                                                    \
      list->size++;                                                            \
      return;                                                                  \
    }                                                                          \
    CONCAT(Node, type) *NodePtr = list->head;                                  \
    while (NodePtr->next != NULL) {                                            \
      NodePtr = NodePtr->next;                                                 \
    }                                                                          \
    NodePtr->next = NewNode;                                                   \
    list->size++;                                                              \
    return;                                                                    \
  }
