#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../dependency/string/string.c"
#include "../dependency/utils/utils.c"
#include "configs.c"
#include "files.c"
#include "game.c"

void filterGameFiles(File src_files[], int src_count, File recv_files[], int *recv_count)
{
    for (size_t i = 0; i < src_count; i++)
    {
        if (streq(src_files[i].extension, "cg"))
        {
            strncpy(recv_files[*recv_count].name, src_files[i].name, MAX_FILENAME_LENGTH);
            strncpy(recv_files[*recv_count].extension, src_files[i].extension, MAX_FILENAME_LENGTH);
            (*recv_count)++;
        }
    }
}

File game_files[MAX_FILES];
int game_file_count = 0;

void GamechooseList()
{
    File files[MAX_FILES];
    int file_count = 0;

    filesList(config_game_path, files, &file_count);
    filterGameFiles(files, file_count, game_files, &game_file_count);

    for (int i = 0; i < game_file_count; i++)
    {
        ConsplayGame _game = {};
        char _game_path[STRING_DEFAULT_LENGTH];

        strcpy(_game_path, config_game_path);
        strcat(_game_path, "\\");
        strcat(_game_path, game_files[i].name);
        strcat(_game_path, ".");
        strcat(_game_path, game_files[i].extension);
        gameLoadFromFile(_game_path, &_game);

        printf("%d.%s\n", i + 1, _game.name);
    }
}
