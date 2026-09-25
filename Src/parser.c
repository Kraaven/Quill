#include "parser.h"
#include "JsonNode.h"
#include "ParserStack.h"
#include "TokenType.h"
#include "jsonNode.h"
#include "logging.h"
#include "sys/stat.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ParseJsonFile(char *FileName) {
  FILE *jsonFile = fopen(FileName, "rb");

  struct stat sb;
  fstat(fileno(jsonFile), &sb);
  int JsonFileSize = (int)sb.st_size;
  LOG("File size: %d", JsonFileSize);

  char *JsonFileContent = malloc(JsonFileSize + 1);

  fread(JsonFileContent, 1, JsonFileSize, jsonFile);

  bool ParsingString = false;
  float DotAccumulator = 0;

  List_JValue *ParserStack = CreateStack();

  for (int i = 0; i < JsonFileSize; i++) {
    LOG("Pasring : %c", JsonFileContent[i]);
    ParseToken token = GetTokenByCharacter(JsonFileContent[i]);

    switch (token.tokenType) {
    case OPEN_OBJECT:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {

        JValue *newJObject = malloc(sizeof(JValue));
        newJObject->Jvalue_Type = JOBJECT;
        newJObject->data.object = (JObject *)malloc(sizeof(JObject));
        newJObject->data.object->KeyValues = NULL;
        PushIntoStack(ParserStack, newJObject);
      }
      break;
    case OPEN_ARRAY:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {

        JValue *newArray = malloc(sizeof(JValue));
        newArray->Jvalue_Type = ARRAY;
        newArray->data.array = NULL;
        PushIntoStack(ParserStack, newArray);
      }
      break;
    case STRING_IDENTIFIER:
      if (!ParsingString) {
        JValue *newString = malloc(sizeof(JValue));
        newString->Jvalue_Type = STRING;
        newString->data.stringBuilder = List_Create_char();
        PushIntoStack(ParserStack, newString);
        ParsingString = true;
      } else {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek->Jvalue_Type != STRING) {
          LOG("Found STRING terminator : %d, But there is no string in the "
              "stack to insert into.",
              i);
          return false;
        }
        // JValue *stringObject = PopOfStack(ParserStack);
        JValue *stringObject = peek;
        List_char *stringBuilder = stringObject->data.stringBuilder;
        stringObject->data.string =
            malloc(sizeof(char) * (stringBuilder->size + 1));
        Node_char *charPtr = stringBuilder->head;
        for (int j = 0; j < stringBuilder->size; j++) {
          stringObject->data.string[j] = *(charPtr->value);
        }
        stringObject->data.string[stringBuilder->size] = '\0';

        /*
        JValue *newPeek = PeekIntoStack(ParserStack);
        if (newPeek->Jvalue_Type == ARRAY) {
          List_Insert_JValue(newPeek->data.array, stringObject);
        } else if (newPeek->Jvalue_Type == KEY_VALUE) {
          newPeek->data.KeyValue->value = stringObject;
          // TODO: Move this whole section to a dedicated ProcessPeek function
        } else if (newPeek->Jvalue_Type == JOBJECT) {
          JValue *KeyPair = malloc(sizeof(JValue));
          KeyPair->Jvalue_Type = KEY_VALUE;
          KeyPair->data.KeyValue->key = stringObject->data.string;
          PushIntoStack(ParserStack, KeyPair);
        }
        */

        ParsingString = false;
      }

      break;
    case WHITESPACE:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert "
              "character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert "
              "character "
              "into String and failed. Tried to insert the character into "
              "not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {
        LOG("Found WHITESPACE in %d", i)
        continue;
      }
      break;
    case KV_SEPERATOR:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {
        JValue *stringObject = PopOfStack(ParserStack);
        if (stringObject->Jvalue_Type != STRING) {
          LOG("Found Key Value Seperator at %d, with no string preceeding "
              "it.",
              i)
          return false;
        }

        JValue *peek = PeekIntoStack(ParserStack);
        if (peek->Jvalue_Type == KEY_VALUE) {
          peek->data.KeyValue->key = stringObject->data.string;
        } else if (peek->Jvalue_Type == JOBJECT) {
          JValue *KeyPair = malloc(sizeof(JValue));
          KeyPair->Jvalue_Type = KEY_VALUE;
          KeyPair->data.KeyValue = malloc(sizeof(KV));
          KeyPair->data.KeyValue->key = stringObject->data.string;
          PushIntoStack(ParserStack, KeyPair);
        } else {
          LOG("Found Key Value Seperator at %d, with no where to put the "
              "string "
              "before it.",
              i);
          return false;
        }
      }
      break;
    case VALUE_SEPERATOR:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {

        JValue *poppedObject = PopOfStack(ParserStack);
        JValue *peekObject = PeekIntoStack(ParserStack);

        if (peekObject->Jvalue_Type == ARRAY) {
          if (peekObject->data.array == NULL) {
            peekObject->data.array = List_Create_JValue();
          }
          List_Insert_JValue(peekObject->data.array, poppedObject);
        } else if (peekObject->Jvalue_Type == KEY_VALUE) {
          peekObject->data.KeyValue->value = poppedObject;
          poppedObject = PopOfStack(ParserStack);
          peekObject = PeekIntoStack(ParserStack);
          if (poppedObject->Jvalue_Type == KEY_VALUE &&
              peekObject->Jvalue_Type == JOBJECT) {

            if (peekObject->data.object->KeyValues == NULL) {
              peekObject->data.object->KeyValues = List_Create_KV();
            }
            List_Insert_KV(peekObject->data.object->KeyValues,
                           poppedObject->data.KeyValue);
          }
        } else {
          LOG("Found value seperator , : %d, with no array or key-value pair "
              "to insert into.",
              i);
          return false;
        }
      }
      break;
    case STRING_CHARACTER:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        }
      }
      break;

    case NUMBER_CHARACTER:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        }
      }

      JValue *peekNumber = PeekIntoStack(ParserStack);
      if (peekNumber->Jvalue_Type != FLOAT) {
        JValue *number = malloc(sizeof(JValue));
        number->Jvalue_Type = FLOAT;
        number->data.number = 0;
        peekNumber = number;
        PushIntoStack(ParserStack, peekNumber);
        DotAccumulator = -1;
      }

      if (DotAccumulator != -1) {
        peekNumber->data.number *= 10;
        peekNumber->data.number += token.character;
      } else {
        float frac = DotAccumulator * token.character;
        peekNumber->data.number += frac;
        DotAccumulator /= 10;
      }

      break;
    case DOT:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {
        DotAccumulator = 0.1;
      }
      break;
    case CLOSE_ARRAY:
      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {
        JValue *poppedObject = PopOfStack(ParserStack);
        JValue *peek = PeekIntoStack(ParserStack);

        if (poppedObject->Jvalue_Type == ARRAY &&
            poppedObject->data.array == NULL && peek != NULL &&
            peek->Jvalue_Type == ARRAY) {
          poppedObject->data.array = malloc(sizeof(JValue));
          PushIntoStack(ParserStack, poppedObject);
        } else if (poppedObject->Jvalue_Type == STRING ||
                   poppedObject->Jvalue_Type == FLOAT ||
                   poppedObject->Jvalue_Type == JOBJECT ||
                   poppedObject->Jvalue_Type == ARRAY) {
          if (peek->Jvalue_Type == ARRAY) {
            List_Insert_JValue(peek->data.array, poppedObject);
          } else {
            LOG("Found ] : %d character, with an object popped off, but no "
                "array to put into.",
                i);
          }
        }
      }
      break;
    case CLOSE_OBJECT:

      if (ParsingString) {
        JValue *peek = PeekIntoStack(ParserStack);
        if (peek != NULL && peek->Jvalue_Type == STRING) {
          char *character = malloc(sizeof(char));
          *character = token.character;
          List_Insert_char(peek->data.stringBuilder, character);
          continue;
        } else if (peek == NULL) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. There was no object to insert the "
              "character into ",
              JsonFileContent[i], i);
          return false;
        } else if (peek->Jvalue_Type != STRING) {
          LOG("Failed to parse Character %c : %d. Expected to insert character "
              "into String and failed. Tried to insert the character into not "
              "a string",
              JsonFileContent[i], i);
        }
      } else {

        JValue *peek = PeekIntoStack(ParserStack);

        if (peek->Jvalue_Type == JOBJECT &&
            peek->data.object->KeyValues == NULL) {
          peek->data.object->KeyValues = List_Create_KV();
          return false;
        }

        JValue *poppedObject = PopOfStack(ParserStack);
        JValue *peekObject = PeekIntoStack(ParserStack);

        if (peekObject->Jvalue_Type == KEY_VALUE) {
          peekObject->data.KeyValue->value = poppedObject;

          poppedObject = PopOfStack(ParserStack);
          peekObject = PeekIntoStack(ParserStack);

          if (peekObject->Jvalue_Type == JOBJECT) {
            if (peekObject->data.object->KeyValues == NULL) {
              peekObject->data.object->KeyValues = List_Create_KV();
            }

            List_Insert_KV(peekObject->data.object->KeyValues,
                           poppedObject->data.KeyValue);
          }
        } else {
          LOG("Attempted to insert object into KV, but no KV found");
          return false;
        }
      }

      break;
    }
  }

  JValue *peek = PeekIntoStack(ParserStack);
  if (peek != NULL && peek->Jvalue_Type == JOBJECT) {
    JValue *PoppedObject = PopOfStack(ParserStack);
    if (ParserStack->head != NULL) {
      LOG("Encountered End Of File, and yet the parser Stack is not "
          "empty");
      return false;
    } else {
      LOG("Encountered End of File, the stack is empty!");
      return true;
    }
  }

  return true;
}
