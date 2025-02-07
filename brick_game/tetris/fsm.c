#include <stdlib.h>

#include "tetris.h"

void onStartState() {
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (!input->shift) {
    switch (input->action) {
      case Start:
        fillTetris();
        *fsm = spawn;
        tetrisAct();
        break;
      case Terminate:
        *fsm = finish;
        break;
      default:
        break;
    }
  }
}

void onSpawnState() {
  spawnFigure();
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  switch (input->action) {
    case Terminate:
      *fsm = finish;
      break;
    default:
      *fsm = moving;
      break;
  }
}

void onMovingState() {
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (!input->shift) {
    switch (input->action) {
      case Pause:
        *fsm = pause;
        break;
      case Terminate:
        *fsm = finish;
        break;
      case Left:
        moveFigLeft();
        if (collision()) {
          moveFigRight();
        }
        break;
      case Right:
        moveFigRight();
        if (collision()) {
          moveFigLeft();
        }
        break;
      case Down:
        shiftToBottom();
        moveFigUp();
        *fsm = attaching;
        tetrisAct();
        break;
      case Action:
        rotateFig();
        if (collision()) {
          unRotateFig();
        }
        break;
      default:
        break;
    }
  } else {
    *fsm = shift;
    tetrisAct();
  }
}

void onShiftingState() {
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (input->shift) input->shift = 0;
  switch (input->action) {
    case Terminate:
      *fsm = finish;
      break;
    default:
      moveFigDown();
      if (collision()) {
        moveFigUp();
        *fsm = attaching;
        tetrisAct();
      } else {
        *fsm = moving;
      }
      break;
  }
}

void onPauseState() {
  makePause();
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (!input->shift) {
    switch (input->action) {
      case Pause:
        makeUnPause();
        *fsm = moving;
        break;
      case Terminate:
        *fsm = finish;
        break;

      default:
        break;
    }
  }
}

void onAttachingState() {
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  switch (input->action) {
    case Terminate:
      *fsm = finish;
      break;

    default:
      if (!figToField()) {
        *fsm = gameOver;
      } else {
        checkCompleteLines();
        if (firstLineIsEmpty()) {
          *fsm = spawn;
          tetrisAct();
        } else {
          *fsm = gameOver;
        }
      }
      break;
  }
}

void onGameOverState() {
  InputToFsm_t *input = getInstanceInputToFsm();
  FiniteStateMachine *fsm = getInstanceFsm();
  if (!input->shift) {
    switch (input->action) {
      case Start:
        *fsm = start;
        tetrisAct();
        break;
      case Terminate:
        *fsm = finish;
        break;

      default:
        break;
    }
  }
}

void tetrisAct() {
  FiniteStateMachine *fsm = getInstanceFsm();
  switch (*fsm) {
    case start:
#ifdef LOGS
      printLog("start\n");
#endif  // LOGS
      onStartState();
      break;
    case spawn:
      onSpawnState();
      break;
    case moving:
      onMovingState();
      break;
    case shift:
      onShiftingState();
      break;
    case pause:
      onPauseState();
      break;
    case attaching:
      onAttachingState();
      break;
    case gameOver:
      onGameOverState();
      break;
    case finish:
      break;
    default:
      break;
  }
}