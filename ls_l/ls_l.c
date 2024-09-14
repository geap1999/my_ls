#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include "../functions/common_functions.h"
#include "ls_l.h"

int getTotalBlocks(char ***all_content, int *nb_content, char *path, bool with_L);
void getPermissions(mode_t mode);
long int getMaxLinkCount(char ***all_content, int *nb_content, char *path, bool with_L);
int getMaxLengthPwd(char ***all_content, int *nb_content, char *path, bool with_L);
int getMaxLengthGrid(char ***all_content, int *nb_content, char *path, bool with_L);
long int getMaxLengthSize(char ***all_content, int *nb_content, char *path, bool with_L);
int count_digits(long number);

int ls_l(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    struct passwd *pw;
    struct group *gr;
    struct tm *time;
    char to_str_time[1000];
    int total_blocks = getTotalBlocks(all_content, nb_content, path, with_L);
    long int max_lc = getMaxLinkCount(all_content, nb_content, path, with_L);
    int max_lc_char = count_digits(max_lc);
    int max_pwd_char = getMaxLengthPwd(all_content, nb_content, path, with_L);
    int max_grid_char = getMaxLengthGrid(all_content, nb_content, path, with_L);
    long int max_file_size = getMaxLengthSize(all_content, nb_content, path, with_L);
    int max_file_size_char = count_digits(max_file_size);
    
    printf("total %d\n", total_blocks);

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

        getPermissions(file_stat.st_mode);
        
        printf(" %*ld", max_lc_char, (long int)file_stat.st_nlink);

        pw = getpwuid(file_stat.st_uid);
        printf(" %-*s", max_pwd_char, pw->pw_name);
        
        gr = getgrgid(file_stat.st_gid);
        printf(" %-*s", max_grid_char, gr->gr_name);

        printf(" %*ld", max_file_size_char, (long int)file_stat.st_size);

        time = localtime(&file_stat.st_mtime);
        strftime(to_str_time, sizeof(to_str_time), "%b %d %H:%M", time);
        printf(" %s", to_str_time);

        printf(" %s", (*all_content)[i]);

        if (S_ISLNK(file_stat.st_mode)) {
            char symlink_target[1000];
            int result_sl = readlink(path_to_content, symlink_target, sizeof(symlink_target) - 1);
            if (result_sl != -1) {
                symlink_target[result_sl] = '\0';
                printf(" -> %s", symlink_target);
            } else {
                printf("readlink failed: %s\n", strerror(errno));
            }
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}

void getPermissions(mode_t mode) {
    printf( (S_ISDIR(mode)) ? "d" : (S_ISLNK(mode)) ? "l" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
}

int getTotalBlocks(char ***all_content, int *nb_content, char *path, bool with_L) {
    int total_blocks = 0;
    struct stat file_stat;

    for (int i = 0; i < *nb_content; i++) { 
        if (compareStrings((*all_content)[i], ".") == 0 || compareStrings((*all_content)[i], "..") == 0) {
            continue;
        }
        char *path_to_content = getPath(path, (*all_content)[i]);
        if(!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } 
        else {
            if(stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        total_blocks += (int)file_stat.st_blocks;
    }
    return total_blocks/2;
}

long int getMaxLinkCount(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    long int lc;
    long int max_length = 0;
    for(int i = 0; i < *nb_content; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        if(!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } 
        else {
            if(stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        lc = (long int)file_stat.st_nlink;
        if (lc > max_length) {
            max_length = lc;
        }
    }
    return max_length;
}

int getMaxLengthPwd(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    struct passwd *pw;
    int max_length = 0;
    for(int i = 0; i < *nb_content; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        if(!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } 
        else {
            if(stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        pw = getpwuid(file_stat.st_uid);
        if(pw != NULL) {
            int length = getStrLength(pw->pw_name);
            if (length > max_length) {
                max_length = length;
            }
        }
    }
    return max_length;
}

int getMaxLengthGrid(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    struct group *gr;
    int max_length = 0;
    for(int i = 0; i < *nb_content; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        if(!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } 
        else {
            if(stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        gr = getgrgid(file_stat.st_gid);
        if(gr != NULL) {
            int length = getStrLength(gr->gr_name);
            if (length > max_length) {
                max_length = length;
            }
        }
    }
    return max_length;
}

long int getMaxLengthSize(char ***all_content, int *nb_content, char *path, bool with_L) {
    struct stat file_stat;
    long int max_length = 0;
    for(int i = 0; i < *nb_content; i++) {
        char *path_to_content = getPath(path, (*all_content)[i]);
        if(!with_L) {
            if (lstat(path_to_content, &file_stat) == -1) {
                printf("lstat failed: %s\n", strerror(errno));
                continue;
            }
        } 
        else {
            if(stat(path_to_content, &file_stat) == -1) {
                printf("stat failed: %s\n", strerror(errno));
                continue;
            }
        }
        long int length = (long int)file_stat.st_size;
        if (length > max_length) {
            max_length = length;
        }
    }
    return max_length;
}

int count_digits(long number) {
    int count = 0;
    
    if (number == 0) {
        return 1;
    }
    if (number < 0) {
        count++;
        number = -number;
    }
    while (number > 0) {
        count++;
        number /= 10;
    }
    return count;
}
