# Brick game. Tetris
Tetris is one from brick game console. This '90 s console is a classic that's fun to bring to life yourself.

## what's in the project
- library with all Tetris game logic
- console output based on ncurses
- basic tests to the library

## how to install
if you in folder, type:
```shell
make install
```
if you want to play, type
```shell
make play
```

## key's
key     |command
--------|----
"enter" | start game
\>      | move figure right
<       | move figure left
\\/     | move figure to the bottom
"space" | rotate figure
"p"     | pause
"q"     | quit game


## documentation
if you want to read documentation firstly check, that you have `doxygen`
```shell
brew install doxygen
```
and then, from src folder type:
```shell
make dvi
```
if you don't have `open` installed, open **src/documentation/dvi/html/index.html** manually

# testing
for testing in make file in 31 line delete `#`
``` shell
TESTING = #-DTESTING
------
TESTING = -DTESTING
```
and reinstall game
``` shell
make rebuild
```

@author https://github.com/Nfcz