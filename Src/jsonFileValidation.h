#pragma once
#include "logging.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

bool ValidateJsonFile(char *fileName) {

  char *extensionCheck = strstr(fileName, ".json");
  if (extensionCheck == NULL) {
    LOG("%s is not a .json file", fileName)
    return false;
  }

  FILE *file = fopen(fileName, "rb");

  if (file == NULL) {
    LOG("%s file does not exist", fileName)
    return false;
  }

  struct stat sb;

  if (fstat(fileno(file), &sb) == -1) {
    LOG("File %s is empty", fileName)
    return false;
  }

  fclose(file);
  return true;
}
