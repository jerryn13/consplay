#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../boolean/boolean.c"

#define undefined "undefined"

#define PATHNAME_MAX_LENGTH 512

int getCurrentPath(char *str, int str_size)
{
    getcwd(str, str_size);
}