#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 500
#define FILE_NAME_LENGTH 100

typedef struct {
    char **lines;
    int count;
    int capacity;
} Document;

void initializeDocument(Document *doc) {
    doc->lines = malloc(INITIAL_CAPACITY * sizeof(char *));
    if (doc->lines == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    doc->count = 0;
    doc->capacity = INITIAL_CAPACITY;
}

void ensureCapacity(Document *doc) {
    if (doc->count >= doc->capacity) {
        int newCapacity = doc->capacity * 2;
        char **newLines = realloc(doc->lines, newCapacity * sizeof(char *));
        if (newLines == NULL) {
            printf("Unable to increase document capacity.\n");
            exit(1);
        }
        doc->lines = newLines;
        doc->capacity = newCapacity;
    }
}

void insertLine(Document *doc, int lineNumber, const char *text) {
    if (lineNumber < 1 || lineNumber > doc->count + 1) {
        printf("Invalid line number. Use 1 to %d.\n", doc->count + 1);
        return;
    }

    ensureCapacity(doc);

    for (int i = doc->count; i >= lineNumber; i--) {
        doc->lines[i] = doc->lines[i - 1];
    }

    doc->lines[lineNumber - 1] = malloc(strlen(text) + 1);
    if (doc->lines[lineNumber - 1] == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(doc->lines[lineNumber - 1], text);
    doc->count++;
    printf("Line inserted successfully.\n");
}

void deleteLine(Document *doc, int lineNumber) {
    if (lineNumber < 1 || lineNumber > doc->count) {
        printf("Invalid line number. Document has %d line(s).\n", doc->count);
        return;
    }

    free(doc->lines[lineNumber - 1]);

    for (int i = lineNumber - 1; i < doc->count - 1; i++) {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->count--;
    printf("Line deleted successfully.\n");
}

void displayDocument(const Document *doc) {
    if (doc->count == 0) {
        printf("The document is empty.\n");
        return;
    }

    printf("\n----- DOCUMENT -----\n");
    for (int i = 0; i < doc->count; i++) {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }
    printf("--------------------\n");
}

void saveDocument(const Document *doc, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    for (int i = 0; i < doc->count; i++) {
        fprintf(file, "%s\n", doc->lines[i]);
    }

    fclose(file);
    printf("Document saved to %s.\n", fileName);
}

void loadDocument(Document *doc, const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("No existing file found. Starting with an empty document.\n");
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        insertLine(doc, doc->count + 1, buffer);
    }

    fclose(file);
    printf("Document loaded from %s.\n", fileName);
}

void searchDocument(const Document *doc, const char *word) {
    int found = 0;
    for (int i = 0; i < doc->count; i++) {
        if (strstr(doc->lines[i], word) != NULL) {
            printf("Found on line %d: %s\n", i + 1, doc->lines[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("Text not found.\n");
    }
}

void showWordCount(const Document *doc) {
    int words = 0;
    for (int i = 0; i < doc->count; i++) {
        int insideWord = 0;
        for (int j = 0; doc->lines[i][j] != '\0'; j++) {
            if (isspace((unsigned char)doc->lines[i][j])) {
                insideWord = 0;
            } else if (!insideWord) {
                words++;
                insideWord = 1;
            }
        }
    }
    printf("Lines: %d\nWords: %d\n", doc->count, words);
}

void showHelp(void) {
    printf("\nCommands:\n");
    printf("  insert <line number> <text>  - Insert a line\n");
    printf("  delete <line number>         - Delete a line\n");
    printf("  display                      - Display all lines\n");
    printf("  save <filename>              - Save document to a text file\n");
    printf("  load <filename>              - Load a text file into the document\n");
    printf("  search <word or phrase>      - Search all lines\n");
    printf("  count                        - Show line and word count\n");
    printf("  help                         - Show commands\n");
    printf("  quit                         - Exit the editor\n");
    printf("\nExample: insert 1 Welcome to my document\n\n");
}

void freeDocument(Document *doc) {
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    free(doc->lines);
}

int main(void) {
    Document doc;
    char input[ MAX_LINE_LENGTH + FILE_NAME_LENGTH ];
    char command[20];
    char argument[MAX_LINE_LENGTH];
    int lineNumber;

    initializeDocument(&doc);

    printf("=================================\n");
    printf("       SIMPLE LINE EDITOR\n");
    printf("=================================\n");
    printf("Type 'help' to see available commands.\n");

    while (1) {
        printf("\neditor> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%19s", command) != 1) {
            continue;
        }

        if (strcmp(command, "insert") == 0) {
            char *position = input + strlen(command);
            while (isspace((unsigned char)*position)) position++;
            char *endPointer;
            lineNumber = (int)strtol(position, &endPointer, 10);

            if (endPointer == position) {
                printf("Usage: insert <line number> <text>\n");
                continue;
            }
            while (isspace((unsigned char)*endPointer)) endPointer++;
            if (*endPointer == '\0') {
                printf("Please provide the line text.\n");
                continue;
            }
            insertLine(&doc, lineNumber, endPointer);

        } else if (strcmp(command, "delete") == 0) {
            if (sscanf(input, "%*s %d", &lineNumber) != 1) {
                printf("Usage: delete <line number>\n");
            } else {
                deleteLine(&doc, lineNumber);
            }

        } else if (strcmp(command, "display") == 0) {
            displayDocument(&doc);

        } else if (strcmp(command, "save") == 0) {
            if (sscanf(input, "%*s %99s", argument) != 1) {
                printf("Usage: save <filename>\n");
            } else {
                saveDocument(&doc, argument);
            }

        } else if (strcmp(command, "load") == 0) {
            if (sscanf(input, "%*s %99s", argument) != 1) {
                printf("Usage: load <filename>\n");
            } else {
                loadDocument(&doc, argument);
            }

        } else if (strcmp(command, "search") == 0) {
            char *term = input + strlen(command);
            while (isspace((unsigned char)*term)) term++;
            if (*term == '\0') {
                printf("Usage: search <word or phrase>\n");
            } else {
                searchDocument(&doc, term);
            }

        } else if (strcmp(command, "count") == 0) {
            showWordCount(&doc);

        } else if (strcmp(command, "help") == 0) {
            showHelp();

        } else if (strcmp(command, "quit") == 0) {
            printf("Exiting line editor. Goodbye!\n");
            break;

        } else {
            printf("Unknown command. Type 'help' to see available commands.\n");
        }
    }

    freeDocument(&doc);
    return 0;
}
