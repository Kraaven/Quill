#include "jsonFileValidation.h"
#include "logging.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>

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

  ParseJsonFile(fileName);
}
