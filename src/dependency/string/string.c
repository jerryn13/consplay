#pragma once

#include "../boolean/boolean.c"
#include <string.h>
#include <windows.h>
#include <locale.h>

#define STRING_DEFAULT_LENGTH 1024

int streq(const char *str1, const char *str2)
{
    if (strcmp(str1, str2) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

wchar_t *ANSIToUnicode(const char *str)
{
    int len;
    wchar_t *result;
    len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    memset(result, 0, (len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, len);
    return result;
}

char *UnicodeToANSI(const wchar_t *str)
{
    char *result;
    int len;
    len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    result = (char *)malloc((len + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (len + 1));
    WideCharToMultiByte(CP_ACP, 0, str, -1, result, len, NULL, NULL);
    return result;
}

wchar_t *UTF8ToUnicode(const char *str)
{
    int len;
    wchar_t *result;
    len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    memset(result, 0, (len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, len);
    return result;
}

char *UnicodeToUTF8(const wchar_t *str)
{
    char *result;
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    result = (char *)malloc((len + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (len + 1));
    WideCharToMultiByte(CP_UTF8, 0, str, -1, result, len, NULL, NULL);
    return result;
}

char *ANSIToUTF8(const char *str)
{
    return UnicodeToUTF8(ANSIToUnicode(str));
}

char *UTF8ToANSI(const char *str)
{
    return UnicodeToANSI(UTF8ToUnicode(str));
}

int isStringUTF8Encoded(const char *str, size_t length)
{
    size_t i = 0;
    int nBytes = 0;
    unsigned char ch = 0;
    int bAllAscii = true; // 如果全部都是ASCII,说明不是UTF-8
    while (i < length)
    {
        ch = *(str + i);
        if ((ch & 0x80) != 0)
            bAllAscii = false;
        if (nBytes == 0)
        {
            if ((ch & 0x80) != 0)
            {
                while ((ch & 0x80) != 0)
                {
                    ch <<= 1;
                    nBytes++;
                }
                if ((nBytes < 2) || (nBytes > 6))
                {
                    return 0;
                }
                nBytes--;
            }
        }
        else
        {
            if ((ch & 0xc0) != 0x80)
            {
                return 0;
            }
            nBytes--;
        }
        i++;
    }
    if (bAllAscii)
        return false;
    return (nBytes == 0);
}