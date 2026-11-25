#include "SHelpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addIntToString(char* desString, int copyNum)
{
    char* temp = malloc(50);
    sprintf(temp, "%d", copyNum);
    strcat(desString, temp);
};