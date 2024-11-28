#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dependency/string/string.c"
#include "../dependency/utils/utils.c"
#include "../dependency/ini/ini.c"

// 路径配置
char config_current_path[PATHNAME_MAX_LENGTH];
char config_game_path[PATHNAME_MAX_LENGTH];
char config_env_file[PATHNAME_MAX_LENGTH];

// LLM配置
char config_llm_endpoint[STRING_DEFAULT_LENGTH];
char config_llm_model[STRING_DEFAULT_LENGTH];
char config_llm_authorization[STRING_DEFAULT_LENGTH];

// 其他配置
int config_debug_mode = false;

void configsLoad()
{
    // 载入当前目录
    getCurrentPath(config_current_path, PATHNAME_MAX_LENGTH);

    // 载入游戏文件夹
    strcpy(config_game_path, config_current_path);
    strcat(config_game_path, "\\userdata\\games");

    // 载入环境变量文件
    strcpy(config_env_file, config_current_path);
    strcat(config_env_file, "\\userdata\\env.ini");

    // 载入LLM配置
    iniRead(config_env_file, "configs", "llm_endpoint", undefined, config_llm_endpoint, STRING_DEFAULT_LENGTH);
    iniRead(config_env_file, "configs", "llm_model", undefined, config_llm_model, STRING_DEFAULT_LENGTH);
    iniRead(config_env_file, "configs", "llm_authorization", undefined, config_llm_authorization, STRING_DEFAULT_LENGTH);

    // 载入其他配置
    char debug_mode[STRING_DEFAULT_LENGTH] = "";
    iniRead(config_env_file, "configs", "debug_mode", "false", debug_mode, STRING_DEFAULT_LENGTH);
    config_debug_mode = (streq(debug_mode, "true") ? true : false);

    printf("%s", config_game_path);
}