#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OPCODE_PRINT 5619136
#define OPCODE_SET 350144
#define OPCODE_ADD 349824
#define OPCODE_MUL 351040
#define OPCODE_JUMP 1405504
#define OPCODE_IFEQ 1401168
#define OPCODE_IFMR 1401696
#define OPCODE_INPUT 5617280
#define OPCODE_PARAM 5603088

int REG[26 * 26];
int REG_TEMP;

int PARAM_PRINT_NEWLINE = 1;

const unsigned long hash(char *str) {
  unsigned long hash = 5381;

  while (*str++) {
    hash += *str;
    hash = hash << 2;
  }

  return hash;
}

int *getValue(char *token) {
  if (isupper(token[0])) {
    int id = (isupper(token[1]) ? token[1] - 'A' + 1 : 0) * 26 + token[0] - 'A';
    return &REG[id];
  } else {
    REG_TEMP = atoi(token);
    return &REG_TEMP;
  }
}

//Replace "\n" with the newline character
char *formatStr(char *str) {
  if (str == NULL || str[0] == '\n') { return str; }

  for (int i = 0; str[i + 1] != '\0'; i++) {
    if (str[i] == '\\' && str[i + 1] == 'n') {
      str[i] = ' ';
      str[i + 1] = '\n';
    }
  }

  return str;
}

int main(int argc, char** argv) {
  FILE *file = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t length = 0;

  if (argc < 2) {
    puts("Enter a file");
    return 0;
  }

  file = fopen(argv[1], "r");

  if (file == NULL) {
    puts("File doesn`t exist");
    return 0;
  }

  //Load arguments into registers
  for (int i = 2; i < argc; i++) {
    REG[i - 2] = atoi(argv[i]);
  }

  for (int currentLine = 0; (length = getline(&line, &len, file)) != -1; currentLine++) {
    char *opcode = strtok(line, " ");

    while (opcode != NULL) {
      if (opcode[0] == ';') {
        break;
      }

      char *firstParam = strtok(NULL, " ");
      char *secondParam = strtok(NULL, " ");

      switch (hash(opcode)) {
        case OPCODE_PRINT:
          if (firstParam[0] == '"') {
            char *str = malloc(length);
            memset(str, 0, length);

            sprintf(str, "%s %s", formatStr(firstParam + 1), formatStr(secondParam));

            do {
              firstParam = strtok(NULL, " ");
              sprintf(str, "%s %s", str, formatStr(firstParam));
            } while (firstParam != NULL);

            //???
            str[strlen(str) - 9] = '\0';

            printf("%s", str);

            free(str);

            goto line_done;
          } else {
            char *format = "%i\n";

            if (!PARAM_PRINT_NEWLINE) {
              format = "%i";
            }

            printf(format, *getValue(firstParam));
          }
        break;
        case OPCODE_INPUT:
          int input;

          if (scanf("%d", &input) == 1) {
            *getValue(firstParam) = input;
          }
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
          int lin = 0;
          int targetLine;

          if (firstParam[0] == '+' || firstParam[0] == '-') {
            int sign = (firstParam[0] == '+' ? 1 : -1);

            memmove(firstParam, firstParam + 1, strlen(firstParam));

            targetLine = currentLine + (sign * *getValue(firstParam));
          } else {
            targetLine = *getValue(firstParam);
          }

          currentLine = targetLine;

          rewind(file);

          //Genius
          while (lin++ != targetLine) { length = getline(&line, &len, file); }

          goto line_done;
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
        case OPCODE_PARAM:
          if (strcmp("PRINT_NEWLINE", firstParam) == 0) {
            PARAM_PRINT_NEWLINE = *getValue(secondParam);
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

    line_done:;
  }

  fclose(file);

  if (line) {
    free(line);
  }

  return 0;
}
