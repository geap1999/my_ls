#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../functions/common_functions.h"
#include "./ls_Alm.h"

void removeDotDir(char ***array, int *size);

int ls_Alm(char ***all_content, int *nb_content) {
    removeDotDir(all_content, nb_content);
    sortArray(*all_content, *nb_content);
    return EXIT_SUCCESS;
}

void removeDotDir(char ***array, int *size) {
    for (int i = 0; i < *size; ) { 
        if (compareStrings((*array)[i], ".") == 0 || compareStrings((*array)[i], "..") == 0 )  {
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