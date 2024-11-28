#pragma once

#include "../dependency/txt/txt.c"
#include "assets.c"
#include "gamechoose.c"

void renderScreenFromTxt(const char *screen_path)
{
    char screen_content[TXT_READ_SIZE_DEFAULT];

    txtRead(screen_path, screen_content, TXT_READ_SIZE_DEFAULT);
    printf("%s", screen_content);
}

void WelcomeScreen()
{
    renderScreenFromTxt(ASSET_PATH_SCREEN_WELCOME);
}

void GamechooseScreen()
{
    renderScreenFromTxt(ASSET_PATH_SCREEN_GAMECHOOSE);
    GamechooseList();
}