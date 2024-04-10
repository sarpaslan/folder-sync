#include <stdio.h>
#include "dirent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
void sync_folder(char *path1, char *path2);
int copy_file(const char *filePath, const char *targetDir, const char *fileName);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please provide two paths as command line arguments.\n");
        return 1;
    }

    char *path1 = argv[1];
    char *path2 = argv[2];

    sync_folder(path1, path2);
    printf("completed");
    return 0;
}

void sync_folder(char *source, char *target)
{
    DIR *sourceDir = opendir(source);
    if (!sourceDir)
    {
        printf("Reading directory failed at source %s", source);
        return;
    }
    DIR *targetDir = opendir(target);
    if (!targetDir)
    {
        struct stat st = {0};
        if (stat(target, &st) == -1)
        {
            mkdir(target);
        }
        targetDir = opendir(target);
        if (targetDir)
        {
            // success
        }
        else
        {
            printf("Directory creation failed\n");
            return;
        }
    }

    struct dirent *entry;
    while ((entry = readdir(sourceDir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        char source_path[600];
        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);

        char dest_path[600];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", target, entry->d_name);

        struct stat source_stat, dest_stat;
        if (stat(source_path, &source_stat) != 0)
        {
            perror("Unable to get file status");
            continue;
        }
        if (S_ISDIR(entry->d_type))
        {
            sync_folder(source_path, dest_path);
        }
        else
        {
            int st = stat(dest_path, &dest_stat);
            if (st == 0)
            {
                if (difftime(source_stat.st_mtime, dest_stat.st_mtime) > 0)
                {
                    copy_file(source_path, target, entry->d_name);
                }
                else
                {
                    copy_file(dest_path, source, entry->d_name);
                }
            }
            else
            {
                copy_file(source_path, target, entry->d_name);
            }
        }
    }
}

int copy_file(const char *filePath, const char *targetDir, const char *fileName)
{
    FILE *source, *destination;
    char destinationFileName[600], ch;

    source = fopen(filePath, "r");
    if (source == NULL)
    {
        printf("Unable to open source file.\n");
        return -1;
    }

    snprintf(destinationFileName, sizeof(destinationFileName), "%s/%s", targetDir, fileName);

    destination = fopen(destinationFileName, "w");
    if (destination == NULL)
    {
        printf("Unable to create destination file.\n");
        fclose(source);
        return -1;
    }

    while ((ch = fgetc(source)) != EOF)
    {
        fputc(ch, destination);
    }

    fclose(source);
    fclose(destination);
    return 0;
}