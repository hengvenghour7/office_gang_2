#include "SCharacter.h"

SCharacter createCharacter(int onlineID, SVector2 pos, bool isActive)
{
    SCharacter character = {
        .onlineID = onlineID,
        .isActive = isActive,
        .pos = pos,
    };
    return character;
}
