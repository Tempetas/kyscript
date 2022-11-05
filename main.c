#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPCODE_PRINT 210685452402
#define OPCODE_SET 193469745
#define OPCODE_ADD 193450094
#define OPCODE_MUL 193463731
#define OPCODE_JUMP 6384195425
#define OPCODE_IFEQ 6384142890
#define OPCODE_IFMR 6384143155
#define OPCODE_IFLS 6384143123

int REG_A, REG_B, REG_C, REG_X, REG_Y, REG_Z;

const unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
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

  //printf("%ld\n", hash("IFLS"));

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
        case OPCODE_IFLS:
          if (*getValue(firstParam) > *getValue(secondParam)) {
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
