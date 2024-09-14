#include <stdio.h>
#include <stdlib.h>
#include "../functions/common_functions.h"
#include "ls_1.h"

int ls_1(char ***all_content, int *nb_content) {
    for (int i = 0; i<*nb_content; i++){
        printf("%s\n", (*all_content)[i]);
    }
    return EXIT_SUCCESS;
}