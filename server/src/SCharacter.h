#ifndef SCHARACTER_H
#define SCHARACTER_H

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
    int id;
    SVector2 pos;
    enum SCharacterState characterState;
    enum SCharacterDirection characterDirection;
    char* (*getCharacterDataAsString)(struct SCharacter* character);
} SCharacter;

char* getCharacterDataAsString(SCharacter* character);
SCharacter createCharacter();

#endif