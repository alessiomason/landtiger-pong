#include <stdio.h>
#include <math.h>
#include "game.h"

volatile ballType ball;
volatile paddleType paddle;
volatile enum gameStateEnum gameState;
volatile unsigned int score;
volatile unsigned int bestScore = 100;

void DrawWalls(uint16_t color) {
	// Muro sinistro
	LCD_DrawLine(0, 0, 0, 287, color);
	LCD_DrawLine(1, 0, 1, 287, color);
	LCD_DrawLine(2, 0, 2, 287, color);
	LCD_DrawLine(3, 0, 3, 287, color);
	LCD_DrawLine(4, 0, 4, 287, color);
	
	// Muro sopra
	LCD_DrawLine(0, 0, 240, 0, color);
	LCD_DrawLine(0, 1, 240, 1, color);
	LCD_DrawLine(0, 2, 240, 2, color);
	LCD_DrawLine(0, 3, 240, 3, color);
	LCD_DrawLine(0, 4, 240, 4, color);
	
	// Muro destra
	LCD_DrawLine(239, 0, 239, 287, color);
	LCD_DrawLine(238, 0, 238, 287, color);
	LCD_DrawLine(237, 0, 237, 287, color);
	LCD_DrawLine(236, 0, 236, 287, color);
	LCD_DrawLine(235, 0, 235, 287, color);
}

void DrawPaddle(int x0, int y0, uint16_t color) {
	int i;
	for (i=0; i<10; i++)
		LCD_DrawLine(x0, y0+i, x0+99, y0+i, color);
}

void DrawBall(int x0, int y0, uint16_t color) {
	int i;
	for (i=0; i<5; i++)
		LCD_DrawLine(x0, y0+i, x0+4, y0+i, color);
}

void DrawScore(uint16_t color) {
	char scoreString[10] = "";
	
	sprintf(scoreString, "%u", score);
	GUI_Text(10, 160, (uint8_t *) scoreString, color, Black);
}

void DrawBestScore(uint16_t color) {
	char scoreString[20] = "";
	int i;
	
	sprintf(scoreString, "Best: %u", bestScore);
	i = 8*(int) floor(log10(bestScore/100));		// calcolo di quanti pixel spostarmi se punteggio ha più di 3 cifre
	GUI_Text(160-i, 10, (uint8_t *) scoreString, color, Black);
}

void UpdateScore(void) {
	DrawScore(Black);
	score = score>100 ? score+10 : score+5;
	DrawScore(White);
	
	if (score > bestScore) {
		DrawBestScore(Black);
		bestScore = score;
		DrawBestScore(White);
	}
}

void SetPaddle(int x0, int y0, int xSpeed) {
	DrawPaddle(paddle.xPos, paddle.yPos, Black);
	paddle.xPos = x0;
	paddle.yPos = y0;
	paddle.xSpeed = xSpeed;
	DrawPaddle(x0, y0, PADDLECOLOR);
}

void SetPaddleSpeed(int xSpeed) {
	paddle.xSpeed = xSpeed;
}

void SetBall(int x0, int y0, int xSpeed, int ySpeed) {
	DrawBall(ball.xPos, ball.yPos, Black);
	ball.xPos = x0;
	ball.yPos = y0;
	ball.xSpeed = xSpeed;
	ball.ySpeed = ySpeed;
	DrawBall(x0, y0, BALLCOLOR);
}

void MovePaddle(void) {
	int i, xPosTmp;
	
	if (paddle.xSpeed > 0) {	// paddle si muove verso destra
		for (i=0; i<paddle.xSpeed && paddle.xPos+100+i<240; i++) {
			LCD_DrawLine(paddle.xPos+i, paddle.yPos, paddle.xPos+i, paddle.yPos+9, Black);
			LCD_DrawLine(paddle.xPos+100+i, paddle.yPos, paddle.xPos+100+i, paddle.yPos+9, PADDLECOLOR);
		}
	}
	
	if (paddle.xSpeed < 0) {	// paddle si muove verso sinistra
		for (i=0; i<-paddle.xSpeed && paddle.xPos-i>0; i++) {
			LCD_DrawLine(paddle.xPos+99-i, paddle.yPos, paddle.xPos+99-i, paddle.yPos+9, Black);
			LCD_DrawLine(paddle.xPos-1-i, paddle.yPos, paddle.xPos-1-i, paddle.yPos+9, PADDLECOLOR);
		}
	}
	
	xPosTmp = paddle.xPos + paddle.xSpeed;
	if (xPosTmp < 0)
		paddle.xPos = 0;
	
	else if (xPosTmp > 140)
		paddle.xPos = 140;
	
	else
		paddle.xPos = xPosTmp;
}

void MoveBall(void) {
	int i, xPosNew, yPosNew;
	static uint8_t scoreHidden = 0, bestScoreHidden = 0;
	
	xPosNew = ball.xPos + ball.xSpeed;
	if (xPosNew<5) xPosNew = 5;
	if (xPosNew>230) xPosNew = 230;
	yPosNew = ball.yPos + ball.ySpeed;
	if (yPosNew<5) yPosNew = 5;
	if (yPosNew>315) yPosNew = 315;
	
	i = 8*(int) floor(log10(score));		// calcolo di quanti pixel spostarmi se punteggio ha più di 1 cifra
	
	// controllo se pallina sarà sopra score
	if (xPosNew>=5 && xPosNew<=21+i && yPosNew>=156 && yPosNew<=179) {
		if (!scoreHidden) {
			scoreHidden = 1;
			DrawScore(Black);
		}
	}
	
	else if (scoreHidden) {
		scoreHidden = 0;
		DrawScore(White);
	}
	
	i = 8*(int) floor(log10(bestScore/100));		// calcolo di quanti pixel spostarmi se punteggio ha più di 3 cifre
	
	// controllo se pallina sarà sopra bestScore
	if (xPosNew>=156-i && xPosNew<=234 && yPosNew>=5 && yPosNew<=29) {
		if (!bestScoreHidden) {
			bestScoreHidden = 1;
			DrawBestScore(Black);
		}
	}
	
	else if (bestScoreHidden) {
		bestScoreHidden = 0;
		DrawBestScore(White);
	}
	
	if (ball.xSpeed > 0) {	// pallina si muove verso destra
		for (i=0; i<ball.xSpeed && ball.xPos+5+i<235; i++) {
			LCD_DrawLine(ball.xPos+i, ball.yPos, ball.xPos+i, ball.yPos+4, Black);
			LCD_DrawLine(ball.xPos+5+i, ball.yPos, ball.xPos+5+i, ball.yPos+4, BALLCOLOR);
		}
	}
		
	if (ball.xSpeed < 0) {	// pallina si muove verso sinistra
		for (i=0; i<-ball.xSpeed && ball.xPos-1-i>4; i++) {
			LCD_DrawLine(ball.xPos+4-i, ball.yPos, ball.xPos+4-i, ball.yPos+4, Black);
			LCD_DrawLine(ball.xPos-1-i, ball.yPos, ball.xPos-1-i, ball.yPos+4, BALLCOLOR);
		}
	}
	
	ball.xPos = xPosNew;
	
	if (ball.ySpeed > 0) {	// pallina si muove verso il basso
		for (i=0; i<ball.ySpeed && ball.yPos+i<316; i++) {
			LCD_DrawLine(ball.xPos, ball.yPos+i, ball.xPos+4, ball.yPos+i, Black);
			LCD_DrawLine(ball.xPos, ball.yPos+5+i, ball.xPos+4, ball.yPos+5+i, BALLCOLOR);
		}
	}
		
	if (ball.ySpeed < 0) {	// pallina si muove verso l'alto
		for (i=0; i<-ball.ySpeed && ball.yPos-i>4; i++) {
			LCD_DrawLine(ball.xPos, ball.yPos+4-i, ball.xPos+4, ball.yPos+4-i, Black);
			LCD_DrawLine(ball.xPos, ball.yPos-1-i, ball.xPos+4, ball.yPos-1-i, BALLCOLOR);
		}
	}

	ball.yPos = yPosNew;
}

void HitSound(uint8_t hitPaddle) {
	reset_timer(1);
	
	if (hitPaddle)
		init_timer(1, 0xDDAE);		// 0.00227 s * 25MHz = 56750 = 0xDDAE		--> La
	
	else
		init_timer(1, 0x14C08);		// 0.0034 s * 25MHz = 85000 = 0x14C08		--> Re
	
	enable_timer(1);
}

void GameReset(void) {
	gameState = TITLESCREEN;
	
	disable_timer(0);
	reset_timer(0);
	
	ball.xPos = 230;
	ball.yPos = 160;
	paddle.xPos = 70;
	paddle.yPos = 288;
	score = 0;
	
	LCD_Clear(Black);
	GUI_Text(104, 100, (uint8_t *) "PONG", White, Black);
	DrawWalls(Red);
	SetPaddle(paddle.xPos, paddle.yPos, 0);
	SetBall(ball.xPos, ball.yPos, 1, 1);
	DrawScore(White);
	DrawBestScore(White);
	GUI_Text(28, 200, (uint8_t *) "Premi KEY1 per iniziare", White, Black);
}

void GameStart(void) {
	gameState = INGAME;
	
	GUI_Text(104, 100, (uint8_t *) "PONG", Black, Black);
	GUI_Text(28, 200, (uint8_t *) "Premi KEY1 per iniziare", Black, Black);
	
	enable_timer(0);
}

void GamePause(void) {
	
	if (gameState == INGAME) {
		disable_timer(0);
		gameState = GAMEPAUSED;
		GUI_Text(76, 100, (uint8_t *) "GAME PAUSED", White, Black);
	}
	
	else if (gameState == GAMEPAUSED) {
		gameState = INGAME;
		GUI_Text(76, 100, (uint8_t *) "GAME PAUSED", Black, Black);
		if (ball.xPos>=72 && ball.xPos<=118 && ball.yPos>=96 && ball.yPos<=119)
			DrawBall(ball.xPos, ball.yPos, BALLCOLOR);
		enable_timer(0);
	}
}

void GameOver(void) {
	gameState = GAMEOVER;
	
	disable_timer(0);
	reset_timer(0);
	
	GUI_Text(88, 100, (uint8_t *) "YOU LOSE", White, Black);
}

void CollisionCheck() {
	// controllo collisione con muro sinistro o muro destro
	if (ball.xPos == 5 || ball.xPos == 230) {
		ball.xSpeed *= -1;
		HitSound(0);
	}
	
	// controllo collisione con muro sopra
	if (ball.yPos == 5) {
		ball.ySpeed *= -1;
		HitSound(0);
	}
	
	// controllo collisione con paddle
	if (ball.yPos == paddle.yPos-5 && ball.xPos > paddle.xPos-5 && ball.xPos < paddle.xPos+100) {
		
		if (paddle.xSpeed == -15) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 3;
			
			else ball.xSpeed = -4;
		}
		
		else if (paddle.xSpeed == -10) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 2;
			
			else ball.xSpeed = -3;
		}
		
		else if (paddle.xSpeed == -5) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 1;
			
			else ball.xSpeed = -2;
		}
		
		else if (paddle.xSpeed == 5) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 1;
			
			else ball.xSpeed = 2;
		}
		
		else if (paddle.xSpeed == 10) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 2;
			
			else ball.xSpeed = 3;
		}
		
		else if (paddle.xSpeed == 15) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 3;
			
			else ball.xSpeed = 4;
		}
		
		ball.ySpeed = -1;
		
		HitSound(1);
		UpdateScore();
	}
	
	// collisione laterale con paddle
	if (ball.yPos > paddle.yPos-5 && ball.yPos < paddle.yPos+10 && (ball.xPos == paddle.xPos-5 || ball.xPos == paddle.xPos+100)) {
		ball.xSpeed *= -1;
		HitSound(1);
	}
	
	if (ball.yPos == 315)		// palla persa
		GameOver();
}
