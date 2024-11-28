#pragma once

#include <windows.h>
#include <stdio.h>

void iniRead(const char *filename, const char *section, const char *key, const char *default_value, char *recv, int recv_size)
{
    // GetPrivateProfileString 参数: section, key, 默认值, 缓冲区, 缓冲区大小, 文件名
    GetPrivateProfileString(section, key, default_value, recv, recv_size, filename);
}

int iniWrite(const char *filename, const char *section, const char *key, const char *value)
{
    // WritePrivateProfileString 参数: section, key, 值, 文件名
    return WritePrivateProfileString(section, key, value, filename);
}