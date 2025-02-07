#include "frontend.h"

void windowSettings() {
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  keypad(stdscr, 1);
  start_color();
  init_pair(BACKGROUND_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(FRAME_PAIR, FRAME_COLOR, FRAME_COLOR);
  init_pair(FIG_N_I, FIG_N_I_COLOR, FIG_N_I_COLOR);
  init_pair(FIG_N_T, FIG_N_T_COLOR, FIG_N_T_COLOR);
  init_pair(FIG_N_L, FIG_N_L_COLOR, FIG_N_L_COLOR);
  init_pair(FIG_N_R, FIG_N_R_COLOR, FIG_N_R_COLOR);
  init_pair(FIG_N_Z, FIG_N_Z_COLOR, FIG_N_Z_COLOR);
  init_pair(FIG_N_S, FIG_N_S_COLOR, FIG_N_S_COLOR);
  init_pair(FIG_N_O, FIG_N_O_COLOR, FIG_N_O_COLOR);
}

int getPlayer(UserAction_t *action) {
  int res = 1;
  int c = getch();
  switch (c) {
    case ENTER_BUTTON:
    case KEY_ENTER:
      *action = Start;
      break;
    case 'p':
      *action = Pause;
      break;
    case ESC_BUTTON:
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
      *action = Up;
      break;
    case ' ':
      *action = Action;
      break;
    default:
      res = 0;
      break;
  }
  return res;
}

void printField(GameInfo_t tetris) {
  if (tetris.field != S21_NULL) {
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIGHT; j++) {
        attron(COLOR_PAIR(tetris.field[i][j]));
        mvprintw(i + 1, 2 * (j + 1), "  ");
        attroff(COLOR_PAIR(tetris.field[i][j]));
      }
    }
  }
  move(HEIGHT, WIGHT * 10);
}

void printFrame() {
  attron(COLOR_PAIR(FRAME_PAIR));
  for (int i = 0; i < HEIGHT + 2; i++) {
    mvprintw(i, 0, "  ");
    mvprintw(i, (WIGHT + 1) * 2, "  ");
  }
  for (int j = 0; j < WIGHT + 2; j++) {
    mvprintw(0, j * 2, "  ");
    mvprintw(HEIGHT + 1, j * 2, "  ");
  }
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < SIZE_F + 2; j++) {
      mvprintw(i, (WIGHT + j + 1) * 2, "  ");
    }
  }
  attroff(COLOR_PAIR(FRAME_PAIR));
  move(HEIGHT, WIGHT * 10);
}

void printFilling() {
  attron(COLOR_PAIR(FRAME_PAIR));
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIGHT + SIZE_F + 2; j++) {
      mvprintw(i, (j + 1) * 2, "  ");
    }
  }
  attroff(COLOR_PAIR(FRAME_PAIR));
}

void printStat(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  mvprintw(SIZE_F + 5, 2 * (WIGHT + 3), "score:");
  mvprintw(SIZE_F + 6, 2 * (WIGHT + 3), "%5d ", tetris.score);
  mvprintw(SIZE_F + 9, 2 * (WIGHT + 3), " max: ");
  mvprintw(SIZE_F + 10, 2 * (WIGHT + 3), "%5d ", tetris.highScore);
  mvprintw(SIZE_F + 13, 2 * (WIGHT + 3), "level:");
  mvprintw(SIZE_F + 14, 2 * (WIGHT + 3), "  %2d  ", tetris.level);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT, WIGHT * 10);
}

void printNext(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  mvprintw(2, 2 * (WIGHT + 2), "   next:  ");
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  if (tetris.next != S21_NULL) {
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        attron(COLOR_PAIR(tetris.next[i][j]));
        mvprintw(i + 3, 2 * (j + WIGHT + 2), "  ");
        attroff(COLOR_PAIR(tetris.next[i][j]));
      }
    }
  }
  move(HEIGHT, WIGHT * 10);
}

void printPause() {
  attron(COLOR_PAIR(FRAME_PAIR));
  mvprintw(HEIGHT / 2 - 1, WIGHT - 1, "       ");
  mvprintw(HEIGHT / 2, WIGHT - 1, "       ");
  mvprintw(HEIGHT / 2 + 1, WIGHT - 1, "       ");
  attroff(COLOR_PAIR(FRAME_PAIR));
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  mvprintw(HEIGHT / 2, WIGHT, "PAUSE");
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT, WIGHT * 10);
}

void printGameOver(GameInfo_t tetris) {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  mvprintw(SIZE_F, WIGHT + 2, "           ");
  mvprintw(SIZE_F + 1, WIGHT + 2, " GAME OVER ");
  mvprintw(SIZE_F + 2, WIGHT + 2, "           ");
  mvprintw(SIZE_F + 5, WIGHT + 4, "score:");
  mvprintw(SIZE_F + 6, WIGHT + 4, "%5d ", tetris.score);
  mvprintw(SIZE_F + 9, WIGHT + 4, " max: ");
  mvprintw(SIZE_F + 10, WIGHT + 4, "%5d ", tetris.highScore);
  mvprintw(SIZE_F + 13, WIGHT + 4, "level:");
  mvprintw(SIZE_F + 14, WIGHT + 4, "  %2d  ", tetris.level);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT, WIGHT * 10);
}

void printStart() {
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  mvprintw(HEIGHT / 2 - 2, WIGHT - 2, "                     ");
  mvprintw(HEIGHT / 2 - 1, WIGHT - 2, "  to start NEW GAME  ");
  mvprintw(HEIGHT / 2 - 0, WIGHT - 2, "     press enter     ");
  mvprintw(HEIGHT / 2 + 1, WIGHT - 2, "                     ");
  move(HEIGHT, WIGHT * 10);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));
  move(HEIGHT, WIGHT * 10);
}

void printGame(GameInfo_t tetris) {
  if (tetris.field != S21_NULL && tetris.next != S21_NULL && !tetris.pause) {
    printFrame();
    printField(tetris);
    printNext(tetris);
    printStat(tetris);
  } else if (tetris.pause) {
    printFrame();
    printNext(tetris);
    printPause();
    printStat(tetris);
  } else if (tetris.field != S21_NULL && tetris.next == S21_NULL) {
    printFilling();
    printGameOver(tetris);
  } else if (tetris.field == S21_NULL && tetris.next != S21_NULL) {
    printFilling();
    printStart();
  }
  refresh();
}