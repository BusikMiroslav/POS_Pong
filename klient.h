#ifndef UNTITLED_KLIENT_H
#define UNTITLED_KLIENT_H
#include "k_s_definitions.h"
#include "pong.h"
#include "k_s_definitions.h"
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <curses.h>

typedef struct KlientData {
    int socket;
    int socket_number;
}KlientData;

int klient_main(int argc, char *argv[]);

#endif //UNTITLED_KLIENT_H
