#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "functions/common_functions.h"
#include "functions/flag_management.h"

#define INITIAL_SIZE 1

ArrayAndSize getPathsAndSize(int argc, char *argv[]);
void applyFlagsToDir(int argc, char *argv[], char *path);

int main(int argc, char *argv[]){
    ArrayAndSize paths_and_size = getPathsAndSize(argc, argv);
    char **all_paths = paths_and_size.array;
    int nb_paths = paths_and_size.size;
    if (nb_paths == 0){
        applyFlagsToDir(argc, argv, ".");
    }
    if (nb_paths == 1){
        applyFlagsToDir(argc, argv, all_paths[0]);
        if (includes(argv, argc, "-d") && !includes(argv, argc, "-l")){
            printf("\n");
        }
    }
    else{
        for (int i = 0; i<nb_paths; i++){
            if (!includes(argv, argc, "-d")){
                printf("%s:\n", all_paths[i]);
            }
            applyFlagsToDir(argc, argv, all_paths[i]);
            if (i != nb_paths - 1 && !includes(argv, argc, "-d")){
                printf("\n");
            }
        }
        if (includes(argv, argc, "-d") && !includes(argv, argc, "-l")){
            printf("\n");
        }
    }
    return EXIT_SUCCESS;
}

ArrayAndSize getPathsAndSize(int argc, char *argv[]) {
    ArrayAndSize result = {NULL, 0};
    char **all_paths = malloc(INITIAL_SIZE * sizeof(char *));
    if (all_paths == NULL) {
        printf("malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int size = 0;
    int capacity = INITIAL_SIZE;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            push(&all_paths, &size, &capacity, argv[i]);
        }
    }
    sortArray(all_paths, size);
    result.array = all_paths;
    result.size = size;
    return result;
}

void applyFlagsToDir(int argc, char *argv[], char *path) {
    ArrayAndSize dir_and_size = getDirContentAndSize(path);
    char **all_content = dir_and_size.array;
    int nb_content = dir_and_size.size;
    callFlags(argc, argv, path, &all_content, &nb_content);
    freeArray(all_content, nb_content);
}