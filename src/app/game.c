#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dependency/utils/utils.c"
#include "../dependency/string/string.c"
#include "../dependency/ini/ini.c"
#include "files.c"

typedef struct
{
    char package_name[STRING_DEFAULT_LENGTH];
    char schema[STRING_DEFAULT_LENGTH];
    char name[STRING_DEFAULT_LENGTH];
    char version[STRING_DEFAULT_LENGTH];
    char description[STRING_DEFAULT_LENGTH];
    char system_prompt[STRING_DEFAULT_LENGTH];
    char assistant_prompt[STRING_DEFAULT_LENGTH];
    char author[STRING_DEFAULT_LENGTH];
    char author_homepage[STRING_DEFAULT_LENGTH];
} ConsplayGame;

void gameLoadFromFile(char filename[], ConsplayGame *recv_game)
{
    strcpy(recv_game->package_name, filename);
    iniRead(filename, "game", "schema", undefined, recv_game->schema, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "name", undefined, recv_game->name, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "version", undefined, recv_game->version, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "description", undefined, recv_game->description, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "system_prompt", undefined, recv_game->system_prompt, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "assistant_prompt", undefined, recv_game->assistant_prompt, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "author", undefined, recv_game->author, STRING_DEFAULT_LENGTH);
    iniRead(filename, "game", "author_homepage", undefined, recv_game->author_homepage, STRING_DEFAULT_LENGTH);
}