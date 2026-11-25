#ifndef SGAME_H
#define SGAME_H

#include "SCharacter.h"
#include "../../include/network_protocol.h"

typedef struct SGame {
    SCharacter allPlayers[MAX_PLAYERS];
} SGame;
SGame createGame();
void addPlayer(SGame* game, int onlineID);
GameState getGameState(SGame* game, int your_id);

#endif
