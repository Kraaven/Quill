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
    strcpy(typeString, "Key Value Paid");
    break;
  }

  return typeString;
}
