#pragma once

#include "JsonNode.h"
#include "LinkedList.h"
#include "ParserStack.h"
#include <stdlib.h>

List_JValue *CreateStack();

void PushIntoStack(List_JValue *Stack, JValue *value);

JValue *PeekIntoStack(List_JValue *Stack);
JValue *PeekSecondIntoStack(List_JValue *Stack);
JValue *PopOfStack(List_JValue *Stack);
