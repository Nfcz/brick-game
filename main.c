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
  // log file
#ifdef LOGS
  FILE *fp = fopen(FILE_NAME_FOR_LOGS, "w");
  if (fp) {
    fclose(fp);
  }
#endif //LOGS

  window_settings();

  // init game, action structs
  GameInfo_t tetris = updateCurrentState();
  UserAction_t action = {0};
  bool hold = 0;

  // start the timer
  struct timespec t_previous = {0};
  clock_gettime(CLOCK_MONOTONIC, &t_previous);

  print_game(tetris);

  while (tetris.field || tetris.next) {
    if (get_player(&action)) {
      userInput(action, hold);
      tetris = updateCurrentState();
      print_game(tetris);
    }
    if (now_is_tick_time(&t_previous, tetris.speed)) {
      userInput(Down, hold);  // for shift
      tetris = updateCurrentState();
      print_game(tetris);
    }
  }
  endwin();
  return 0;
}