#include "common_functions.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_SIZE 1

ArrayAndSize getDirContentAndSize(char *path) {
    DIR *directory;
    struct dirent *entry;
    ArrayAndSize result = {NULL, 0};
    char **all_content = malloc(INITIAL_SIZE * sizeof(char *));
    if (all_content == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int size = 0;
    int capacity = INITIAL_SIZE;
    directory = opendir(path);
    if (directory == NULL) {
        printf("my_ls: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(directory)) != NULL) {
        push(&all_content, &size, &capacity, entry->d_name);
    }
    closedir(directory);
    result.array = all_content;
    result.size = size;
    return result;
}

char *duplicateStr(char *src) {
    int length = getStrLength(src);
    char *dup = malloc(length + 1);
    if (dup == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        dup[i] = src[i];
    }
    dup[length] = '\0';
    return dup;
}

void removeHiddenFiles(char ***array, int *size) {
    for (int i = 0; i < *size; ) { 
        if ((*array)[i][0] == '.') {
            free((*array)[i]);
            for (int j = i; j < *size - 1; j++) {
                (*array)[j] = (*array)[j + 1];
            }
            (*size)--;
        } else {
            i++;
        }
    }
    *array = realloc(*array, (*size) * sizeof(char *));
    if (*array == NULL && *size > 0) {
        printf("Failed to realloc: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int getTerminalWidth(){
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        printf("ioctl failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    int terminal_width = w.ws_col;
    return terminal_width;
}

//pushes a string in an array dynamically 
void push(char ***array, int *size, int *capacity, char *str) {
    if (*size >= *capacity) {
        *capacity *= 2;
        char **check = realloc(*array, *capacity * sizeof(char *));
        if (check == NULL) {
            printf("Failed to realloc: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        *array = check;
    }
    (*array)[*size] = duplicateStr(str);
    if ((*array)[*size] == NULL) {
        printf("Failed to allocate memory for string copy: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    (*size)++;
}

int getStrLength(char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int compareStrings(char *str1, char *str2) {
    int len1 = getStrLength(str1);
    int len2 = getStrLength(str2);
    int min_len = (len1 < len2) ? len1 : len2;

    for (int i = 0; i < min_len; i++) {
        if (str1[i] < str2[i]) {
            return -1;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
    }
    if (len1 < len2) {
        return -1;
    } else if (len1 > len2) {
        return 1;
    }
    return 0;
}

//sort array alphabetically
void sortArray(char **array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareStrings(array[j], array[j + 1]) > 0) {
                char *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void freeArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

void addSpaces(char *dest, char *src, int size) {
    int i = 0;
    while (src[i] != '\0' && i < size) {
        dest[i] = src[i];
        i++;
    }
    while (i < size) {
        dest[i] = ' ';
        i++;
    }
    dest[size] = '\0';
}

void updateColumnSizeInArray(int column_length, char **array, int column_ranges[2]) {
    for (int i = column_ranges[0]; i <= column_ranges[1]; i++) {
        int string_length = getStrLength(array[i]);
        if (string_length < column_length) {
            char *new_str = malloc(column_length + 1);
            if (new_str == NULL) {
                printf("malloc failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            addSpaces(new_str, array[i], column_length);
            free(array[i]);
            array[i] = new_str;
        }
    }
}

int getColumnIndex(int index, int columns, int column_ranges[][2]) {
    for (int i = 0; i < columns; i++) {
        if (index >= column_ranges[i][0] && index <= column_ranges[i][1]) {
            return i;
        }
    }
    return -1;
}

int* findColumnRange(int index, int columns, int column_ranges[][2]) {
    for (int i = 0; i < columns; i++) {
        if (index >= column_ranges[i][0] && index <= column_ranges[i][1]) {
            return column_ranges[i];
        }
    }
    return NULL;
}

void fillColumnRanges(int column_ranges[][2], int columns, int rows, int size) {
    int gaps = 0;
    for (int c = 0; c < columns; c++) {
        column_ranges[c][0] = gaps;
        gaps += rows;                    
        if (gaps - 1 >= size) {
            column_ranges[c][1] = size - 1;
        } else {
            column_ranges[c][1] = gaps - 1;
        }   
    }
}

void populateArray(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

char **copyCharArray(char **array, int size) {
    if (array == NULL) return NULL;
    char **array_copy = malloc((size + 1) * sizeof(char *));
    if (array_copy == NULL) {
        printf("Failed to allocate memory for array_copy: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        if (array[i] == NULL) {
            array_copy[i] = NULL;
        } else {
            array_copy[i] = duplicateStr(array[i]);
            if (array_copy[i] == NULL) {
                printf("Failed to allocate memory for string copy: %s\n", strerror(errno));
                for (int j = 0; j < i; j++) {
                    free(array_copy[j]);
                }
                free(array_copy);
                exit(EXIT_FAILURE);
            }
        }
    }
    array_copy[size] = NULL;
    return array_copy;
}

void printContent(char **array, int size, int terminal_width) {
    char **array_copy = copyCharArray(array, size);
    int rows = 1;
    int columns = size;
    int column_lengths[columns];
    populateArray(column_lengths, columns);
    for (int i = 0; i < rows; i++){
        int total_column_length = 0;
        int column_ranges[columns][2];
        fillColumnRanges(column_ranges, columns, rows, size);
        for (int j = i; j < size; j += rows) {
            int string_length = getStrLength(array_copy[j]);
            int column_index = getColumnIndex(j, columns, column_ranges);
            if(string_length > column_lengths[column_index] && rows != 1){
                column_lengths[column_index] = string_length;
                int* column_range = findColumnRange(j, columns, column_ranges);
                updateColumnSizeInArray(column_lengths[column_index], array_copy, column_range);
                i = -1;
                break;
            }
            total_column_length += getStrLength(array_copy[j]) + 2;
            if(total_column_length >= terminal_width){
                rows ++;
                columns = ceil((double)size/rows);
                freeArray(array_copy, size);
                array_copy = copyCharArray(array, size);
                populateArray(column_lengths, columns);
                i = -1;
                break ;
            }
        }
    }
    for (int i = 0; i < rows; i++){
        for (int j = i; j < size; j += rows) {
            printf("%s  ", array_copy[j]);
        }
        printf("\n");
    }
    freeArray(array_copy, size);
}

//build the path with the directory and the name of the file/folder
int buildPath(char *result, int total_length, char *path, char *content) {
    int path_length = getStrLength(path);
    int content_length = getStrLength(content);
    int required_length = path_length + 1 + content_length; 
    if (required_length >= total_length) {
        return required_length;
    }
    int i;
    for (i = 0; i < path_length; i++) {
        result[i] = path[i];
    }
    result[i++] = '/';
    for (int j = 0; j < content_length; j++) {
        result[i++] = content[j];
    }
    result[i] = '\0';
    return required_length;
}

char* getPath(char *path, char *content) {
    int path_length = getStrLength(path);
    int content_length = getStrLength(content);
    int total_length = path_length + content_length + 2;
    char *result = malloc(total_length * sizeof(char));
    if (result == NULL) {
        printf("Failed to allocate memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    buildPath(result, total_length, path, content);
    return result;
}

