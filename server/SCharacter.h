#ifndef SCHARACTER_H
#define SCHARACTER_H

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
    int posX;
} SCharacter;

#endif