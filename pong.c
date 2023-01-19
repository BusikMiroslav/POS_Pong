#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

#include "pong.h"

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


int hraj()
{
    srand(time(NULL)); // initiate randomness
    int ball_x = 20;
    int ball_y = 10;

    int paddle_right_y = 10;
    int paddle_left_y = 10;

    int max_y = 0;
    int max_x = 0;

    int ball_next_x = 0;
    int ball_next_y = 0;
    int ball_direction_x = 1;
    int ball_direction_y = 0;

    int skore_pravy = 0;
    int skore_lavy = 0;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);

    keypad(stdscr, TRUE);

    while (1) {
        getmaxyx(stdscr, max_y, max_x);
        switch (getch()) {
            case 'Q':
            case 'q':
                endwin();
                return 0;
            case 'w':
            case 'W':
                if ((paddle_left_y - RYCHLOST_PADU) > 4)
                    paddle_left_y -= RYCHLOST_PADU;
                else
                    paddle_left_y = VELKOST_PADU;
                break;
            case KEY_UP:
                if ((paddle_right_y - RYCHLOST_PADU) > 4)
                    paddle_right_y -= RYCHLOST_PADU;
                else
                    paddle_right_y = VELKOST_PADU;
                break;
            case 's':
            case 'S':
                if ((paddle_left_y + RYCHLOST_PADU) < max_y)
                    paddle_left_y += RYCHLOST_PADU;
                else
                    paddle_left_y = max_y - 1;
                break;
            case KEY_DOWN:
                if ((paddle_right_y + RYCHLOST_PADU) < max_y)
                    paddle_right_y += RYCHLOST_PADU;
                else
                    paddle_right_y = max_y - 1;
                break;
        }

        erase();
        zobrazenie_ohranicenia(max_x, max_y);
        aktualizuj_score(skore_lavy, skore_pravy);
        mvprintw(ball_y, ball_x, "o");
        zobrazenie_padu(VELKOST_PADU, paddle_left_y, paddle_right_y, max_x - 2);
        refresh();

        usleep(DELAY);

        ball_next_x = ball_x + ball_direction_x;
        ball_next_y = ball_y + ball_direction_y;

        // Odrazenie loptičky od padu
        for (int i = 0; i < VELKOST_PADU; ++i) {
            if (ball_y == paddle_right_y - i  && ball_next_x >= max_x - 2) {
                ball_direction_x *= -1;
                ball_direction_y = rand() % 3 - 1;
                break;
            } else if (ball_y == paddle_left_y - i  && ball_next_x <= 1) {
                ball_direction_x *= -1;
                ball_direction_y = rand() % 3 - 1;
                break;
            }
        }

        // Odrazenie od okrajov
        if (ball_next_x >= max_x || ball_next_x < 0)
            ball_direction_x *= -1;
        else
            ball_x += ball_direction_x;

        if (ball_next_y >= max_y - 1 || ball_next_y < 1)
            ball_direction_y *= -1;
        else
            ball_y += ball_direction_y;

        // Nechytenie loptičky, narazila o stenu za padom
        if (ball_next_x > max_x - 2) {
            skore_lavy += 1;
            sleep(1);
            if (skore_lavy == 10)
                break;
            ball_x = max_x/2;
            ball_y = 10;
            ball_direction_y = 0;
            ball_direction_x = 1;
        }

        if (ball_next_x < 1) {
            skore_pravy += 1;
            sleep(1);
            if (skore_pravy == 10)
                break;
            ball_x = max_x/2;
            ball_y = 10;
            ball_direction_y = 0;
            ball_direction_x = -1;
        }
    }

    mvprintw(8, max_x/2 - 7, "GAME OVER!");
    mvprintw(9, max_x/2 - 10, "press \'q\' to quit");
    refresh();
    while (getchar() != 'q');
    endwin();
}

