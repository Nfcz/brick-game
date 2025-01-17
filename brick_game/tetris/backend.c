#include <stdlib.h>

#include "tetris.h"

#ifndef TETRIS_BACKEND_C
#define TETRIS_BACKEND_C

InsideGameInfo_t *get_instance_tetris() {
  static InsideGameInfo_t tetris = {0};
  return &tetris;
}

int **get_instance_field() {
  static int values[HEIGHT * WIGHT] = {0};
  static int *pvalues[HEIGHT] = {0};
  for (int i = 0; i < HEIGHT; i++) {
    pvalues[i] = &(values[i * WIGHT]);
  }
  static int **ppvalues = pvalues;
  return ppvalues;
}

int **get_instance_figure() {
  static int values[SIZE_F * SIZE_F] = {0};
  static int *pvalues[SIZE_F] = {0};
  for (int i = 0; i < SIZE_F; i++) {
    pvalues[i] = &(values[i * SIZE_F]);
  }
  static int **ppvalues = pvalues;
  return ppvalues;
}

int block_of_fig_t(int number_of_fig, int num_block) {
  static int fig_t[] = {
      0, 0, FIG_N_I, 0,       0,       0, 0,       FIG_N_I, 0,
      0, 0, 0,       FIG_N_I, 0,       0, 0,       0,       FIG_N_I,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, 0,       FIG_N_T, 0,
      0, 0, FIG_N_T, FIG_N_T, FIG_N_T, 0, 0,       0,       0,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, FIG_N_L, FIG_N_L, 0,
      0, 0, 0,       FIG_N_L, 0,       0, 0,       0,       FIG_N_L,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, 0,       FIG_N_R, FIG_N_R,
      0, 0, 0,       FIG_N_R, 0,       0, 0,       0,       FIG_N_R,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, FIG_N_Z, FIG_N_Z, 0,
      0, 0, 0,       FIG_N_Z, FIG_N_Z, 0, 0,       0,       0,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, 0,       FIG_N_S, FIG_N_S,
      0, 0, FIG_N_S, FIG_N_S, 0,       0, 0,       0,       0,
      0, 0, 0,       0,       0,       0, 0,

      0, 0, 0,       0,       0,       0, FIG_N_O, FIG_N_O, 0,
      0, 0, FIG_N_O, FIG_N_O, 0,       0, 0,       0,       0,
      0, 0, 0,       0,       0,       0, 0};
  return fig_t[(number_of_fig - 1) * SIZE_F * SIZE_F + num_block];
}

UserAction_t *get_instance_input_to_fsm() {
  static UserAction_t input = {0};
  return &input;
}

finite_state_machine *get_instance_fsm() {
  static finite_state_machine fsm = {0};
  return &fsm;
}

void userInput(UserAction_t action, bool hold) {
  UserAction_t *input = get_instance_input_to_fsm();
  *input = action;
  FILE *fp = fopen(FILE_NAME_FOR_LOGS, "a");
  fprintf(fp, "%d", (int)action);
  fclose(fp);
  tetris_act();
  if (hold) {
    hold = 0;
  }
}

void new_fig_to_preview() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int fug_num = rand() % NUM_OF_FIGURES + 1;
  // fug_num = FIG_N_I; // for testing
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->next[i][j] = block_of_fig_t(fug_num, i * SIZE_F + j);
    }
  }
  tetris->preview_fig_num = fug_num;
}

int str_to_int(char *str) {
  int res = 0;
  for (; *str && *str >= 48 && *str <= 57; str++) {
    res = res * 10 + (*str - 48);
  }
  return res;
}

void get_high_score() {
  FILE *fp = fopen(FILE_NAME_FOR_SCORE, "r");
  if (fp != NULL) {
    char score_str[20];
    fgets(score_str, 10, fp);
    fclose(fp);
    InsideGameInfo_t *tetris = get_instance_tetris();
    tetris->high_score = str_to_int(score_str);
  }
}

void get_speed() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  switch (tetris->level) {
    case 0:
      tetris->speed = 20;
      break;
    case 1:
      tetris->speed = 30;
      break;
    case 2:
      tetris->speed = 40;
      break;
    case 3:
      tetris->speed = 45;
      break;
    case 4:
      tetris->speed = 48;
      break;
    case 5:
      tetris->speed = 50;
      break;
    case 6:
      tetris->speed = 53;
      break;
    case 7:
      tetris->speed = 56;
      break;
    case 8:
      tetris->speed = 58;
      break;
    case 9:
      tetris->speed = 60;
      break;

    default:
      break;
  }
}

void fill_tetris() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIGHT; j++) {
      tetris->field[i][j] = 0;
    }
  }
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->next[i][j] = 0;
    }
  }
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->present_fig[i][j] = 0;
    }
  }
  new_fig_to_preview();
  get_high_score();
  tetris->score = 0;
  tetris->level = 0;
  get_speed();
  tetris->fig_num = 0;
  tetris->pause = 0;
  tetris->preview_fig_num = 0;
}

void get_right_height() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  switch (tetris->fig_num) {
    case FIG_N_I:
    case FIG_N_L:
    case FIG_N_R:
      tetris->y = -3;
      break;
    case FIG_N_T:
    case FIG_N_S:
    case FIG_N_Z:
    case FIG_N_O:
      tetris->y = -2;
      break;
    default:
      tetris->y = 0;
      break;
  }
}

void fig_from_preview_to_game() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->present_fig[i][j] = tetris->next[i][j];
    }
  }
  tetris->fig_num = tetris->preview_fig_num;
  tetris->x = (WIGHT - SIZE_F) / 2 + 1;
  get_right_height();
}

void spawn_figure() {
  fig_from_preview_to_game();
  new_fig_to_preview();
}

int this_line_is_complete(int height) {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int status = 1;
  for (int i = 0; i < WIGHT; i++) {
    if (!tetris->field[height][i]) {
      status = 0;
    }
  }
  return status;
}

void shift_rest_of_field(int height) {
  InsideGameInfo_t *tetris = get_instance_tetris();
  for (int i = height; i > 0; i--) {
    for (int j = 0; j < WIGHT; j++) {
      tetris->field[i][j] = tetris->field[i - 1][j];
    }
  }
  for (int j = 0; j < WIGHT; j++) {
    tetris->field[0][j] = 0;
  }
}

void write_high_score() {
  FILE *fp = fopen(FILE_NAME_FOR_SCORE, "w");
  if (fp != NULL) {
    InsideGameInfo_t *tetris = get_instance_tetris();
    fprintf(fp, "%d", tetris->high_score);
    fclose(fp);
  }
}

void check_level() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->level = tetris->score / 600;
  if (tetris->level > 10) tetris->level = 10;
}

void complete_lines_to_score(int complete_lines) {
  int score = 0;
  switch (complete_lines) {
    case 1:
      score = 100;
      break;
    case 2:
      score = 300;
      break;
    case 3:
      score = 700;
      break;
    case 4:
      score = 1500;
      break;

    default:
      break;
  }
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->score += score;
  if (tetris->high_score < tetris->score) {
    tetris->high_score = tetris->score;
    write_high_score();
  }
  check_level();
}

void check_complete_lines() {
  int complete_lines = 0;
  for (int i = HEIGHT - 1; i >= 0; i--) {
    while (this_line_is_complete(i)) {
      complete_lines++;
      shift_rest_of_field(i);
    }
  }
  complete_lines_to_score(complete_lines);
}

void move_fig_left() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->x--;
}

void move_fig_right() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->x++;
}

void move_fig_down() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->y++;
}

void move_fig_up() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->y--;
}

int block_collision(int i, int j) {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int status = 0;
  if (tetris->present_fig[i][j]) {
    int y = i + tetris->y;
    int x = j + tetris->x;
    if (x >= 0 && x < WIGHT && y >= 0 && y < HEIGHT) {
      if (tetris->field[y][x]) {
        status = 1;
      }
    } else if (x >= WIGHT || x < 0 || y >= HEIGHT) {
      status = 1;
    }
  }
  return status;
}

int collision() {
  int status = 0;
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F && !status; j++) {
      status = block_collision(i, j);
    }
  }
  return status;
}

void rotate_fig() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  if (tetris->fig_num != FIG_N_O) {
    int figure_copy[SIZE_F][SIZE_F] = {0};
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        figure_copy[i][j] = tetris->present_fig[i][j];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->fig_num != FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->present_fig[i][j] = figure_copy[j][SIZE_F - 1 - i];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->fig_num == FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->present_fig[i][j] = figure_copy[j][i];
      }
    }
  }
}

void unrotate_fig() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  if (tetris->fig_num != FIG_N_O) {
    int figure_copy[SIZE_F][SIZE_F] = {0};
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        figure_copy[i][j] = tetris->present_fig[i][j];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->fig_num != FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->present_fig[i][j] = figure_copy[SIZE_F - 1 - j][i];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->fig_num == FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->present_fig[i][j] = figure_copy[j][i];
      }
    }
  }
}

int block_field(int i, int j) {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int status = 1;
  if (tetris->present_fig[i][j]) {
    int x = j + tetris->x;
    int y = i + tetris->y;
    if (x >= 0 && x <= WIGHT && y >= 0 && y <= HEIGHT) {
      tetris->field[y][x] = tetris->present_fig[i][j];
    } else {
      status = 0;
    }
  }
  return status;
}

int fig_to_field() {
  int status = 1;
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      status = block_field(i, j);
    }
  }
  return status;
}

int first_line_is_empty() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int status = 1;
  for (int i = 0; i < WIGHT; i++) {
    if (tetris->field[0][i]) {
      status = 0;
    }
  }
  return status;
}

void make_pause() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->pause = 1;
}

void make_unpause() {
  InsideGameInfo_t *tetris = get_instance_tetris();
  tetris->pause = 0;
}

void block_of_field_output(int i, int j) {
  InsideGameInfo_t *tetris = get_instance_tetris();
  int **field = get_instance_field();
  field[i][j] = tetris->field[i][j];
  int x = j - tetris->x;
  int y = i - tetris->y;
  if (x >= 0 && x < SIZE_F && y >= 0 && y < SIZE_F) {
    if (tetris->present_fig[y][x] != 0) {
      field[i][j] = tetris->present_fig[y][x];
    }
  }
}

int **field_output() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIGHT; j++) {
      block_of_field_output(i, j);
    }
  }
  int **field = get_instance_field();
  return field;
}

int **next_output() {
  InsideGameInfo_t *tetris = get_instance_tetris();

  int **next = get_instance_figure();
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      next[i][j] = tetris->next[i][j];
    }
  }
  return next;
}

GameInfo_t updateCurrentState() {
  GameInfo_t tetris_output = {0};
  InsideGameInfo_t *tetris = get_instance_tetris();
  finite_state_machine *fsm = get_instance_fsm();
  if (*fsm != finish && *fsm != gameover && *fsm != start) {
    tetris_output.field = field_output();
    tetris_output.next = next_output();
  } else if (*fsm == gameover) {
    tetris_output.field = field_output();
  } else if (*fsm == start) {
    tetris_output.next = next_output();
  }
  tetris_output.high_score = tetris->high_score;
  tetris_output.level = tetris->level;
  tetris_output.pause = tetris->pause;
  tetris_output.score = tetris->score;
  tetris_output.speed = tetris->speed;
  return tetris_output;
}

void print_log(char *message) {
  FILE *fp = fopen(FILE_NAME_FOR_LOGS, "a");
  if (!fp) {
    fp = fopen(FILE_NAME_FOR_LOGS, "w");
  }
  if (fp) {
    fprintf(fp, "%s", message);
    fclose(fp);
  }
}

#endif