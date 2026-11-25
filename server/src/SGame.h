#include "SCharacter.h"

typedef struct SGame {
    SCharacter character;
} SGame;

const char* getGameDataAsString(SGame* game);