#include <stdlib.h>

#include "tetris.h"

#ifndef TETRIS_FSM_C
#define TETRIS_FSM_C

void on_start_state() {
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Start:
      fill_tetris();
      *fsm = spawn;
      break;
    case Terminate:
      *fsm = finish;
      break;
    default:
      break;
  }
}

void on_spawn_state() {
  spawn_figure();
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Terminate:
      *fsm = finish;
      break;
    default:
      *fsm = moving;
      break;
  }
  tetris_act();
}

void on_moving_state() {
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Pause:
      *fsm = pause;
      break;
    case Terminate:
      *fsm = finish;
      break;
    case Left:
      move_fig_left();
      if (collision()) {
        move_fig_right();
      }
      break;
    case Right:
      move_fig_right();
      if (collision()) {
        move_fig_left();
      }
      break;
    case Down:
      *fsm = shift;
      break;
    case Action:
      rotate_fig();
      if (collision()) {
        unrotate_fig();
      }
      break;
    default:
      break;
  }
}

void on_shifting_state() {
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Terminate:
      *fsm = finish;
      break;
    default:
      move_fig_down();
      if (collision()) {
        move_fig_up();
        *fsm = attaching;
      } else {
        *fsm = moving;
      }
      break;
  }
  tetris_act();
}

void on_pause_state() {
  make_pause();
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Pause:
      *fsm = moving;
      make_unpause();
      break;
    case Terminate:
      *fsm = finish;
      break;

    default:
      break;
  }
}

void on_attaching_state() {
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Terminate:
      *fsm = finish;
      break;

    default:
      if (!fig_to_field()) {
        *fsm = gameover;
      } else {
        check_complete_lines();
        if (first_line_is_empty()) {
          *fsm = spawn;
        } else {
          *fsm = gameover;
        }
      }
      break;
  }
  tetris_act();
}

void on_game_over_state() {
  UserAction_t *input = get_instance_input_to_fsm();
  finite_state_machine *fsm = get_instance_fsm();
  switch (*input) {
    case Start:
      *fsm = start;
      tetris_act();
      break;
    case Terminate:
      *fsm = finish;
      break;

    default:
      break;
  }
}

void on_game_finish_state() {}

void tetris_act() {
  finite_state_machine *fsm = get_instance_fsm();
  switch (*fsm) {
    case start:
      // print_log("start\n");
      on_start_state();
      break;
    case spawn:
      // print_log("spawn\n");
      on_spawn_state();
      break;
    case moving:
      // print_log("moving\n");
      on_moving_state();
      break;
    case shift:
      // print_log("shift\n");
      on_shifting_state();
      break;
    case pause:
      // print_log("pause\n");
      on_pause_state();
      break;
    case attaching:
      // print_log("attaching\n");
      on_attaching_state();
      break;
    case gameover:
      // print_log("gameover\n");
      on_game_over_state();
      break;
    case finish:
      // on_game_finish_state();
      break;
    default:
      // print_log("default:\n");
      break;
  }
}

#endif