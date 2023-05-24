#include "comunication.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct players_info {
  int socket_c1;
  int socket_c2;
} PlayersInfo;

PlayersInfo *prepare_sockets_and_get_clients(char *IP, int port);
