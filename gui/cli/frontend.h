/**
 * \file
 * \brief header file of console version of game
 */
#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H
#include <ncurses.h>
#include <stdio.h>

#include "../../brick_game/tetris/tetris.h"

#define BACKGROUND_PAIR 0
#define FRAME_PAIR NUM_OF_FIGURES + 1
#define FRAME_COLOR 244
#define FIG_N_I_COLOR 160
#define FIG_N_T_COLOR 20
#define FIG_N_L_COLOR 11
#define FIG_N_R_COLOR 208
#define FIG_N_Z_COLOR 53
#define FIG_N_S_COLOR 22
#define FIG_N_O_COLOR 212

#define ESC_BUTTON 27
#define ENTER_BUTTON 10

/**
 * \defgroup front
 * @{
 */

/**
 * \brief start window settings
 */
void windowSettings();

/**
 * \brief check keyboard and write it to the enum 'UserAction_t'
 * \param[out] action action that happened
 */
int getPlayer(UserAction_t *action);

/**
 * \brief print current game of tetris
 * \param[in] tetris game of tetris, that should be printed
 */
void printGame(GameInfo_t tetris);

/**
 * \brief print frame for game over and start mode
 */
void printFilling();

/**
 * \brief print frame for game and pause mode
 */
void printFrame();

/**
 * \brief print statistik of game
 *
 * print score, hight score, level, and pause status
 */
void printStart();

/**
 * \brief function that print field with ncurses
 *
 * Print field 20 x 10 "pixel" with standard colors. "Pixel" is 2 spaces long,
 * one line hight
 *
 * \param[in] tetris game of tetris, that should be printed
 */
void printField(GameInfo_t tetris);

/**
 * \brief print preview of a next figure
 * \param[in] tetris game of tetris, that should be printed
 */
void printNext(GameInfo_t tetris);

/**
 * \brief print statistik of game
 *
 * print score, hight score, level, and pause status
 *
 * \param[in] tetris game of tetris, that should be printed
 */
void printStat(GameInfo_t tetris);

/**
 * \brief function that print pause window
 *
 * print "pause" in the center of the board
 */
void printPause();

/**
 * \brief function that print field with ncurses
 *
 * print score hight score6 when game is over
 *
 * \param[in] tetris game of tetris, that should be printed
 */
void printGameOver(GameInfo_t tetris);
///@}

#endif