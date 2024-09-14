#include <stdlib.h>
#include "../functions/common_functions.h"
#include "basic_ls.h"

int basic_ls(char ***all_content, int *nb_content) {
    removeHiddenFiles(all_content, nb_content);
    sortArray(*all_content, *nb_content);
    return EXIT_SUCCESS;
}