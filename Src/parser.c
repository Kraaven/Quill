#include "parser.h"
#include "logging.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>

bool ParseJsonFile(char *FileName) {
  FILE *jsonFile = fopen(FileName, "rb");

  struct stat sb;
  fstat(fileno(jsonFile), &sb);
  int JsonFileSize = (int)sb.st_size;
  LOG("File size: %d", JsonFileSize);

  return true;
}
