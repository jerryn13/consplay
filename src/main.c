#include <stdio.h>
#include <windows.h>

#include "app/interact.c"
#include "app/configs.c"

int main()
{
    // 设置cmd活动代码页为UTF-8，防止中文乱码
    SetConsoleOutputCP(65001);

    // 载入配置项
    configsLoad();

    // 程序主循环
    interactLoop();

    return 0;
}