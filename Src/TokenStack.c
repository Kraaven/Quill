#pragma once
#include "TokenStack.h"
#include "TokenType.h"
#include <stdlib.h>

TokenStack *CreateStack() { return malloc(sizeof(TokenStack)); }

void PushIntoStack(TokenStack *Stack, char character) {

  TokenNode *node = malloc(sizeof(TokenNode));
  node->token = GetTokenByCharacter(character);
  if (Stack->StackTop == NULL) {
    Stack->StackTop = node;
  } else {
    TokenNode *head = Stack->StackTop;
    while (head->next != NULL) {
      head = head->next;
    }
    head->next = node;
    Stack->StackLength++;
  }
}

ParseToken *PeekIntoStack(TokenStack *stack) { return stack->StackTop->token; }
ParseToken *PeekSecondIntoStack(TokenStack *stack) {
  return stack->StackTop->next->token;
}
ParseToken *PopOfStack(TokenStack *stack) {
  TokenNode *popped = stack->StackTop;
  ParseToken *token = stack->StackTop->token;
  if (stack->StackLength > 1) {
    stack->StackTop = stack->StackTop->next;
    stack->StackLength--;
  } else {
    stack->StackTop = NULL;
  }

  free(popped);
  return token;
}
