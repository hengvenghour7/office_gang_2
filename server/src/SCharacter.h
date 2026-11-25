#ifndef SCHARACTER_H
#define SCHARACTER_H

#include <stdbool.h>
#include "SUtilities/SUtilities.h"

enum SCharacterState {
    Attacking,
    Idle,
    Walking,
    Running,
};
enum SCharacterDirection {
    Left,
    Right,
    Up,
    Down,
};
typedef struct SCharacter {
    int onlineID;
    bool isActive;
    SVector2 pos;
    enum SCharacterState characterState;
    enum SCharacterDirection characterDirection;
} SCharacter;

SCharacter createCharacter(int onlineID, SVector2 pos, bool isActive);

#endif
