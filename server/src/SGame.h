#ifndef SGAME_H
#define SGAME_H

#include "SCharacter.h"

#define MAX_PLAYERS 20
typedef struct SGame {
    SCharacter allPlayers[MAX_PLAYERS];
} SGame;
SGame createGame();
void addPlayer(SGame* game, int onlineID);
char* getGameDataAsString(SGame* game);

#endif