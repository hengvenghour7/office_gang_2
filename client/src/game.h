#include "character.h"
#include "world.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
typedef struct ServerPackage {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int socketfd;
    char buf[100];
} ServerPackage;
enum GameState {
    MainMenu,
    Pause,
    Playing,
    Shop,
};
typedef struct Game {
    bool isMultiplayer;
    int onlineID;
    ServerPackage serverPackage;
    Player player;
    World world;
    void (* tick)(void* self);
} Game;

Game createGame();
void gameTick(void* self);
void singlePlayerTick(void* self);
void initMultiplayer(Game* game);
void multiPlayerTick(Game* game);
void endMultiplayer(Game* game);
const char * getGameDataAsString(Game* game);