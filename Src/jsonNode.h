#pragma once
#include "LinkedList.h"

LIST_DEFINATION(char)

typedef struct JValue JValue;
typedef struct KV KV;
LIST_DEFINATION(JValue)

enum JVALUE_TYPE { JOBJECT, ARRAY, STRING, FLOAT, KEY_VALUE };

typedef struct JValue {
  enum JVALUE_TYPE Jvalue_Type;
  union {
    struct JObject *object;
    List_JValue *array;
    List_char *stringBuilder;
    float number;
    char *string;
    KV *KeyValue;
  } data;
} JValue;

typedef struct KV {
  char *key;
  JValue *value;
} KV;

LIST_DEFINATION(KV)
typedef struct JObject {
  List_KV *KeyValues;
} JObject;

char *NodeTypeToString(enum JVALUE_TYPE type);
