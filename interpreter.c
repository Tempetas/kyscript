#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"

//Opcode hashes, e.g. OPCODE_PRINT = strhash("PRINT")
#define OPCODE_PRINT 234108866
#define OPCODE_SET 193454910
#define OPCODE_ADD 193453293
#define OPCODE_MUL 193472070
#define OPCODE_JUMP 2089614743
#define OPCODE_IFEQ 2089067009
#define OPCODE_IFMR 2089075754
#define OPCODE_INPUT 229624364
#define OPCODE_PARAM 214257254
#define OPCODE_MOD 193465272
#define OPCODE_DIV 193459332

//Same as for opcodes, but for interpreter parameters
#define PARAM_PRINT_NUM_NEWLINE 3498924034
#define PARAM_DEBUG 218447528

//Interpreter parameters
bool PRINT_NUM_NEWLINE = 1;
bool DEBUG = 0;

//The "memory" of the interpreter
int REG[26 * 26];

/*Temporary register used to return a pointer to an int
	when it is provided as an opcode argument*/
int REG_TEMP;

//Number of lines in the file that is being interpreted
int linesInFile;

const unsigned int strhash(const char str[]) {
	int hash = 5381;

	//Hack! TODO: remove
	while (*str++ && *str != '\n' && *(str + 1) != ';') {
		hash = ((hash << 5) + hash) + *str;
	}

	return hash;
}

//Returns the value of a register or the number provided
int* getValue(const char token[]) {
	if (isupper(token[0])) {
		int id = (isupper(token[1]) ? token[1] - 'A' + 1 : 0) * 26 + token[0] - 'A';
		return &REG[id];
	} else {
		REG_TEMP = atoi(token);
		return &REG_TEMP;
	}
}

int main(const int argc, const char* argv[]) {
	char lines[MAX_LINES][LINE_LENGTH];
	memset(lines, EOF, sizeof(lines));

	if (argc < 2) {
		puts("Enter a file to execute");
		return 0;
	}

	FILE* file = fopen(argv[1], "r");

	if (file == NULL) {
		printf("File \"%s\" doesn`t exist\n", argv[1]);
		return 0;
	}

	//Early debug mode toggle
	int argstart = 2;
	if ((argc > argstart) && !strcmp(argv[argstart], "--debug")) {
		DEBUG = true;
		argstart++;
	}

	char line[LINE_LENGTH];

	if (DEBUG) {
		puts("[Debug] <-Loading the file->");
	}

	int currentLine = 0;

	for (; currentLine < MAX_LINES && fgets(line, LINE_LENGTH, file) != NULL; currentLine++) {
		strcpy(lines[currentLine], line);
	}

	linesInFile = currentLine + 1;

	fclose(file);

	preprocess(lines);

	if (DEBUG) {
		puts("[Debug] <-Interpreting->");
	}

	//Load arguments into registers
	for (int i = argstart; i < argc; i++) {
		REG[i - 2] = atoi(argv[i]);
	}

	//Interpret the file
	for (int currentLine = 0; lines[currentLine][0] != EOF; currentLine++) {
	/*if (DEBUG) {
		printf("[Debug] Line %i\n", currentLine);
	}*/

	//Used to skip the for statement
	line_skip:;

		strcpy(line, lines[currentLine]);

		//First token on every line, usually the opcode
		char* opcode = strtok(line, " ");

		//Continue if the line is a comment
		if (opcode[0] == ';') {
			continue;
		}

		//Opcode parameters
		char* firstParam = strtok(NULL, " ");
		char* secondParam = strtok(NULL, " ");

		switch (strhash(opcode)) {
			case OPCODE_PRINT:
				if (firstParam[0] == '"') {
					//Concatenate all words in the string
					char* str = malloc(LINE_LENGTH);
					memset(str, 0, LINE_LENGTH);

					strcpy(str, firstParam + 1);

					while (secondParam != NULL && secondParam[0] != ';') {
						strcat(str, " ");
						strcat(str, secondParam);
						secondParam = strtok(NULL, " ");
					}

					//Work around the weirdness of the preprocessor
					int len = strlen(str);

					if (len < 3) {
						puts("");
					} else {
						for (int i = 1; i <= 2; i++) {
							if (str[len - i] == '"') {
								str[len - i] = '\0';
								break;
							}
						}
					}

					printf("%s", str);

					free(str);

					continue;
				} else {
					printf((PRINT_NUM_NEWLINE) ? "%i\n" : "%i", *getValue(firstParam));
				}
				break;
			case OPCODE_INPUT:;
				if (scanf("%d", &REG_TEMP) == 1) {
					*getValue(firstParam) = REG_TEMP;
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
			case OPCODE_DIV:
				*getValue(firstParam) /= *getValue(secondParam);
				break;
			case OPCODE_JUMP:;
				//The line we will be jumping to
				int targetLine;

				if (firstParam[0] == '+' || firstParam[0] == '-') {
					targetLine = currentLine + ((firstParam[0] == '+' ? 1 : -1) * *getValue(firstParam + 1));
				} else {
					targetLine = *getValue(firstParam);
				}

				currentLine = targetLine;

				goto line_skip;
				break;
			case OPCODE_IFEQ:
				if (*getValue(firstParam) != *getValue(secondParam)) {
					currentLine++;
				}
				break;
			case OPCODE_IFMR:
				if (*getValue(firstParam) < *getValue(secondParam)) {
					currentLine++;
				}
				break;
			case OPCODE_PARAM:
				switch (strhash(firstParam)) {
					case PARAM_PRINT_NUM_NEWLINE:
						PRINT_NUM_NEWLINE = *getValue(secondParam);
						break;
					case PARAM_DEBUG:
						DEBUG = *getValue(secondParam);
						break;
				}
				break;
			case OPCODE_MOD:;
				*getValue(firstParam) %= *getValue(secondParam);
				break;
			default:
				break;
		}
	}

	return 0;
}
