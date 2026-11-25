#include "SGame.h"
#include <networkHelpers.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SGame createGame ()
{
    SGame game = {
        .allPlayers = {0},
    };
    int index = 0;
    while (index < MAX_PLAYERS)
    {
        game.allPlayers[index] = createCharacter(0, (SVector2){0, 0});
        index++;
    }
    
    return game;
}
char* getGameDataAsString(SGame* game)
{
    char* s = malloc(2048);
    s[0] = '\0';
    int index = 0;
    while (index < MAX_PLAYERS)
    {
        SCharacter* p = &game->allPlayers[index];
        addIntToString(s, p->onlineID);
        strcat(s, ",");
        addIntToString(s, p->pos.x);
        strcat(s, ",");
        addIntToString(s, p->pos.y);
        strcat(s, ",");
        index++;
    }
    return s;
}