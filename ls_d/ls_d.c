#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "../functions/common_functions.h"
#include "ls_d.h"

char **ls_d(char *path) {
    int size = 0;
    int capacity = 1;
    char **actual_directory = malloc(1 * sizeof(char *));
    if (actual_directory == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    push(&actual_directory, &size, &capacity, path);
    return actual_directory;
}