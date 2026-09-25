#pragma once
#include "ParserStack.h"
#include "JsonNode.h"
#include "jsonNode.h"
#include <stdlib.h>
#include <string.h>

List_JValue *CreateStack() {
  List_JValue *stack = malloc(sizeof(List_JValue));
  if (stack) {
    stack->head = NULL;
    stack->size = 0;
    return stack;
  }
  return NULL;
}

void PushIntoStack(List_JValue *Stack, JValue *value) {

  Node_JValue *newNode = malloc(sizeof(Node_JValue));
  newNode->value = value;
  newNode->next = NULL;

  if (Stack->head == NULL) {
    Stack->head = newNode;
    return;
  }

  newNode->next = Stack->head;
  Stack->head = newNode;
};

JValue *PeekIntoStack(List_JValue *Stack) {

  if (Stack->head == NULL) {
    return NULL;
  }
  return Stack->head->value;
}
JValue *PeekSecondIntoStack(List_JValue *Stack) {
  return Stack->head->next->value;
}
JValue *PopOfStack(List_JValue *Stack) {

  if (Stack->head == NULL) {
    return NULL;
  }

  Node_JValue *poppedNode = Stack->head;
  JValue *extractedData = poppedNode->value;
  Stack->head = Stack->head->next;

  free(poppedNode);
  return extractedData;
}

char *StackStateIntoString(List_JValue *Stack) {
  char *ParserState = malloc(sizeof(char) * 100);
  ParserState[0] = '\0';
  strcat(ParserState, "Parser Stack: ");
  Node_JValue *QueryPtr = Stack->head;

  while (QueryPtr != NULL) {
    char *NodeName = NodeTypeToString(QueryPtr->value->Jvalue_Type);
    strcat(ParserState, NodeName);
    strcat(ParserState, " <- ");
    free(NodeName);
    QueryPtr = QueryPtr->next;
  }

  return ParserState;
}
