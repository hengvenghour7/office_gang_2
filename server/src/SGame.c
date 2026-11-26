#include "SGame.h"
#include <stdio.h>
#include <string.h>
#include <network_protocol.h>

SGame createGame ()
{
    SGame game = {
        .allPlayers = {0},
    };
    int index = 0;
    while (index < MAX_PLAYERS)
    {
        game.allPlayers[index] = createCharacter(0, (SVector2){0, 0}, false);
        index++;
    }
    
    return game;
}
GameState getGameState(SGame* game, int your_id)
{
    GameState state = { .type = MSG_GAME_STATE, .your_id = your_id };
    for (int i = 0; i < MAX_PLAYERS; i++) if (game->allPlayers[i].isActive) {
        NetworkPlayer* p = &state.players[state.player_count++];
        p->id = game->allPlayers[i].onlineID;
        p->x = game->allPlayers[i].pos.x; p->y = game->allPlayers[i].pos.y;
    }
    return state;
}
void addPlayer(SGame* game, int onlineID)
{
    int i = 0;
    while (i < MAX_PLAYERS)
    {
        if (!game->allPlayers[i].isActive)
        {
            game->allPlayers[i] = createCharacter(onlineID, (SVector2){onlineID * 20, 0}, true);
            return;
        }
        i++;
    }
}
