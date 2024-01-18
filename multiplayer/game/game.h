#ifndef __GAME_H 
#define __GAME_H

#define BALLCOLOR Green
#define PADDLECOLOR Green

#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"

typedef struct ballStruct {
	int xPos;
	int yPos;
	int xSpeed;
	int ySpeed;
} ballType;

typedef struct paddleStruct {
	int xPos;
	int yPos;
	int xSpeed;
} paddleType;

enum gameStateEnum {TITLESCREEN, INGAME, GAMEPAUSED, GAMEOVER};

#endif

void DrawWalls(uint16_t color);
void DrawPaddle(int x0, int y0, uint16_t color);
void DrawBall(int x0, int y0, uint16_t color);
void DrawScore(uint8_t player, uint16_t color);
void UpdateScore(uint8_t score);
void SetPaddle(uint8_t paddle, int x0, int y0, int xSpeed);
void SetPaddle1Speed(int xSpeed);
void SetBall(int x0, int y0, int xSpeed, int ySpeed);
void MovePaddle(void);
void MoveBall(void);
void HitSound(uint8_t hitPaddle);
void GameReset(void);
void GameStart(void);
void GamePause(void);
void GameOver(uint8_t winningPlayer);
void CollisionCheck(void);
