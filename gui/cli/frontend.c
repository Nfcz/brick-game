#ifndef TETRIS_FRONTEND_C
#define TETRIS_FRONTEND_C

#include "frontend.h"

void print_field(GameInfo_t tetris) {
  if (tetris.field != S21_NULL) {
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIGHT; j++) {
        attron(COLOR_PAIR(tetris.field[i][j]));
        mvaddch(i, 2 * j, ' ');
        mvaddch(i, 2 * j + 1, ' ');
        attroff(COLOR_PAIR(tetris.field[i][j]));
      }
    }
  }
}

void print_stat(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  move(SIZE_F + 1, 2 * WIGHT);
  printw("score:%d", tetris.score);
  move(SIZE_F + 2, 2 * WIGHT);
  printw("high score:%d", tetris.high_score);
  move(SIZE_F + 3, 2 * WIGHT);
  printw("level:%d", tetris.level);
  move(SIZE_F + 4, 2 * WIGHT);
  printw("not paused");
  move(HEIGHT, WIGHT * 10);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
}

void print_next(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  move(0, 2 * WIGHT);
  printw("next\n");
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  if (tetris.next != S21_NULL) {
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        attron(COLOR_PAIR(tetris.next[i][j]));
        mvaddch(i + 1, 2 * (j + WIGHT), ' ');
        mvaddch(i + 1, 2 * (j + WIGHT) + 1, ' ');
        attroff(COLOR_PAIR(tetris.next[i][j]));
      }
    }
  }
}

void print_pause(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT / 2 - 2, WIGHT - 3);
  printw("PAUSE\n");
  move(HEIGHT / 2, WIGHT - 5);
  printw("score:%d", tetris.score);
  move(HEIGHT / 2 + 1, WIGHT - 5);
  printw("max score:%d", tetris.high_score);
  move(HEIGHT / 2 + 2, WIGHT - 5);
  printw("level:%d", tetris.level);
  move(HEIGHT, WIGHT * 10);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
}

void print_gameover(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT / 2 - 2, WIGHT + 1);
  printw("GAMEOVER\n");
  move(HEIGHT / 2 + 1, WIGHT - 4);
  printw("score:\t\t%d", tetris.score);
  move(HEIGHT / 2 + 2, WIGHT - 4);
  printw("max score:\t%d", tetris.high_score);
  move(HEIGHT, WIGHT * 10);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
}

void print_start() {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT / 2 - 2, WIGHT - 3);
  printw("to start NEW GAME\n");
  move(HEIGHT / 2 - 1, WIGHT + 1);
  printw("press enter\n");
  move(HEIGHT, WIGHT * 10);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
}

void print_game(GameInfo_t tetris) {
  clear();
  if (tetris.field != S21_NULL && tetris.next != S21_NULL && !tetris.pause) {
    print_field(tetris);
    print_next(tetris);
    print_stat(tetris);
  } else if (tetris.pause) {
    print_pause(tetris);
  } else if (tetris.field != S21_NULL && tetris.next == S21_NULL) {
    print_gameover(tetris);
  } else if (tetris.field == S21_NULL && tetris.next != S21_NULL) {
    print_start();
  }

  refresh();
}

int get_player(UserAction_t *action) {
  int res = 1;
  int c = getch();
  // if (c != -1) {
  //   FILE *fp = fopen(FILE_NAME_FOR_LOGS, "a");
  //   fprintf(fp, "%i\t", c);
  //   fclose(fp);
  // }
  switch (c) {
    case 10:
    case KEY_ENTER:
      *action = Start;
      break;
    case ' ':
    case 'p':
      *action = Pause;
      break;
    case 27:
    case 'q':
    case KEY_BREAK:
      *action = Terminate;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_UP:
      *action = Action;
      break;
    default:
      res = 0;
      break;
  }
  return res;
}

int now_is_tick_time(struct timespec *t_previous, int speed) {
  int res = 0;
  struct timespec t_now = {0};
  clock_gettime(CLOCK_MONOTONIC, &t_now);
  if (t_now.tv_sec - t_previous->tv_sec > 0 ||
      t_now.tv_nsec - t_previous->tv_nsec > 10000000000 / (speed + 1)) {
    res = 1;
    t_previous->tv_nsec = t_now.tv_nsec;
    t_previous->tv_sec = t_now.tv_sec;
  }
  return res;
}

void window_settings() {
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  keypad(stdscr, 1);
  start_color();
  init_pair(BACKGROUND_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(FIG_N_I, FIG_N_I_PAIR, FIG_N_I_PAIR);
  init_pair(FIG_N_T, FIG_N_T_PAIR, FIG_N_T_PAIR);
  init_pair(FIG_N_L, FIG_N_L_PAIR, FIG_N_L_PAIR);
  init_pair(FIG_N_R, FIG_N_R_PAIR, FIG_N_R_PAIR);
  init_pair(FIG_N_Z, FIG_N_Z_PAIR, FIG_N_Z_PAIR);
  init_pair(FIG_N_S, FIG_N_S_PAIR, FIG_N_S_PAIR);
  init_pair(FIG_N_O, FIG_N_O_PAIR, FIG_N_O_PAIR);
}

#endif