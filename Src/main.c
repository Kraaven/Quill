#include "JsonNode.h"
#include "jsonFileValidation.h"
#include "jsonNode.h"
#include "logging.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  char *fileName = argv[1];

  fileName = "hello.json";
  /*

  LOG("Executing Quill")

  if (argc < 2) {

    LOG("No Json File Provided")
    return 0;
  }

  if (argc > 2) {

    LOG("Too many arguments provided")
    return 0;
  }

  */

  if (!ValidateJsonFile(fileName)) {
    LOG("File Argument not a Json File. Parsing Aborted")
    return 0;
  } else {
    LOG("Json File found")
  }

  JValue *Json = NULL;
  if (!ParseJsonFile(fileName, &Json)) {
    LOG("Failed to Parse Json");
  }
  /*
    LOG("\nTEST: Json\n1st KeyPair, Key: %s Value: %s\n2nd KeyPair, Key: %s "
        "Value: "
        "%f",
        Json->data.object->KeyValues->head->value->key,
        Json->data.object->KeyValues->head->value->value->data.string,
        Json->data.object->KeyValues->head->next->value->key,
        Json->data.object->KeyValues->head->next->value->value->data.number);
    */
}
