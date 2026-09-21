#pragma once
#include <stdbool.h>
#include <stdio.h>
#define LOGGING true

#if LOGGING
#define LOG(...)                                                               \
  printf(__VA_ARGS__);                                                         \
  printf("\n");
#else
#define LOG(...)
#endif
