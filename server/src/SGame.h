#ifndef SGAME_H
#define SGAME_H

#include "SCharacter.h"

#define MAX_PLAYERS 20
typedef struct SGame {
    SCharacter allPlayers[MAX_PLAYERS];
} SGame;
SGame createGame();
void addSPlayer(SGame* game, int onlineID);
char* getGameDataAsString(SGame* game);
void updateSPlayers(SGame* game, int onlineID, int x, int y);

#endif