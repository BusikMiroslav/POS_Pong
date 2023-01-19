#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include "pong.h"
#include "k_s_definitions.h"
#include "pong.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int server_main(int argc, char** argv);

#endif //UNTITLED_SERVER_H
