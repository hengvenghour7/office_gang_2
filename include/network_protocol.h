#ifndef OFFICE_GANG_NETWORK_PROTOCOL_H
#define OFFICE_GANG_NETWORK_PROTOCOL_H

#include <stdint.h>

#define SERVER_PORT 5000
#define MAX_PLAYERS 16

#define MSG_PLAYER_UPDATE 1u
#define MSG_GAME_STATE 2u

typedef struct
{
    uint32_t type;
    float x;
    float y;
} PlayerUpdate;

typedef struct
{
    int32_t id;
    float x;
    float y;
} NetworkPlayer;

typedef struct
{
    uint32_t type;
    uint32_t player_count;
    int32_t your_id;
    NetworkPlayer players[MAX_PLAYERS];
} GameState;

#endif
