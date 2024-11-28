#include <stdio.h>
#include <string.h>
#include "../../dependency/utils/utils.c"
#include "../../dependency/ini/ini.c"

int main()
{
    char cwd[1024];
    getCurrentPath(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);


    char ini_filename[1024];
    getCurrentPath(ini_filename, sizeof(ini_filename));
    strcat(ini_filename, "\\test.ini");
    printf("INI path: %s\n", ini_filename);


    printf("\n----ini read test----\n");
    char testKeyName_value[2048];
    char testChineseKeyName_value[2048];
    iniRead(ini_filename, "testSectionName", "testKeyName", "undefined", testKeyName_value, sizeof(testKeyName_value));
    iniRead(ini_filename, "testSectionName", "测试中文键名", "undefined", testChineseKeyName_value, sizeof(testChineseKeyName_value));
    printf("testKeyName_value: %s\n", testKeyName_value);
    printf("testChineseKeyName_value: %s\n", testChineseKeyName_value);
    

    printf("\n----ini write test----\n");


    return 0;
}