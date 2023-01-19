#include "pong.h"


void zobraz_uvod() {
    mvprintw(1,0,"      oooooooooo                                        \n"
                 "      ooo    ooo  ooooooo   oooooooo    oooooooo       \n"
                 "      ooooooooo ooo     ooo ooo   ooo  ooo     oo       \n"
                 "      ooo       ooo     ooo ooo   ooo   ooooooooo       \n"
                 "     ooooo        ooooooo  ooooo ooooo         ooo      \n"
                 "                                        ooooooooo     \n\n");
    mvprintw(8, 1, "Vitajte v hre Pong");
    mvprintw(10, 1, "Ovladanie padu je pomocou klaves w/W a s/S");
    mvprintw(12, 1, "Pre ukoncenie hry stlacte q/Q");
    mvprintw(14, 1, "Hru vyhra ten, kto prvy ziska 7 bodov");
    mvprintw(16, 1, "Uzite si nasu hru");
}


void zobrazenie_padu(int size, int y1, int y2, int x2)
{
    for (int i = 0; i < size; ++i) {
        mvprintw(y2 - i, x2, "|");
        mvprintw(y1 - i, 1, "|");
    }
}


void zobrazenie_ohranicenia(int sirka, int vyska)
{
    for (int i = 0; i < sirka; ++i) {
        mvprintw(0, i, "*");
        mvprintw(vyska - 1, i, "*");
    }
}


void aktualizuj_score(int prvy, int druhy)
{
    char prvy_hrac[2];
    char druhy_hrac[2];

    sprintf(prvy_hrac, "%d", prvy);
    sprintf(druhy_hrac, "%d", druhy);
    mvprintw(1, 2, "HRAC 1:");
    mvprintw(1, 10, prvy_hrac);
    mvprintw(1, 172, "HRAC 2:");
    mvprintw(1, 180, druhy_hrac);
}


void* hraj(void* gdata) {
    srand(time(NULL));
    ThrData* data = (ThrData*) gdata;
    srand(time(0));

    int iteracia = 0;
    int max_y = 0;
    int max_x = 0;
    char buffer[BUFFER_LENGTH];

    sleep(10);
    while (data->ukoncenie == 0) {
        if (data->skore_pravy >= 7 || data->skore_lavy >= 7) {
            pthread_mutex_lock(data->mutex);
            data->fazaHry = 1;
            pthread_mutex_unlock(data->mutex);
        }

        int ball_next_x = data->ball_x + data->ball_direction_x;
        int ball_next_y = data->ball_y + data->ball_direction_y;

        // Odrazenie loptičky od padu
        for (int i = 0; i < VELKOST_PADU; ++i) {
            if (data->ball_y == data->pad_druhy_y - i  && ball_next_x >= max_x - 2) {
                pthread_mutex_lock(data->mutex);
                data->ball_direction_x *= -1;
                data->ball_direction_y = rand() % 3 - 1;
                pthread_mutex_unlock(data->mutex);
                break;
            } else if (data->ball_y == data->pad_prvy_y - i  && ball_next_x <= 1) {
                pthread_mutex_lock(data->mutex);
                data->ball_direction_x *= -1;
                data->ball_direction_y = rand() % 3 - 1;
                pthread_mutex_unlock(data->mutex);
                break;
            }
        }

        // Odrazenie od okrajov
        if (ball_next_x >= max_x || ball_next_x < 0) {
            pthread_mutex_lock(data->mutex);
            data->ball_direction_x *= -1;
            pthread_mutex_unlock(data->mutex);
        } else {
            pthread_mutex_lock(data->mutex);
            data->ball_x += data->ball_direction_x;
            pthread_mutex_unlock(data->mutex);
        }

        if (ball_next_y >= max_y - 1 || ball_next_y < 1) {
            pthread_mutex_lock(data->mutex);
            data->ball_direction_y *= -1;
            pthread_mutex_unlock(data->mutex);
        } else {
            pthread_mutex_lock(data->mutex);
            data->ball_y += data->ball_direction_y;
            pthread_mutex_unlock(data->mutex);
        }

        getmaxyx(stdscr, max_y, max_x);
        // Nechytenie loptičky, narazila o stenu za padom
        if (ball_next_x > max_x - 2 && max_x != -1) {
            pthread_mutex_lock(data->mutex);
            data->skore_lavy += 1;
            pthread_mutex_unlock(data->mutex);
            sleep(1);
            if (data->skore_lavy == 10)
                break;
            pthread_mutex_lock(data->mutex);
            data->ball_x = max_x/2;
            data->ball_y = (rand() % (max_y - 6) + 4);
            data->ball_direction_y = 0;
            data->ball_direction_x = 1;
            pthread_mutex_unlock(data->mutex);
        }

        if (ball_next_x < 1) {
            pthread_mutex_lock(data->mutex);
            if (iteracia != 0)
                data->skore_pravy += 1;
            pthread_mutex_unlock(data->mutex);
            iteracia++;
            sleep(1);
            if (data->skore_pravy == 10)
                break;
            pthread_mutex_lock(data->mutex);
            data->ball_x = max_x/2;
            data->ball_y = (rand() % (max_y - 6) + 4);
            data->ball_direction_y = 0;
            data->ball_direction_x = -1;
            pthread_mutex_unlock(data->mutex);
        }

        if (data->ukoncenie == 0) {
            sprintf(buffer,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",data->ball_x, data->ball_y, data->ball_direction_x, data->ball_direction_y, data->pad_prvy_y, data->pad_druhy_y, data->skore_pravy, data->skore_lavy, data->socket_pravy, data->socket_lavy, data->jePrvy, data->fazaHry, data->ukoncenie);
            write(data->socket_lavy, buffer, strlen(buffer) + 1);
            write(data->socket_pravy, buffer, strlen(buffer) + 1);
        }

        usleep(DELAY);
    }
    sprintf(buffer,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",data->ball_x, data->ball_y, data->ball_direction_x, data->ball_direction_y, data->pad_prvy_y, data->pad_druhy_y, data->skore_pravy, data->skore_lavy, data->socket_pravy, data->socket_lavy, data->jePrvy, data->fazaHry, data->ukoncenie);
    write(data->socket_lavy, buffer, strlen(buffer) + 1);
    write(data->socket_pravy, buffer, strlen(buffer) + 1);
}


void* zobrazHru(void* gdata) {
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    erase();
    zobraz_uvod();
    refresh();
    sleep(10);

    int max_y = 0;
    int max_x = 0;
    int koniec = 0;
    int koniec1 = 0;
    KlientData* data = (KlientData*) gdata;
    ThrData nove_data;

    while (koniec == 0 && koniec1 == 0) {
        erase();
        char buffer[BUFFER_LENGTH];
        read(data->socket, buffer, BUFFER_LENGTH);
        parsujData(buffer, &nove_data);

        getmaxyx(stdscr, max_y, max_x);
        koniec1 = nove_data.ukoncenie;
        koniec = nove_data.fazaHry;

        zobrazenie_ohranicenia(max_x, max_y);
        aktualizuj_score(nove_data.skore_lavy, nove_data.skore_pravy);
        mvprintw(nove_data.ball_y, nove_data.ball_x, "o");
        zobrazenie_padu(VELKOST_PADU, nove_data.pad_prvy_y, nove_data.pad_druhy_y, max_x - 2);
        refresh();
    }

    zobraz_koniec(data->socket_number, &nove_data, max_x);
    char tlacidlo;
    tlacidlo = getch();
    while (tlacidlo != 'Q' || tlacidlo != 'q') {
        tlacidlo = getch();
    }

    endwin();
}


void zobraz_koniec(int cislo_vlakna, ThrData* nove_data, int max_x) {
    erase();
    mvprintw(8, max_x/2 - 5, "Koniec hry");
    if (cislo_vlakna == 1) {
        if (nove_data->skore_lavy > nove_data->skore_pravy)
            mvprintw(10, max_x / 2 - 5, "Vyhral si");
        else if (nove_data->skore_lavy == nove_data->skore_pravy)
            mvprintw(10, max_x / 2 - 3, "Remíza");
        else
            mvprintw(10, max_x/2 - 5, "Prehral si");
    } else {
        if (nove_data->skore_pravy > nove_data->skore_lavy)
            mvprintw(10, max_x/2 - 5, "Vyhral si");
        else if (nove_data->skore_lavy == nove_data->skore_pravy)
            mvprintw(10, max_x / 2 - 3, "Remíza");
        else
            mvprintw(10, max_x/2 - 5, "Prehral si");
    }
    mvprintw(12, max_x/2 - 15, "Pre ukoncenie stlac klavesu q/Q");
    refresh();
}

void* prepocetPozicii(void* gdata) {
    ThrData* data = (ThrData*) gdata;
    char buffer[BUFFER_LENGTH];
    int max_y = 0;
    int max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    bool jePrvy;

    jePrvy = data->jePrvy == 1 ? true : false;

    while(data->ukoncenie == 0) {
        if(jePrvy) {
            read(data->socket_lavy, buffer, BUFFER_LENGTH);

            switch (buffer[0]) {
                case 'w':
                case 'W':
                    if ((data->pad_prvy_y - RYCHLOST_PADU) > 4) {
                        pthread_mutex_lock(data->mutex);
                        data->pad_prvy_y -= RYCHLOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    } else {
                        pthread_mutex_lock(data->mutex);
                        data->pad_prvy_y = VELKOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    }
                    break;
                case 's':
                case 'S':
                    if ((data->pad_prvy_y + RYCHLOST_PADU) < max_y) {
                        pthread_mutex_lock(data->mutex);
                        data->pad_prvy_y += RYCHLOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    } else {
                        pthread_mutex_lock(data->mutex);
                        data->pad_prvy_y = max_y - 1;
                        pthread_mutex_unlock(data->mutex);
                    }
                    break;
                case 'q':
                case 'Q':
                    pthread_mutex_lock(data->mutex);
                    data->ukoncenie = 1;
                    pthread_mutex_unlock(data->mutex);
                    break;
            }
        } else {
            read(data->socket_pravy, buffer, BUFFER_LENGTH);

            switch (buffer[0]) {
                case 'w':
                case 'W':
                    if ((data->pad_druhy_y - RYCHLOST_PADU) > 4) {
                        pthread_mutex_lock(data->mutex);
                        data->pad_druhy_y -= RYCHLOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    } else {
                        pthread_mutex_lock(data->mutex);
                        data->pad_druhy_y = VELKOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    }
                    break;
                case 's':
                case 'S':
                    if ((data->pad_druhy_y + RYCHLOST_PADU) < max_y) {
                        pthread_mutex_lock(data->mutex);
                        data->pad_druhy_y += RYCHLOST_PADU;
                        pthread_mutex_unlock(data->mutex);
                    } else {
                        pthread_mutex_lock(data->mutex);
                        data->pad_druhy_y = max_y - 1;
                        pthread_mutex_unlock(data->mutex);
                    }
                    break;
                case 'q':
                case 'Q':
                    pthread_mutex_lock(data->mutex);
                    data->ukoncenie = 1;
                    pthread_mutex_unlock(data->mutex);
                    break;
            }
        }
    }
}


void parsujData(char* buffer, ThrData* data) {
    char* token;
    token = strtok(buffer, ",");
    int i = 0;

    while( token != NULL ) {
        i++;
        if( i == 1)
            data->ball_x = atoi(token);
        if( i == 2)
            data->ball_y = atoi(token);
        if( i == 5)
            data->pad_prvy_y = atoi(token);
        if( i == 6)
            data->pad_druhy_y = atoi(token);
        if( i == 7)
            data->skore_pravy = atoi(token);
        if( i == 8)
            data->skore_lavy = atoi(token);
        if( i == 12)
            data->fazaHry = atoi(token);
        if( i == 13)
            data->ukoncenie = atoi(token);
        token = strtok(NULL, ",");
    }
}

