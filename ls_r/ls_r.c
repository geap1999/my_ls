#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../functions/common_functions.h"
#include "ls_r.h"

char **reverseArray(char **array, int nb_content);

int ls_r(char ***all_content, int *nb_content) {
    char **rev_content = reverseArray(*all_content, *nb_content);
    *all_content = rev_content;
    return EXIT_SUCCESS;
}

char **reverseArray(char **array, int nb_content) {
    char **revArray = malloc(nb_content * sizeof(char *));
    if (revArray == NULL) {
        printf("Failed to allocate memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nb_content; i++) {
        revArray[i] = array[nb_content - 1 - i];
    }
    return revArray;
}