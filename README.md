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
and then if you want to play, type
```shell
make play
```

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

## figures in tetris

```c
i
  *  
  *  
  *  
  *  
     

t - blue
     
  *  
 *** 
     
     

l - yellow
     
 **  
  *  
  *  
     

r - orange
     
  ** 
  *  
  *  
     

z - purple
     
 **  
  ** 
     
     

s - green
     
  ** 
 **  
     
     

o - pink

 **  
 **  
     
```


@autor https://github.com/Nfcz