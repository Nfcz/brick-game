/**
 * \file
 * \brief header file of s21_tetris library
 */
#ifndef TETRIS_H
#define TETRIS_H
#include <stdbool.h>
#include <stdio.h>

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
#define FILE_NAME_FOR_SCORE "tetris_high_score.txt"
#define FILE_NAME_FOR_LOGS "tetris_log.txt"

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
  gameover,   ///< end of this game
  finish      ///< end of this program run
} finite_state_machine;

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
 * \brief API struct, that store output game info for front
 *
 * This struct store information to display a game. Used as a link between
 * front and back. Not used for counting game.
 */
typedef struct {
  int **field;     ///< result field with falling figure and stopt blokes
  int **next;      ///< next figure, to show preview
  int score;       ///< score of game
  int high_score;  ///< highest score in game in this installation
  int level;       ///< level of game
  int speed;       ///< speed of game
  int pause;       ///< 1 if game on pause, 0 if it's not
} GameInfo_t;

/**
 * \brief My struct to that store and process game info in back
 *
 * This struct store all info that used in a game. Used only inside back
 */
typedef struct {
  int field[HEIGHT]
           [WIGHT];  ///< result field with falling figure and stopt blokes
  int next[SIZE_F][SIZE_F];         ///< next figure, to show preview
  int present_fig[SIZE_F][SIZE_F];  ///<
  int fig_num;          ///< figure number of figure, that is falling down
  int preview_fig_num;  ///< figure number, that is on preview
  int x;           ///< the x coordinate of hight left zone of falling figure
  int y;           ///< the y coordinate of hight left zone of falling figure
  int score;       ///< score of game
  int high_score;  ///< highest score in game in this installation
  int level;       ///< level of game
  int speed;       ///< speed of game
  int pause;       ///< 1 if game on pause, 0 if it's not
} InsideGameInfo_t;

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
int **get_instance_field();
/**
 * \brief make static array for output figure
 * make static int SIZE_F x SIZE_F for result next figure
 * \return array for figure with zeros
 */
int **get_instance_figure();
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
void block_of_field_output(int i, int j);
/**
 * \brief fill result field with data from inside field
 * Add data from `InsideGameInfo_t` to the result field. Glues the figure to the
 * field \return filled field
 */
int **field_output();
/**
 * \brief fill result figure with data from inside next
 * \return filled field
 */
int **next_output();
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
void on_start_state();
/**
 * \brief function that operate on spawn state
 * - if nothing pressed (that means time is up) go to `moving`
 * - if pressed `terminate` go to `finish`
 */
void on_spawn_state();
/**
 * \brief function that operate on moving state
 * - if nothing pressed (that means time is up) go to `shift`
 * - if pressed `Pause` go to `pause`
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Left`, `Right`, `Down` go to `moving`, and make move of figure
 * - if pressed `Action` go to `moving` rotate and check collision
 */
void on_moving_state();
/**
 * \brief function that operate on shifting state
 * - if nothing pressed (that means time is up), move figure down, check
 * collision and go to `attaching` or `moving`
 * - if pressed `Terminate` go to `finish`
 */
void on_shifting_state();
/**
 * \brief function that operate on pause state
 *
 * make pause and:
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Pause` go to `moving`
 */
void on_pause_state();
/**
 * \brief function that operate on attaching state
 * - if nothing pressed (that means time is up), check can figure go to field,
 * and can new figure appear. If yes to both -> `spawn`, if no `gameover`
 * - if pressed `Terminate` go to `finish`
 */
void on_attaching_state();
/**
 * \brief function that operate on gameover state
 * - if pressed `Terminate` go to `finish`
 * - if pressed `Start` go to `start`
 */
void on_game_over_state();
/**
 * \brief function that operate on finish state
 *
 * do nothing
 */
void on_game_finish_state();
/**
 * \brief function the make next step in finite-state machine
 *
 * checking current state and sending to local function
 */
void tetris_act();
///@}

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
InsideGameInfo_t *get_instance_tetris();
/**
 * \brief get instance of input to fsm struct
 *
 * make local static version of struct `input_to_fsm`
 * \return poitier to static version of input to fsm
 */
UserAction_t *get_instance_input_to_fsm();
/**
 * \brief get state of finite state machine
 *
 * make local static version of struct `finite_state_machine`
 * \return poitier to static version of state machine in FSM
 */
finite_state_machine *get_instance_fsm();
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
 * \param[in] number_of_fig number of figure 
 * \param[in] num_block number of block in figure 
 * \return block that in needed place
 */
int block_of_fig_t(int number_of_fig, int num_block);
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
void fill_tetris();
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
int str_to_int(char *str);
/**
 * \brief open a file, read score
 *
 * Open a file for high score and read string, call `str_to_int` to make integer
 * from string
 */
void get_high_score();
/**
 * \brief write high score curent game to file
 *
 * write high score from
 */
void write_high_score();
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
void get_right_height();
/**
 * \brief spawn new figure to preview
 */
void new_fig_to_preview();
/**
 * \brief copy figure from preview to falling figure
 */
void fig_from_preview_to_game();
/**
 * \brief spawn figure
 *
 * copy figure from preview to falling figure, and spawn new figure to preview
 */
void spawn_figure();
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
void move_fig_left();
/**
 * \brief move figure right
 */
void move_fig_right();
/**
 * \brief move figure down
 */
void move_fig_down();
/**
 * \brief move figure up
 */
void move_fig_up();
/**
 * \brief rotate figure
 */
void rotate_fig();
/**
 * \brief unrotate figure
 */
void unrotate_fig();
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
int block_collision(int i, int j);
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
int block_field(int i, int j);
/**
 * \brief Glues the falling figure to the field
 * \return 0 if figure out of the field, 1 if not
 */
int fig_to_field();
///@}

/**
 * \defgroup check_fun The checking functions
 * @{
 */
/**
 * \brief count speed from the level
 */
void get_speed();
/**
 * \brief get level from the score
 */
void check_level();
/**
 * \brief checking if first line is empty
 *
 * 1 if empty, 0 if not
 */
int first_line_is_empty();
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
int this_line_is_complete(int height);
/**
 * \brief checking all lines for completness
 *
 * if for checking all lines from lowest to heist, in while cheek lines and
 * shift if complete
 */
void check_complete_lines();
/**
 * \brief shift rest of field
 *
 * shifting the rest of the field, whn one line is dropped
 * \param[in] height height the height of line that should be dropped
 */
void shift_rest_of_field(int height);
/**
 * \brief count number of dropped lines to the score
 */
void complete_lines_to_score(int complete_lines);
///@}

/**
 * \defgroup pause pause
 * @{
 */
/**
 * \brief make pause = 1 in tetris info
 */
void make_pause();
/**
 * \brief make pause = 0 in tetris info
 */
void make_unpause();
///@}

/**
 * \defgroup log log
 * @{
 */
/**
 * \brief print logs to the log file
 */
void print_log(char *message);
///@}

#endif  // TETRIS_H