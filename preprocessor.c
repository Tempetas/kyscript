#include "preprocessor.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Maximum amount of jump labels
#define MAX_LABELS 512

extern const unsigned int strhash(const char str[]);

void preprocess(char lines[MAX_LINES][LINE_LENGTH]) {
	extern bool DEBUG;
	extern int linesInFile;

	if (DEBUG) {
		puts("[Debug] <-Preprocessing->");
	}

	unsigned short labels[MAX_LABELS] = { 0 };

	if (DEBUG) {
		puts("[Debug] -> Parse file");
	}

	char line[LINE_LENGTH];

	for (int currentLine = 0; lines[currentLine][0] != EOF; currentLine++) {
		strcpy(line, lines[currentLine]);

		if (line[0] == '.') {
			char* label = strtok(line + 1, " ");

			const unsigned int hash = strhash(label);

			if (labels[hash % MAX_LABELS]) {
				printf("[Error] Hash collision on line %i\n", currentLine);
				exit(1);
			}

			labels[hash % MAX_LABELS] = currentLine + 1;

			if (DEBUG) {
				printf("[Debug] The hash of the label on line %i is %u\n", currentLine, hash);
			}

			line[0] = ';';
		}

		strcpy(lines[currentLine], line);
	}

	if (DEBUG) {
		puts("[Debug] -> Replace strings");
	}

	for (int currentLine = 0; lines[currentLine][0] != EOF; currentLine++) {
		strcpy(line, lines[currentLine]);

		if (!strncmp("PRINT \"", line, 7)) {
			//Add proper newlines
			int i = 6;
			for (; line[i + 1] != '"'; i++)
				;
			//Remove everything past the last `"`
			if (line[i - 1] == '\\' && line[i] == 'n') {
				line[i - 1] = '\n';
				line[i] = '\0';
			}
		} else if (!strncmp("JUMP ", line, 5) && isalpha(line[5])) {
			strtok(line, " ");
			char* label = strtok(NULL, " ");

			const unsigned int hash = strhash(label);

			if (!labels[hash % MAX_LABELS]) {
				printf("[Error] Jump to unknown label on line %i\n", currentLine);
				exit(1);
			}

			//Replace jump labels
			char numStr[11];

			sprintf(numStr, "JUMP %d\n", labels[hash % MAX_LABELS] - 1);
			strcpy(line, numStr);
		}

		strcpy(lines[currentLine], line);
	}

	if (DEBUG) {
		puts("[Debug] <-Start source->");

		for (int currentLine = 0; lines[currentLine][0] != EOF; currentLine++) {
			printf("%i. %s", currentLine, lines[currentLine]);
		}

		puts("[Debug] <-End source->");
	}
}
