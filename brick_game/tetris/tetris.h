/**
 * \file
 * \brief header file of s21_tetris library
 * @author https://github.com/Nfcz
 */
#ifndef TETRIS_H
#define TETRIS_H
// #define _POSIX_C_SOURCE 199309L
// #define __USE_POSIX199309
// #define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define S21_NULL (void *)0
#define HEIGHT 20
#define WIGHT 10
#define SIZE_F 5
#define NUM_OF_FIGURES 7
#define FIG_N_I 1
#define FIG_N_T 2
#define FIG_N_L 3
#define FIG_N_R 4
#define FIG_N_Z 5
#define FIG_N_S 6
#define FIG_N_O 7
#define FILE_NAME_FOR_SCORE "tetrisHighScore.txt"
#define FILE_NAME_FOR_LOGS "tetrisLog.txt"
#define NO_INF_WHILE 1000

/**
 * \brief enum of all possible state in finite state machine of tetris
 */
typedef enum {
  start,      ///< start of the game
  spawn,      ///< spawn new figure
  moving,     ///< moving falling figure in a field
  attaching,  ///< attaching falling figure to stopped figure
  shift,      ///< shift falling figure down
  pause,      ///< game pause
  gameOver,   ///< end of this game
  finish      ///< end of this program run
} FiniteStateMachine;

/**
 *\brief Enum, that store all possible button on console
 */
typedef enum {
  Start,      ///< button Start
  Pause,      ///< button Pause
  Terminate,  ///< button Terminate
  Left,       ///< button Left
  Right,      ///< button Right
  Up,         ///< button Up
  Down,       ///< button Down
  Action      ///< button Action
} UserAction_t;

/**
 * \brief struct that store input to fsm
 *
 * timer it means it inside call from timer;
 */
typedef struct {
  UserAction_t action;  ///< action of player
  int shift;            ///< 1 if timer, 0 if player
} InputToFsm_t;

/**
 * \brief API struct, that store output game info for front
 *
 * This struct store information to display a game. Used as a link between
 * front and back. Not used for counting game.
 */
typedef struct {
  int **field;    ///< result field with falling figure and stopt blokes
  int **next;     ///< next figure, to show preview
  int score;      ///< score of game
  int highScore;  ///< highest score in game in this installation
  int level;      ///< level of game
  int speed;      ///< speed of game
  int pause;      ///< 1 if game on pause, 0 if it's not
} GameInfo_t;

/**
 * \brief My struct to that store and process game info in back
 *
 * This struct store all info that used in a game. Used only inside back
 */
typedef struct {
  int field[HEIGHT]
           [WIGHT];  ///< result field with falling figure and stopt blokes
  int next[SIZE_F][SIZE_F];        ///< next figure, to show preview
  int presentFig[SIZE_F][SIZE_F];  ///<
  int figNum;         ///< figure number of figure, that is falling down
  int previewFigNum;  ///< figure number, that is on preview
  int x;              ///< the x coordinate of hight left zone of falling figure
  int y;              ///< the y coordinate of hight left zone of falling figure
  int score;          ///< score of game
  int highScore;      ///< highest score in game in this installation
  int level;          ///< level of game
  int speed;          ///< speed of game
  int pause;          ///< 1 if game on pause, 0 if it's not
} InsideGameInfo_t;

/**
 * \defgroup back Backend
 * @{
 */

/**
 * \defgroup Front-Back Functions fo cooperate frontend and backend
 * \brief This group for functions that cooperate between frontend and backend
 * @{
 */
/**
 * \defgroup open_fun Functions, that open to Front
 * \ingroup Front-Back
 * \brief this function can be called in front
 * @{
 */
/**
 * \brief function that updates game info for correct output
 *
 * this function make new example of `GameInfo_t` with data from
 * `InsideGameInfo_t`
 * \warning should be called only by timer of shift
 * \return curent info of tetris game
 */
GameInfo_t updateCurrentState();

/**
 * \brief function that insert actions of player to backend
 *
 * this function insert value to static input_to_fsm
 * \warning should be called only when player press the button
 * \param[in] action action of player
 * \param[in] hold 1 when player is pressing the button, 0 if not
 */
void userInput(UserAction_t action, bool hold);

/**
 * @}
 * \defgroup back_output functions that forming ouput for frontend
 * \ingroup Front-Back
 * \brief Inside functions, that make ouput for frontend
 * @{
 * \defgroup static functions that make static arrays
 * \ingroup back_output
 * \brief Functions, that make static arrays
 * @{
 */
/**
 * \brief make static array for output
 * make static int HEIGHT x WIGHT for result field
 * \return array fo field with zeros
 */
int **getInstanceField();
/**
 * \brief make static array for output figure
 * make static int SIZE_F x SIZE_F for result next figure
 * \return array for figure with zeros
 */
int **getInstanceFigure();
/**
 * @}
 * \defgroup output_filling functions fill arrays for output
 * \ingroup back_output
 * \brief Inside functions, that make ouput for frontend
 * @{
 */
/**
 * \brief fill result block of field with data from inside field
 * Add data from `InsideGameInfo_t` to the result field. Glues the figure to the
 * field
 * \return filled block
 */
void blockOfFieldOutput(int i, int j);
/**
 * \brief fill result field with data from inside field
 * Add data from `InsideGameInfo_t` to the result field. Glues the figure to the
 * field \return filled field
 */
int **fieldOutput();
/**
 * \brief fill result figure with data from inside next
 * \return filled field
 */
int **nextOutput();
/**
 * @}
 * @}
 * @}
 */

/**
 * \defgroup fsm finite-state machine
 * \brief functions that operate finite-state machine
 * @{
 */
/**
 * \brief function that operate on start state
 * - if pressed `start` go to `spawn`
 * - if pressed `terminate` go to `finish`
 */
void onStartState();
/**
 * \brief function that operate on spawn state
 * - if nothing pressed (that means time is up) go to `moving`
 * - if pressed `terminate` go to `finish`
 */
void onSpawnState();
/**
 * \brief function that operate on moving state
 * - if nothing pressed (that means time is up) go to `shift`
 * - if pressed `Pause` go to `pause`
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Left`, `Right`, `Down` go to `moving`, and make move of figure
 * - if pressed `Action` go to `moving` rotate and check collision
 */
void onMovingState();
/**
 * \brief function that operate on shifting state
 * - if nothing pressed (that means time is up), move figure down, check
 * collision and go to `attaching` or `moving`
 * - if pressed `Terminate` go to `finish`
 */
void onShiftingState();
/**
 * \brief function that operate on pause state
 *
 * make pause and:
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Pause` go to `moving`
 */
void onPauseState();
/**
 * \brief function that operate on attaching state
 * - if nothing pressed (that means time is up), check can figure go to field,
 * and can new figure appear. If yes to both -> `spawn`, if no `gameOver`
 * - if pressed `Terminate` go to `finish`
 */
void onAttachingState();
/**
 * \brief function that operate on game over state
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Start` go to `start`
 */
void onGameOverState();
/**
 * \brief function the make next step in finite-state machine
 *
 * checking current state and sending to local function
 */
void tetrisAct();
///@}

/**
 * \brief function the make FSM do shift by timer
 *
 * making intput to FSM that FSM would be shifting figure properly.
 * calling in a while timer
 */
void shifter();

/**
 * \brief function the make FSM do shift of to bottom
 *
 * Shift figure to the bottom unlit it collision.
 * Calling collision in while
 */
void shiftToBottom();

/**
 * \defgroup singleton Singleton's
 * \brief function that have local static and return poitier to it
 * @{
 */
/**
 * \brief get instance tetris info struct
 *
 * make local static version of struct `InsideGameInfo_t`
 * \return poitier to static version of inside game info
 */
InsideGameInfo_t *getInstanceTetris();
/**
 * \brief get instance of input to fsm struct
 *
 * make local static version of struct `input_to_fsm`
 * \return poitier to static version of input to fsm
 */
InputToFsm_t *getInstanceInputToFsm();
/**
 * \brief get state of finite state machine
 *
 * make local static version of struct `FiniteStateMachine`
 * \return poitier to static version of state machine in FSM
 */
FiniteStateMachine *getInstanceFsm();

/**
 * \brief get time struct
 *
 * make local static version of `struct timespec` for saving previous shift time
 * \return poitier to static version of previous shift time
 */
struct timespec *getTimePrev();
///@}

/**
 * \defgroup fig_t all figure type
 * \brief function with all figure types
 * @{
 */
/**
 * \brief function with all figures
 *
 * function, that on input have number of needed block, and number of needed
 * figure and return block
 * \param[in] numOfFig number of figure
 * \param[in] blockNum number of block in figure
 * \return block that in needed place
 */
int blocksOfFig(int numOfFig, int blockNum);
///@}

/**
 * \defgroup init initialization
 * \brief functions that called in a start of the game
 * @{
 */
/**
 * \brief fill game with starting values
 * fill all the field of `InsideGameInfo_t` with stating values
 */
void fillTetris();
/**
 * \defgroup high_score initialization of high score
 * \brief initialization of high score from file
 * @{
 */
/**
 * \brief make `str` to `int` for score
 *
 * digit by digit read string and write it to integer
 * \warning if in file something else, except ACHII character from 48 to 57,
 * stoped reading
 */
int strToInt(char *str);
/**
 * \brief open a file, read score
 *
 * Open a file for high score and read string, call `strToInt` to make integer
 * from string
 *
 * \return 1 if okey, 0 if can't open and create file
 */
int getHighScore();
/**
 * \brief write high score curent game to file
 *
 * write high score from
 * \return 1 if okey, 0 if can't open and create file
 */
int writeHighScore();
///@}
///@}

/**
 * \defgroup spawn Spawning
 * \brief function that called when spawning
 *
 * @{
 */
/**
 * \brief get right `y` for spawning figure
 *
 * get spawning figure on right height
 */
void getRightHeight();
/**
 * \brief spawn new figure to preview
 */
void newFigToPreview();
/**
 * \brief copy figure from preview to falling figure
 */
void figFromPreviewToGame();
/**
 * \brief spawn figure
 *
 * copy figure from preview to falling figure, and spawn new figure to preview
 */
void spawnFigure();
///@}

/**
 * \defgroup move_fig move figure
 * \brief functions that move figures
 *
 * @{
 */
/**
 * \brief move figure left
 */
void moveFigLeft();
/**
 * \brief move figure right
 */
void moveFigRight();
/**
 * \brief move figure down
 */
void moveFigDown();
/**
 * \brief move figure up
 */
void moveFigUp();
/**
 * \brief rotate figure
 */
void rotateFig();
/**
 * \brief unrotate figure
 */
void unRotateFig();
///@}

/**
 * \defgroup fig_field falling figure and field
 * \brief functions how do fields and falling figure interact
 * @{
 */
/**
 * \brief check collision between block of figure and block field
 * \param i hight if checking block
 * \param j weight if checking block
 * \return 1 if collision, 0 if not
 */
int blockCollision(int i, int j);
/**
 * \brief check collision between figure and field
 * \return 1 if collision, 0 if not
 */
int collision();
/**
 * \brief Glues the block of falling figure to the field
 * \param i hight if checking block
 * \param j weight if checking block
 * \return 0 if block out of the field, 1 if not
 */
int blockField(int i, int j);
/**
 * \brief Glues the falling figure to the field
 * \return 0 if figure out of the field, 1 if not
 */
int figToField();
///@}

/**
 * \defgroup check_fun The checking functions
 * @{
 */
/**
 * \brief count speed from the level
 *
 * speed longness of one tick in milliseconds
 */
void getSpeed();
/**
 * \brief get level from the score
 */
void checkLevel();
/**
 * \brief checking if first line is empty
 *
 * 1 if empty, 0 if not
 */
int firstLineIsEmpty();
/**
 * \brief check tick time
 *
 * check is time from last update is more than tick time
 * \return count of tick that have be done
 */
int countOfTick();
///@}

/**
 * \defgroup drop_line dropping a line
 * @{
 */
/**
 * \brief checking if this line is complete
 * \param[in] height the height of line that should be check
 * \return 1 if line is complete, 0 if not
 */
int thisLineIsComplete(int height);
/**
 * \brief checking all lines for completness
 *
 * if for checking all lines from lowest to heist, in while cheek lines and
 * shift if complete
 */
void checkCompleteLines();
/**
 * \brief shift rest of field
 *
 * shifting the rest of the field, whn one line is dropped
 * \param[in] height height the height of line that should be dropped
 */
void shiftRestOfField(int height);
/**
 * \brief count number of dropped lines to the score
 */
void completeLinesToScore(int complete_lines);
///@}

/**
 * \defgroup pause pause
 * @{
 */
/**
 * \brief make pause = 1 in tetris info
 */
void makePause();
/**
 * \brief make pause = 0 in tetris info
 */
void makeUnPause();
///@}

#ifdef LOGS
/**
 * \defgroup log log
 * @{
 */
/**
 * \brief print logs to the log file
 */
void printLog(char *message);
#endif  // LOGS
///@}
///@}

#endif  // TETRIS_H