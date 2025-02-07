#include <stdlib.h>

#include "tetris.h"

InsideGameInfo_t *getInstanceTetris() {
  static InsideGameInfo_t tetris = {0};
  return &tetris;
}

int **getInstanceField() {
  static int values[HEIGHT * WIGHT] = {0};
  static int *pValues[HEIGHT] = {0};
  for (int i = 0; i < HEIGHT; i++) {
    pValues[i] = &(values[i * WIGHT]);
  }
  static int **pPValues = pValues;
  return pPValues;
}

int **getInstanceFigure() {
  static int values[SIZE_F * SIZE_F] = {0};
  static int *pValues[SIZE_F] = {0};
  for (int i = 0; i < SIZE_F; i++) {
    pValues[i] = &(values[i * SIZE_F]);
  }
  static int **pPValues = pValues;
  return pPValues;
}

int blocksOfFig(int numOfFig, int blockNum) {
  static const int figArr[] = {
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
  return figArr[(numOfFig - 1) * SIZE_F * SIZE_F + blockNum];
}

InputToFsm_t *getInstanceInputToFsm() {
  static InputToFsm_t input = {0};
  return &input;
}

FiniteStateMachine *getInstanceFsm() {
  static FiniteStateMachine fsm = {0};
  return &fsm;
}

struct timespec *getTimePrev() {
  static struct timespec tPrev = {0};
  return &tPrev;
}

void shifter() {
  InputToFsm_t *input = getInstanceInputToFsm();
  int count = countOfTick();
  for (int i = 0; i < count; i++) {
    input->shift = 1;
    tetrisAct();
  }
  input->shift = 0;
}

void shiftToBottom() {
  for (int i = 0; i < NO_INF_WHILE && !collision(); i++) {
    moveFigDown();
  }
}

void userInput(UserAction_t action, bool hold) {
  shifter();
  InputToFsm_t *input = getInstanceInputToFsm();
  input->action = action;
  tetrisAct();
  if (hold) {
    input->action = action;
  }
}

void newFigToPreview() {
  InsideGameInfo_t *tetris = getInstanceTetris();
#ifndef TESTING
  int fugNum = rand() % NUM_OF_FIGURES + 1;
#endif  // not TESTING
#ifdef TESTING
  int fugNum = FIG_N_I;
#endif  // TESTING
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->next[i][j] = blocksOfFig(fugNum, i * SIZE_F + j);
    }
  }
  tetris->previewFigNum = fugNum;
}

int strToInt(char *str) {
  int res = 0;
  for (; *str >= 48 && *str <= 57; str++) {
    res = res * 10 + (*str - 48);
  }
  return res;
}

int getHighScore() {
  int status = 1;
  FILE *fp = fopen(FILE_NAME_FOR_SCORE, "r");
  if (fp != S21_NULL) {
    char scoreStr[20];
    fgets(scoreStr, 10, fp);
    fclose(fp);
    InsideGameInfo_t *tetris = getInstanceTetris();
    tetris->highScore = strToInt(scoreStr);
  } else {
    fp = fopen(FILE_NAME_FOR_SCORE, "w");
    if (!fp) {
      status = 0;
    } else {
      fprintf(fp, "%d", 0);
      fclose(fp);
    }
  }
  return status;
}

void getSpeed() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->speed = 1000 / tetris->level;
}

void fillTetris() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIGHT; j++) {
      tetris->field[i][j] = 0;
    }
  }
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->next[i][j] = 0;
      tetris->presentFig[i][j] = 0;
    }
  }
  newFigToPreview();
  getHighScore();
  tetris->score = 0;
  tetris->level = 1;
  getSpeed();
  tetris->figNum = 0;
  tetris->pause = 0;
  struct timespec *tPrev = getTimePrev();
  clock_gettime(CLOCK_MONOTONIC, tPrev);
}

void getRightHeight() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  switch (tetris->figNum) {
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

void figFromPreviewToGame() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      tetris->presentFig[i][j] = tetris->next[i][j];
    }
  }
  tetris->figNum = tetris->previewFigNum;
  tetris->x = (WIGHT - SIZE_F) / 2 + 1;
  getRightHeight();
}

void spawnFigure() {
  figFromPreviewToGame();
  newFigToPreview();
}

int thisLineIsComplete(int height) {
  InsideGameInfo_t *tetris = getInstanceTetris();
  int status = 1;
  for (int i = 0; i < WIGHT; i++) {
    if (!tetris->field[height][i]) {
      status = 0;
    }
  }
  return status;
}

void shiftRestOfField(int height) {
  InsideGameInfo_t *tetris = getInstanceTetris();
  for (int i = height; i > 0; i--) {
    for (int j = 0; j < WIGHT; j++) {
      tetris->field[i][j] = tetris->field[i - 1][j];
    }
  }
  for (int j = 0; j < WIGHT; j++) {
    tetris->field[0][j] = 0;
  }
}

int writeHighScore() {
  int status = 0;
  FILE *fp = fopen(FILE_NAME_FOR_SCORE, "w");
  if (fp != S21_NULL) {
    InsideGameInfo_t *tetris = getInstanceTetris();
    fprintf(fp, "%d", tetris->highScore);
    fclose(fp);
  }
  return status;
}

void checkLevel() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->level = tetris->score / 600 + 1;
  if (tetris->level > 10) tetris->level = 10;
}

void completeLinesToScore(int complete_lines) {
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
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->score += score;
  if (tetris->highScore < tetris->score) {
    tetris->highScore = tetris->score;
    writeHighScore();
  }
  checkLevel();
  getSpeed();
}

void checkCompleteLines() {
  int complete_lines = 0;
  for (int i = HEIGHT - 1; i >= 0; i--) {
    while (thisLineIsComplete(i)) {
      complete_lines++;
      shiftRestOfField(i);
    }
  }
  completeLinesToScore(complete_lines);
}

void moveFigLeft() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->x--;
}

void moveFigRight() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->x++;
}

void moveFigDown() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->y++;
}

void moveFigUp() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->y--;
}

int blockCollision(int i, int j) {
  InsideGameInfo_t *tetris = getInstanceTetris();
  int status = 0;
  if (tetris->presentFig[i][j]) {
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
      status = blockCollision(i, j);
    }
  }
  return status;
}

void rotateFig() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  if (tetris->figNum != FIG_N_O) {
    int figureCopy[SIZE_F][SIZE_F] = {0};
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        figureCopy[i][j] = tetris->presentFig[i][j];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->figNum != FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->presentFig[i][j] = figureCopy[j][SIZE_F - 1 - i];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->figNum == FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->presentFig[i][j] = figureCopy[j][i];
      }
    }
  }
}

void unRotateFig() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  if (tetris->figNum != FIG_N_O) {
    int figureCopy[SIZE_F][SIZE_F] = {0};
    for (int i = 0; i < SIZE_F; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        figureCopy[i][j] = tetris->presentFig[i][j];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->figNum != FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->presentFig[i][j] = figureCopy[SIZE_F - 1 - j][i];
      }
    }
    for (int i = 0; i < SIZE_F && tetris->figNum == FIG_N_I; i++) {
      for (int j = 0; j < SIZE_F; j++) {
        tetris->presentFig[i][j] = figureCopy[j][i];
      }
    }
  }
}

int blockField(int i, int j) {
  InsideGameInfo_t *tetris = getInstanceTetris();
  int status = 1;
  if (tetris->presentFig[i][j]) {
    int x = j + tetris->x;
    int y = i + tetris->y;
    if (x >= 0 && x < WIGHT && y >= 0 && y < HEIGHT) {
      tetris->field[y][x] = tetris->presentFig[i][j];
    } else {
      status = 0;
    }
  }
  return status;
}

int figToField() {
  int status = 1;
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      status = blockField(i, j);
    }
  }
  return status;
}

int firstLineIsEmpty() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  int status = 1;
  for (int i = 0; i < WIGHT; i++) {
    if (tetris->field[0][i]) {
      status = 0;
    }
  }
  return status;
}

void makePause() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->pause = 1;
}

void makeUnPause() {
  InsideGameInfo_t *tetris = getInstanceTetris();
  tetris->pause = 0;
}

void blockOfFieldOutput(int i, int j) {
  InsideGameInfo_t *tetris = getInstanceTetris();
  int **field = getInstanceField();
  field[i][j] = tetris->field[i][j];
  int x = j - tetris->x;
  int y = i - tetris->y;
  if (x >= 0 && x < SIZE_F && y >= 0 && y < SIZE_F) {
    if (tetris->presentFig[y][x] != 0) {
      field[i][j] = tetris->presentFig[y][x];
    }
  }
}

int **fieldOutput() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIGHT; j++) {
      blockOfFieldOutput(i, j);
    }
  }
  int **field = getInstanceField();
  return field;
}

int **nextOutput() {
  InsideGameInfo_t *tetris = getInstanceTetris();

  int **next = getInstanceFigure();
  for (int i = 0; i < SIZE_F; i++) {
    for (int j = 0; j < SIZE_F; j++) {
      next[i][j] = tetris->next[i][j];
    }
  }
  return next;
}

GameInfo_t updateCurrentState() {
  shifter();
  GameInfo_t tetrisOutput = {0};
  InsideGameInfo_t *tetris = getInstanceTetris();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (*fsm != finish && *fsm != gameOver && *fsm != start) {
    tetrisOutput.field = fieldOutput();
    tetrisOutput.next = nextOutput();
  } else if (*fsm == gameOver) {
    tetrisOutput.field = fieldOutput();
  } else if (*fsm == start) {
    tetrisOutput.next = nextOutput();
  }
  tetrisOutput.highScore = tetris->highScore;
  tetrisOutput.level = tetris->level;
  tetrisOutput.pause = tetris->pause;
  tetrisOutput.score = tetris->score;
  tetrisOutput.speed = tetris->speed;
  return tetrisOutput;
}

int countOfTick() {
  int res = 0;
  InsideGameInfo_t *tetris = getInstanceTetris();
  struct timespec *tPrev = getTimePrev();
  struct timespec tNow = {0};
  if (tPrev->tv_sec == 0 && tPrev->tv_nsec == 0) {
    clock_gettime(CLOCK_MONOTONIC, tPrev);
  }
  clock_gettime(CLOCK_MONOTONIC, &tNow);
  int diff = (tNow.tv_nsec - tPrev->tv_nsec) / 1000000 +
             (tNow.tv_sec - tPrev->tv_sec) * 1000;
  if (diff > tetris->speed + 1) {
    res = diff / (tetris->speed + 1);
    tPrev->tv_nsec = tNow.tv_nsec;
    tPrev->tv_sec = tNow.tv_sec;
  }
  return res;
}  // maybe do it differently

#ifdef LOGS
void printLog(char *message) {
  FILE *fp = fopen(FILE_NAME_FOR_LOGS, "a");
  if (fp) {
    fprintf(fp, "%s", message);
    fclose(fp);
  }
}
#endif  // LOGS