#ifndef UNTITLED_PONG_H
#define UNTITLED_PONG_H

#include <stdbool.h>
#include "pthread.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <string.h>

#include "klient.h"
#include "server.h"

#define DELAY 30000
#define VELKOST_PADU 5
#define RYCHLOST_PADU 4

typedef struct ThrData {
    int ball_x;
    int ball_y;

    int ball_direction_x;
    int ball_direction_y;

    int pad_prvy_y;
    int pad_druhy_y;

    int skore_pravy;
    int skore_lavy;

    int socket_pravy;
    int socket_lavy;

    int jePrvy;
    int fazaHry;           // 0 - nič sa nemení, 1 - koniec hry

    int ukoncenie;

    pthread_mutex_t* mutex;
}ThrData;

void zobraz_uvod();
void zobrazenie_padu(int size, int y1, int y2, int x2);
void zobrazenie_ohranicenia(int sirka, int vyska);
void* hraj(void* gdata);
void* zobrazHru(void* gdata);
void zobraz_koniec(int cislo_vlakna, ThrData* nove_data, int max_x);
void* prepocetPozicii(void* gdata);
void parsujData(char* buffer, ThrData* data);

#endif //UNTITLED_PONG_H
