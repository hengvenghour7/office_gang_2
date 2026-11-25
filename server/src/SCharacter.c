#include "SCharacter.h"
#include <stdio.h>
#include <stdlib.h>

SCharacter createCharacter(int onlineID, SVector2 pos, bool isActive)
{
    SCharacter character = {
        .onlineID = onlineID,
        .isActive = isActive,
        .pos = pos,
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
