#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../dependency/string/string.c"
#include "../dependency/utils/utils.c"

#define MAX_FILES 10
#define MAX_FILENAME_LENGTH 256

typedef struct
{
    char name[MAX_FILENAME_LENGTH];
    char extension[MAX_FILENAME_LENGTH];
    char path[PATHNAME_MAX_LENGTH];
    char fullpath[PATHNAME_MAX_LENGTH];
} File;

// 目录、接收文件、接收数量
void filesList(const char *path, File files[], int *count)
{
    struct dirent *entry;
    DIR *dp = opendir(path);

    *count = 0;

    while ((entry = readdir(dp)) != NULL)
    {
        // 判断是否有扩展名
        char *dot = strrchr(entry->d_name, '.');

        if (dot && dot != entry->d_name)
        {
            strncpy(files[*count].name, entry->d_name, dot - entry->d_name);
            files[*count].name[dot - entry->d_name] = '\0';
            strncpy(files[*count].extension, dot + 1, MAX_FILENAME_LENGTH);
        }
        else
        {
            strncpy(files[*count].name, entry->d_name, MAX_FILENAME_LENGTH);
            files[*count].extension[0] = '\0';
        }

        (*count)++;
    }

    closedir(dp);
}
