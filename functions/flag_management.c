#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "./flag_management.h"
#include "./common_functions.h"
#include "../ls_Rec/ls_Rec.h"
#include "../ls_d/ls_d.h"
#include "../basic_ls/basic_ls.h"
#include "../ls_a/ls_a.h"
#include "../ls_Alm/ls_Alm.h"
#include "../ls_t/ls_t.h"
#include "../ls_r/ls_r.h"
#include "../ls_l/ls_l.h"
#include "../ls_1/ls_1.h"

//see if a string is in an array
int includes(char *array[], int size, char *str) {
    for (int i = 0; i < size; i++) {
        if (compareStrings(array[i], str) == 0) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int isArrayEmpty(int size) {
    if (size == 0) {
        return 1;
    }
    return 0;
}

void removeRFlag(int *argc, char *argv[]) {
    for (int i = 1; i < *argc; i++) {
        if (compareStrings(argv[i], "-R") == 0) {
            for (int j = i; j < *argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
            i--;
        }
    }
}

int isValidFlags(int argc, char *argv[]) {
    char *valid_flags[] = {"-R", "-d", "-a", "-A", "-t", "-r", "-1", "-l", "-L"};
    for(int i = 1; i<argc; i++){
        if(argv[i][0] == '-' && !includes(valid_flags, 9, argv[i])){
            return i;
        }
    }
    return 0;
}

void callFlags(int argc, char *argv[], char *path, char ***all_content, int *size){
    if(isValidFlags(argc, argv) != 0) {
        int err_index = isValidFlags(argc, argv);
        printf("Flag %s does not exist.\nPlease refer to the my_ls man ;)\n", argv[err_index]);
    }
    else if(includes(argv, argc, "-d")) {
        char **dir_name = ls_d(path);
        int new_size = 1;
        if(includes(argv, argc, "-l")) {
            ls_l(&dir_name, &new_size, path, false);
        }
        else{
            printf("%s  ", dir_name[0]);
        }
    }
    else if(includes(argv, argc, "-R")) {
        removeRFlag(&argc, argv);
        ls_Rec(argc, argv, all_content, size, path);
    }
    else {
        if(includes(argv, argc, "-a")) {
            ls_a(all_content, size);
        }
        else if(includes(argv, argc, "-A")) {
            ls_Alm(all_content, size);
        }
        else {
            basic_ls(all_content, size);
        }
        if(includes(argv, argc, "-t")) {
            if (includes(argv, argc, "-L")){
                ls_t(all_content, size, path, true);
            }
            else{
                ls_t(all_content, size, path, false);
            }
        }
        if(includes(argv, argc, "-r")) {
            ls_r(all_content, size);
        }
        if (includes(argv, argc, "-l")){
            if (includes(argv, argc, "-L")){
                ls_l(all_content, size, path, true);
            }
            else{
                ls_l(all_content, size, path, false);
            }
        }
        else if(includes(argv, argc, "-1")) {
            ls_1(all_content, size);
        }
        else {
            if(!isArrayEmpty(*size)) {
                int terminal_width = getTerminalWidth();
                printContent(*all_content, *size, terminal_width);
            }
        }
    }
}