#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

typedef struct {
    char **array;
    int size;
} ArrayAndSize;

ArrayAndSize getDirContentAndSize(char *path);
void removeHiddenFiles(char ***array, int *size);
int getTerminalWidth();
void push(char ***array, int *size, int *capacity, char *str);
int getStrLength(char *str);
int compareStrings(char *str1, char *str2);
void sortArray(char **array, int size);
char *duplicateStr(char *src);
void printContent(char **array, int size, int terminal_width);
void freeArray(char **array, int size);
char* getPath(char *path, char *content);

#endif 