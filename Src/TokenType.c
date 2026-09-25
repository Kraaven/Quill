#pragma once
#include "TokenType.h"
#include "logging.h"
#include <stdlib.h>
#include <string.h>

char NumberSet[] = "1234567890";
char AlphabetSet[] = "abcdefghijklmniopqrstuvqxyzABCDEFGHIJKLMNOPQRSTUVWQYZ";

enum TokenType ParseCharacter(char character) {
  switch (character) {
  case '{':
    return OPEN_OBJECT;
    break;
  case '}':
    return CLOSE_OBJECT;
    break;
  case '[':
    return OPEN_ARRAY;
    break;
  case ']':
    return CLOSE_ARRAY;
    break;
  case '\"':
    return STRING_IDENTIFIER;
    break;
  case ',':
    return VALUE_SEPERATOR;
    break;
  case ':':
    return KV_SEPERATOR;
    break;
  case ' ':
    return WHITESPACE;
    break;
  case '\n':
    return WHITESPACE;
    break;
  case '\t':
    return WHITESPACE;
    break;
  case '.':
    return DOT;
    break;
  case '\r':
    return WHITESPACE;
  }

  if (strchr(NumberSet, character) == NULL) {
    if (strchr(AlphabetSet, character) == NULL) {
      LOG("Unknown Cladified Character %c : %d", character, character);
    } else {
      return STRING_CHARACTER;
    }
  } else {
    return NUMBER_CHARACTER;
  }
}

ParseToken GetTokenByCharacter(char character) {
  ParseToken token;
  token.tokenType = ParseCharacter(character);

  if (token.tokenType == NUMBER_CHARACTER) {
    token.character = character - '0';
  } else {
    token.character = character;
  }

  return token;
}
