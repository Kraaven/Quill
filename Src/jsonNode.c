#include "jsonNode.h"
#include <stdlib.h>
#include <string.h>

char *NodeTypeToString(enum JVALUE_TYPE type) {
  char *typeString = malloc(sizeof(char) * 15);

  switch (type) {
  case JOBJECT:
    strcpy(typeString, "Json Object");
    break;
  case ARRAY:
    strcpy(typeString, "Array");
    break;
  case FLOAT:
    strcpy(typeString, "Number");
    break;
  case STRING:
    strcpy(typeString, "String");
    break;
  case KEY_VALUE:
    strcpy(typeString, "Key Value Pair");
    break;
  }

  return typeString;
}

bool ValidateJState(JValue *value) {

  // true, false, null

  switch (value->data.stringBuilder->size) {
  case 4:
    char trueValue[4] = "true";
    char nullValue[4] = "null";
    bool matchState = true;

    Node_char *stringCharPtr = value->data.stringBuilder->head;
    for (int i = 0; i < 4; i++) {
      if (*(stringCharPtr->value) != trueValue[i]) {
        matchState = false;
      } else {
        stringCharPtr = stringCharPtr->next;
      }
    }

    if (matchState == false) {
      return false;
    }

    break;
  case 5:
    break;
  default:
    if (value->data.stringBuilder->size < 5) {
      return true;
    }
    if (value->data.stringBuilder->size > 5) {
      return false;
    }
  }
}
