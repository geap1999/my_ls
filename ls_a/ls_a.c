#include <stdlib.h>
#include "../functions/common_functions.h"
#include "ls_a.h"

int ls_a(char ***all_content, int *nb_content) {
    sortArray(*all_content, *nb_content);
    return EXIT_SUCCESS;
}