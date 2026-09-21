#include "parser.h"
#include "TokenStack.h"
#include "logging.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool ParseJsonFile(char *FileName) {
  FILE *jsonFile = fopen(FileName, "rb");

  struct stat sb;
  fstat(fileno(jsonFile), &sb);
  int JsonFileSize = (int)sb.st_size;
  LOG("File size: %d", JsonFileSize);

  char *JsonFileContent = malloc(JsonFileSize + 1);

  fgets(JsonFileContent, JsonFileSize + 1, jsonFile);

  TokenStack *jsonStack = CreateStack();
  for (int i = 0; i < JsonFileSize; i++) {
    LOG("Pasring : %c", JsonFileContent[i]);
    PushIntoStack(jsonStack, JsonFileContent[i]);
  }

  return true;
}
