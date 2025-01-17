# finite-state machine table

state\input | Start     | Pause | Terminate | Left  | Right | Up    | Down  | Action
------------|-----------|-------|-----------|-------|-------|-------|-------|-------
start       |   spawn   |   -   |finish     |   -   |   -   |   -   |   -   |   -   |
spawn       |moving     |moving |finish     |moving |moving |moving |moving |moving |
moving      |   -       |pause  |finish     |moving |moving |   -   |shift  |moving |
attaching   |spawn/gameovere|spawn/gameovere|finish|spawn/gameovere|spawn/gameovere|spawn/gameovere|spawn/gameovere|spawn/gameovere|
shift       |moving     |moving |finish     |moving |moving |moving |moving |moving |
pause       |   -       |moving |finish     |   -   |   -   |   -   |   -   |   -   |
gameover    |start      |   -   |finish     |   -   |   -   |   -   |   -   |   -   |  


# comments
If the pressed button was only used to move to the next state, we save the if and call `void tetris_act();` a second time to process it correctly.

`finish` is not active state, it is the exit of finite-state machine