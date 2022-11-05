#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPCODE_PRINT 5619136
#define OPCODE_SET 350144
#define OPCODE_ADD 349824
#define OPCODE_MUL 351040
#define OPCODE_JUMP 1405504
#define OPCODE_IFEQ 1401168
#define OPCODE_IFMR 1401696

int REG_A, REG_B, REG_C, REG_X, REG_Y, REG_Z;

const unsigned long hash(char *str) {
  unsigned long hash = 5381;

  while (*str++) {
    hash += *str;
    hash = hash << 2;
  }

  return hash;
}

int *getValue(char *token) {
  switch (token[0]) {
    case 'X':
      return &REG_X;
    break;
    case 'Y':
      return &REG_Y;
    break;
    case 'Z':
      return &REG_Z;
    break;
    case 'A':
      return &REG_A;
    break;
    case 'B':
      return &REG_B;
    break;
    case 'C':
      return &REG_C;
    break;
    default:
      REG_A = atoi(token);
      return &REG_A;
    break;
  }
}

int main(int argc, char** argv) {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  ssize_t length;

  if (argc < 2) {
    puts("Enter a file");
    return 0;
  }

  file = fopen(argv[1], "r");

  if (file == NULL) {
    puts("File doesn`t exist");
    return 0;
  }

  while ((length = getline(&line, &len, file)) != -1) {
    char *opcode = strtok(line, " ");

    while(opcode != NULL) {
      if (opcode[0] == ';') {
        break;
      }

      char *firstParam = strtok(NULL, " ");
      char *secondParam = strtok(NULL, " ");

      switch (hash(opcode)) {
        case OPCODE_PRINT:
          printf("%i\n", *getValue(firstParam));
        break;
        case OPCODE_SET:
          *getValue(firstParam) = *getValue(secondParam);
        break;
        case OPCODE_ADD:
          *getValue(firstParam) += *getValue(secondParam);
        break;
        case OPCODE_MUL:
          *getValue(firstParam) *= *getValue(secondParam);
        break;
        case OPCODE_JUMP:
          int currentLine = 0;
          int targetLine = *getValue(firstParam);

          rewind(file);

          //Genius
          while (currentLine++ != targetLine) { length = getline(&line, &len, file); }
        break;
        //Disgusting
        case OPCODE_IFEQ:
          if (*getValue(firstParam) != *getValue(secondParam)) {
            length = getline(&line, &len, file);
          }
        break;
        case OPCODE_IFMR:
          if (*getValue(firstParam) < *getValue(secondParam)) {
            length = getline(&line, &len, file);
          }
        break;
        default:
          if (opcode[0] != '\n') {
            //printf("Uknown opcode: %s\n", opcode);
          }
        break;
      }

      opcode = strtok(NULL, ",");
    }
  }

  fclose(file);

  if (line) {
    free(line);
  }

  return 0;
}
