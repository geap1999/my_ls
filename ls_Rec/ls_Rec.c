#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../functions/flag_management.h"
#include "../functions/common_functions.h"
#include "ls_Rec.h"

void exploreDirectory(int argc, char *argv[], char ***all_content, char *path, int *size);

int ls_Rec(int argc, char *argv[], char ***all_content, int *nb_content, char *path) {
    printf("%s:\n", path);
    callFlags(argc, argv, path, all_content, nb_content);
    exploreDirectory(argc, argv, all_content, path, nb_content);
    return EXIT_SUCCESS;
}

void exploreDirectory(int argc, char *argv[], char ***all_content, char *path, int *size) {
    for (int i = 0; i < *size; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        struct stat statbuf;
        if (stat(path_to_content, &statbuf) == 0 && S_ISDIR(statbuf.st_mode) && compareStrings((*all_content)[i], ".") != 0 && compareStrings((*all_content)[i], "..") != 0) {
            printf("\n");
            ArrayAndSize dir_and_size = getDirContentAndSize(path_to_content);
            char **sub_content = dir_and_size.array;
            int nb_content = dir_and_size.size;
            ls_Rec(argc, argv, &sub_content, &nb_content, path_to_content);
        }
    }
}