#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_LENGTH 128

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

#define PARAM_PRINT_NEWLINE -1079885616
int PRINT_NEWLINE = 1;

const int hash(char *str) {
  int hash = 5381;

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
  char line[LINE_LENGTH];

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

  for (int currentLine = 0; fgets(line, LINE_LENGTH, file) != NULL; currentLine++) {
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
            char *str = malloc(LINE_LENGTH);
            memset(str, 0, LINE_LENGTH);

            strcpy(str, formatStr(firstParam + 1));

            while (secondParam != NULL && secondParam[0] != ';') {
              sprintf(str, "%s %s", str, formatStr(secondParam));
	      secondParam = strtok(NULL, " ");
	    }

	    //Hack!
            str[strlen(str) - ((str[strlen(str) - 2] == '"') ? 2 : 1)] = '\0';

            printf("%s", str);

            free(str);

            goto line_done;
          } else {
            printf((PRINT_NEWLINE) ? "%i\n" : "%i", *getValue(firstParam));
          }
        break;
        case OPCODE_INPUT:;
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
        case OPCODE_JUMP:;
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
          while (lin++ != targetLine) { fgets(line, LINE_LENGTH, file); }

          goto line_done;
        break;
        //Disgusting
        case OPCODE_IFEQ:
          if (*getValue(firstParam) != *getValue(secondParam)) {
            fgets(line, LINE_LENGTH, file);
          }
        break;
        case OPCODE_IFMR:
          if (*getValue(firstParam) < *getValue(secondParam)) {
            fgets(line, LINE_LENGTH, file);
          }
        break;
        case OPCODE_PARAM:
          switch (hash(firstParam)) {
            case PARAM_PRINT_NEWLINE:
              PRINT_NEWLINE = *getValue(secondParam);
            break;
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

  return 0;
}
