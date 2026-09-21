#pragma once

#include "TokenType.h"
#include <stdlib.h>
typedef struct TokenNode {
  ParseToken *token;
  struct TokenNode *next;
} TokenNode;

typedef struct TokenStack {
  TokenNode *StackTop;
  int StackLength;
} TokenStack;

TokenStack *CreateStack();

void PushIntoStack(TokenStack *Stack, char character);
ParseToken *PeekIntoStack(TokenStack *stack);
ParseToken *PeekSecondIntoStack(TokenStack *stack);
ParseToken *PopOfStack(TokenStack *stack);
