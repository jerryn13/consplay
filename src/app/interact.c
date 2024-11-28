#pragma once

#include <string.h>

#include "screens.c"
#include "game.c"
#include "gamechoose.c"
#include "llm.c"
#include "../dependency/string/string.c"

enum InteractState
{
    EXIT,
    WELCOME_SCREEN,
    GAME_CHOOSE_SCREEN,
    GAME_PLAY_SCREEN
};

int is_exit = false;
enum InteractState current_interact_state = WELCOME_SCREEN;
char user_input[STRING_DEFAULT_LENGTH] = "";
ConsplayGame current_game;
char current_game_path[PATHNAME_MAX_LENGTH] = "";

void interact()
{
    switch (current_interact_state)
    {
    case WELCOME_SCREEN:
        if (streq(user_input, "1"))
        {
            system("cls");
            current_interact_state = GAME_CHOOSE_SCREEN;
            GamechooseScreen();
        }
        if (streq(user_input, "2"))
        {
            system("cls");
            current_interact_state = EXIT;
            is_exit = true;
        }

        break;
    case GAME_CHOOSE_SCREEN:
        int index = atoi(user_input);

        if (index == 0)
        {
            current_interact_state = WELCOME_SCREEN;

            system("cls");
            WelcomeScreen();
            
            return;
        }

        if (index < 1 || index > game_file_count)
        {
            printf("输入的序号无效\n");
            return;
        }

        current_interact_state = GAME_PLAY_SCREEN;

        system("cls");

        strcpy(current_game_path, config_game_path);
        strcat(current_game_path, "\\");
        strcat(current_game_path, game_files[index - 1].name);
        strcat(current_game_path, ".");
        strcat(current_game_path, game_files[index - 1].extension);
        gameLoadFromFile(current_game_path, &current_game);

        printf("游戏简介\n");
        printf("名称: %s\n", current_game.name);
        printf("作者: %s\n", current_game.author);
        printf("版本: %s\n", current_game.version);
        printf("描述: %s\n", current_game.description);
        printf("————————————————————\n");
        printf("%s: %s", current_game.name, current_game.assistant_prompt);
        printf("\n");
        printf("————————————————————\n");
        printf("你: ");

        break;
    case GAME_PLAY_SCREEN:
        printf("————————————————————\n");
        printf("%s: ", current_game.name);
        LLMSubmit(ANSIToUTF8(user_input), current_game.system_prompt, current_game.assistant_prompt);
        break;
    case EXIT:
        break;
    };
}

void interactLoop()
{
    system("cls");
    WelcomeScreen();

    while (!is_exit)
    {
        scanf("%s", &user_input);
        interact();
    }
}
