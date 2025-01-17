/**
 * \file
 * \brief header file of console version of game
 */
#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "../../brick_game/tetris/tetris.h"

#define BACKGROUND_PAIR 0
#define FIG_N_I_PAIR 160
#define FIG_N_T_PAIR 20
#define FIG_N_L_PAIR 11
#define FIG_N_R_PAIR 208
#define FIG_N_Z_PAIR 53
#define FIG_N_S_PAIR 22
#define FIG_N_O_PAIR 212

/**
 * \defgroup front
 * @{
 */
/**
 * \brief function that print field with ncurses
 *
 * Print field 20 x 10 "pixel" with standard colors. "Pixel" is 2 spaces long,
 * one line hight
 *
 * \param[in] tetris game of tetris, that should be printed
 */
void print_field(GameInfo_t tetris);

/**
 * \brief function that print statistics
 *
 * print score, high score, and preview of new figure
 *
 * \param[in] tetris game of tetris, that should be printed
 */
void print_stat(GameInfo_t tetris);

/**
 * \brief print preview of a next figure
 * \param[in] tetris game of tetris, that should be printed
 */
void print_next(GameInfo_t tetris);

/**
 * \brief print current game of tetris
 * \param[in] tetris game of tetris, that should be printed
 */
void print_game(GameInfo_t tetris);

/**
 * \brief check keyboard and write it to the enum 'UserAction_t'
 * \param[out] action action that happened
 */
int get_player(UserAction_t *action);

/**
 * \brief check tick time
 *
 * check is time from last update is more than tick time
 * \param[inout] t_previous time of last tick
 * \param[in] speed curent speed of game
 */
int now_is_tick_time(struct timespec *t_previous, int speed);

/**
 * \brief start window settings
 */
void window_settings();

void printttt();

///@}

#endif