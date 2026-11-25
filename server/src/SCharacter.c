#include "SCharacter.h"
#include <stdio.h>
#include <stdlib.h>

SCharacter createCharacter()
{
    SCharacter character = {
        .pos = (SVector2){0, 0},
    };
    return character;
};
char* getCharacterDataAsString(SCharacter* character)
{
    char* stringData = malloc(100);
    stringData = "";
    stringData += character->pos.x;
    return stringData;
}
