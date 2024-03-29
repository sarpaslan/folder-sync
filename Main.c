#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"
#include <sys/types.h>
#include <sys/stat.h>

#define RED "\033[31m"
#define RESET "\033[0m"


const int FILE_ID_T = 32768;
const int DIRECTORY_ID_T =  16384;


void sync_directories(char *f, char *f2);
int main(int argc, char **argv)
{

    if (argc != 3)
    {
        printf(RED "Invalid argument count!\n" RESET);
        printf("Usage: format /path/to/folder/ another/path/to/folder\n");
        return 1;
    }
    sync_directories(argv[1], argv[2]);
}


void sync_directories(char *path1, char *path2) {
    DIR *dir1 = opendir(path1);
    DIR *dir2 = opendir(path2);

    if (!dir1 || !dir2) {
        printf("Error: Unable to open directories.\n");
        exit(1);
    }
    struct dirent *en;
    while((en=readdir(dir1))!=NULL){
        struct dirent *rec;
        if(DIRECTORY_ID_T==en->d_type){
            if((abs(strcmp(en->d_name, "."))==0 || abs(strcmp(en->d_name,"..")) ==0))
            {
                continue;
            }
            printf("dir: %s",en->d_name);
        }
        else if(FILE_ID_T == en->d_type){
            printf("file: %d - %s\n",en->d_type,en->d_name);
        }
    }
    closedir(dir1);
    closedir(dir2);
}
