#pragma once
#include "TokenType.h"
#include "logging.h"
#include <string.h>

enum TokenType {
  OPEN_OBJECT,
  CLOSE_OBJECT,
  OPEN_ARRAY,
  CLOSE_ARRAY,
  STRING_IDENTIFIER,
  OBJECT_SEPERATOR,
  KV_SEPERATOR,
  STRING_CHARACTER,
  NUMBER_CHARACTER,
  WHITESPACE,
  DOT
};

typedef struct ParseToken {
  enum TokenType tokenType;
  char character;
} ParseToken;

enum TokenType ParseCharacter(char character);

ParseToken *GetTokenByCharacter(char character);
