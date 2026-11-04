#pragma once
#include "jsonNode.h"
#include <stdbool.h>

bool ParseJsonFile(char *FileName, JValue **rootJson);
