# LandTiger Pong

This repository contains a simplified version of Pong, a game originally developed by Atari, written for the [LandTiger board](https://os.mbed.com/users/wim/notebook/landtiger-baseboard/) during the Computers architectures course @ Politecnico di Torino.

[🌐 Qui in italiano](README_it.md)

## Features
- Play a single-player version of the game, in which the best score is registered
- Play a multiplayer version of the game against the computer

## Technology

The project was developed in C to study the low-level functioning of the board.  
The software has four different functioning states, `TITLESCREEN`, `INGAME`, `GAMEPAUSED` and `GAMEOVER`.  
When a game is started, the timer 0 starts the analog-to-digital conversion on the potentiometer: the result of the conversion will be used at the following tick of the timer to determine the position of the paddle.  
After moving the paddle according to its velocity, a collision check for the ball is run, verifying possible collisions with the walls or with the paddle itself. In such a case, the speed of the ball is reversed (ensuring a reflection whose angle is equal to the angle of incidence). If the ball collided with the paddle, its horizontal velocity is determined by the velocity of the paddle too, resulting in a less predictable gameplay. In case of collision, an appropriate sound is played using timer 1.  
Eventually, after updating the score in case of collision, the ball is moved and the process is then repeated at the following tick of the timer.

## Screenshots

| ![Initial screen](images/Initial_screen.png) Initial screen | ![Game paused](images/Game_paused.png) Game paused |
| :-------------: | :-------------: |