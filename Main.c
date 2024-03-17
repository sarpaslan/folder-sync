#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "dirent.h"

#define RED "\033[31m"
#define RESET "\033[0m"
void sync_folder(char *f, char *f2);
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf(RED "Invalid argument count!\n" RESET);
        printf("Usage: format /path/to/folder/ another/path/to/folder\n");
        return 1;
    }

    sync_folder(argv[1], argv[2]);
}

void sync_folder(char *path1, char *path2)
{
    DIR *folder1 = opendir(path1);
    DIR *folder2 = opendir(path2);
    if (folder1 == NULL || folder2 == NULL)
    {
        perror("Unable to read directories");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int files = 0;
    while ((entry = readdir(folder1)))
    {
        printf("File %3d: %s\n", files, entry->d_name);

        char target_file[PATH_MAX];
        snprintf(target_file, sizeof(target_file),
                 "%s/%s", folder1, entry->d_name);

        if (S_ISDIR(entry->d_type))
            continue;
        printf("snprintf: %s\n", target_file);
        struct stat fileStat;
        if (stat(target_file, &fileStat) == -1)
        {
            printf("Failed to get file status%s", target_file);
            exit(EXIT_FAILURE);
        }
        if (S_ISREG(fileStat.st_mode))
        {
            printf("File %s,  last modified time %s", entry->d_name, ctime(&fileStat.st_mtime));
        }
    }
    closedir(folder1);
    closedir(folder2);
}
