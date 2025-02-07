# finite-state machine table

state / input + recall| Start | Pause | Terminate | Left  | Right | Up    | Down  | Action| Timer | recall|
------------|-------|-------|-----------|-------|-------|-------|-------|-------|-------|-------|
start       |spawn  |   -   |finish     |   -   |   -   |   -   |   -   |   -   |   -   | NO
spawn       |moving |moving |finish     |moving |moving |moving |moving |moving |moving | YES
moving      |   -   |pause  |finish     |moving |moving |   -   |moving |moving / attaching|shift  | NO
attaching   |spawn / gameOver|spawn / gameOver|finish     |spawn / gameOver|spawn / gameOver|spawn / gameOver|spawn / gameOver|spawn / gameOver|spawn / gameOver| YES
shift       |moving / attaching|moving / attaching|finish     |moving / attaching|moving / attaching|moving / attaching|moving / attaching|moving / attaching|moving / attaching| YES
pause       |   -   |moving |finish     |   -   |   -   |   -   |   -   |   -   |   -   | NO
gameOver    |start  |   -   |finish     |   -   |   -   |   -   |   -   |   -   |   -   | NO


# comments
## Recall
Recall used from being between calls n 1 of 4 "processed" positions:
- start
- moving
- pause
- gameOver

that way spawn and attaching processed faster - from 1 outside call

## Timer
Timer is used then `InputToFsm_t.timer` is 1, that mean that call was from `userInput`, or `updateCurrentState`, when before the action we should shift figure.

## `finish`

`finish` is not active state, it is the exit of finite-state machine.