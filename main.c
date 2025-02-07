/**
 * \file
 * \brief main starting file
 */
#include "brick_game/tetris/tetris.h"
#include "gui/cli/frontend.h"

/**
 * \brief Entry point of the program
 *
 * in main we initialize front and back and then in a `while` display game.
 */
int main() {
#ifdef LOGS
  FILE *fp = fopen(FILE_NAME_FOR_LOGS, "w");
  if (fp) {
    fclose(fp);
  }
#endif  // LOGS

  windowSettings();

  // init game, action struct
  GameInfo_t tetris = updateCurrentState();
  UserAction_t action = {0};
  bool hold = 0;

  printGame(tetris);

  while (tetris.field || tetris.next) {
    if (getPlayer(&action)) {
      userInput(action, hold);
    }
    tetris = updateCurrentState();
    printGame(tetris);
  }
  endwin();
  return 0;
}