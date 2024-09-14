#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include "../functions/common_functions.h"
#include "ls_t.h"

typedef struct {
    char *name;
    time_t mtime;
} FileInfo;

int compareByTime(const void *a, const void *b) {
    const FileInfo *fileA = (const FileInfo *)a;
    const FileInfo *fileB = (const FileInfo *)b;
    if (fileB->mtime != fileA->mtime) {
        return (fileB->mtime - fileA->mtime);
    }
    return compareStrings(fileA->name, fileB->name);
}

void sortByTime(FileInfo *fileInfoArray, int size, char ***all_content) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareByTime(&fileInfoArray[j], &fileInfoArray[j + 1]) > 0) {
                FileInfo temp = fileInfoArray[j];
                fileInfoArray[j] = fileInfoArray[j + 1];
                fileInfoArray[j + 1] = temp;
            }
        }
    }
    *all_content = malloc(size * sizeof(char *));
    if (all_content == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        (*all_content)[i] = fileInfoArray[i].name;
    }
}

int ls_t(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    FileInfo *fileInfoArray = malloc(*nb_content * sizeof(FileInfo));
    if (fileInfoArray == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *nb_content; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        if (!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } else {
            if (stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        fileInfoArray[i].name = duplicateStr((*all_content)[i]);
        if (fileInfoArray[i].name== NULL) {
            printf("Failed to allocate memory for string copy: %s\n", strerror(errno));
            free(fileInfoArray[i].name);
            exit(EXIT_FAILURE);
        }
        fileInfoArray[i].mtime = file_stat.st_mtime;
        free(path_to_content);
    }
    sortByTime(fileInfoArray, *nb_content, all_content);
    free(fileInfoArray);
    return EXIT_SUCCESS;
}