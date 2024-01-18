#include <stdio.h>
#include <math.h>
#include "game.h"

volatile ballType ball;
volatile paddleType paddle1;
volatile paddleType paddle2;
volatile enum gameStateEnum gameState;
volatile unsigned int score1;
volatile unsigned int score2;
uint8_t score2Hidden;

void DrawWalls(uint16_t color) {
	// Muro sinistro
	LCD_DrawLine(0, 42, 0, 287, color);
	LCD_DrawLine(1, 42, 1, 287, color);
	LCD_DrawLine(2, 42, 2, 287, color);
	LCD_DrawLine(3, 42, 3, 287, color);
	LCD_DrawLine(4, 42, 4, 287, color);
	
	// Muro destra
	LCD_DrawLine(239, 42, 239, 287, color);
	LCD_DrawLine(238, 42, 238, 287, color);
	LCD_DrawLine(237, 42, 237, 287, color);
	LCD_DrawLine(236, 42, 236, 287, color);
	LCD_DrawLine(235, 42, 235, 287, color);
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

void DrawScore(uint8_t player, uint16_t color) {
	int i;
	char scoreString[10] = "";
	
	if (player == 1) {
		sprintf(scoreString, "%u", score1);
		GUI_Text(10, 160, (uint8_t *) scoreString, color, Black);
	}
	
	else if (player == 2) {
		sprintf(scoreString, "%u", score2);
		i = 8*(int) floor(log10(score2));		// calcolo di quanti pixel spostarmi se punteggio ha più di 1 cifra
		GUI_Text_Upside_Down(229-i, 174, (uint8_t *) scoreString, color, Black);
	}
}

void UpdateScore(uint8_t score) {
	if (score == 1) {
		DrawScore(1, Black);
		score1++;
		DrawScore(1, White);
	}
	
	else if (score == 2) {
		DrawScore(2, Black);
		score2++;
		DrawScore(2, White);
	}
	
	if (score1 >= 5)	GameOver(1);
	else if (score2 >= 5) GameOver(2);
}

void SetPaddle(uint8_t paddle, int x0, int y0, int xSpeed) {
	if (paddle == 1) {
		DrawPaddle(paddle1.xPos, paddle1.yPos, Black);
		paddle1.xPos = x0;
		paddle1.yPos = y0;
		paddle1.xSpeed = xSpeed;
		DrawPaddle(x0, y0, PADDLECOLOR);
	}
	
	else if (paddle == 2) {
		DrawPaddle(paddle2.xPos, paddle2.yPos, Black);
		paddle2.xPos = x0;
		paddle2.yPos = y0;
		paddle2.xSpeed = xSpeed;
		DrawPaddle(x0, y0, PADDLECOLOR);
	}
}

void SetPaddle1Speed(int xSpeed) {
	paddle1.xSpeed = xSpeed;
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
	int i, xPosTmp1, xPosTmp2;
	
	// PADDLE 1
	
	if (paddle1.xSpeed > 0) {	// paddle 1 si muove verso destra
		for (i=0; i<paddle1.xSpeed && paddle1.xPos+100+i<240; i++) {
			LCD_DrawLine(paddle1.xPos+i, paddle1.yPos, paddle1.xPos+i, paddle1.yPos+9, Black);
			LCD_DrawLine(paddle1.xPos+100+i, paddle1.yPos, paddle1.xPos+100+i, paddle1.yPos+9, PADDLECOLOR);
		}
	}
	
	if (paddle1.xSpeed < 0) {	// paddle 1 si muove verso sinistra
		for (i=0; i<-paddle1.xSpeed && paddle1.xPos-i>0; i++) {
			LCD_DrawLine(paddle1.xPos+99-i, paddle1.yPos, paddle1.xPos+99-i, paddle1.yPos+9, Black);
			LCD_DrawLine(paddle1.xPos-1-i, paddle1.yPos, paddle1.xPos-1-i, paddle1.yPos+9, PADDLECOLOR);
		}
	}
	
	xPosTmp1 = paddle1.xPos + paddle1.xSpeed;
	if (xPosTmp1 < 0)
		paddle1.xPos = 0;
	
	else if (xPosTmp1 > 140)
		paddle1.xPos = 140;
	
	else
		paddle1.xPos = xPosTmp1;
	
	
	// PADDLE 2
	
	if (paddle2.xSpeed > 0) {	// paddle 1 si muove verso destra
		for (i=0; i<paddle2.xSpeed && paddle2.xPos+100+i<240; i++) {
			LCD_DrawLine(paddle2.xPos+i, paddle2.yPos, paddle2.xPos+i, paddle2.yPos+9, Black);
			LCD_DrawLine(paddle2.xPos+100+i, paddle2.yPos, paddle2.xPos+100+i, paddle2.yPos+9, PADDLECOLOR);
		}
	}
	
	if (paddle2.xSpeed < 0) {	// paddle 1 si muove verso sinistra
		for (i=0; i<-paddle2.xSpeed && paddle2.xPos-i>0; i++) {
			LCD_DrawLine(paddle2.xPos+99-i, paddle2.yPos, paddle2.xPos+99-i, paddle2.yPos+9, Black);
			LCD_DrawLine(paddle2.xPos-1-i, paddle2.yPos, paddle2.xPos-1-i, paddle2.yPos+9, PADDLECOLOR);
		}
	}
	
	xPosTmp2 = paddle2.xPos + paddle2.xSpeed;
	if (xPosTmp2 <= 0) {
		paddle2.xPos = 0;
		paddle2.xSpeed *= -1;
	}
	
	else if (xPosTmp2 >= 140) {
		paddle2.xPos = 140;
		paddle2.xSpeed *= -1;
	}
	
	else
		paddle2.xPos = xPosTmp2;
}

void MoveBall(void) {
	int i, xPosNew, yPosNew;
	static uint8_t score1Hidden = 0;
	
	xPosNew = ball.xPos + ball.xSpeed;
	if (xPosNew<5) xPosNew = 5;
	if (xPosNew>230) xPosNew = 230;
	yPosNew = ball.yPos + ball.ySpeed;
	if (yPosNew<5) yPosNew = 5;
	if (yPosNew>315) yPosNew = 315;
	
	// controllo se pallina sarà sopra score 1
	i = 8*(int) floor(log10(score1));		// calcolo di quanti pixel spostarmi se punteggio ha più di 1 cifra
	if (xPosNew>=5 && xPosNew<=21+i && yPosNew>=156 && yPosNew<=179) {
		if (!score1Hidden) {
			score1Hidden = 1;
			DrawScore(1, Black);
		}
	}
	
	else if (score1Hidden) {
		score1Hidden = 0;
		DrawScore(1, White);
	}
	
	// controllo se pallina sarà sopra score 2
	i = 8*(int) floor(log10(score2));		// calcolo di quanti pixel spostarmi se punteggio ha più di 1 cifra
	if (xPosNew>=215-i && xPosNew<=234 && yPosNew>=156 && yPosNew<=179) {
		if (!score2Hidden) {
			score2Hidden = 1;
			DrawScore(2, Black);
		}
	}
	
	else if (score2Hidden) {
		score2Hidden = 0;
		DrawScore(2, White);
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
	
	score1 = 0;
	score2 = 0;
	score2Hidden = 1;
	
	LCD_Clear(Black);
	GUI_Text(104, 100, (uint8_t *) "PONG", White, Black);
	DrawWalls(Red);
	SetPaddle(1, 70, 288, 0);
	SetPaddle(2, 70, 32, 5);
	SetBall(230, 160, 1, 1);
	DrawScore(1, White);
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

void GameOver(uint8_t winningPlayer) {
	gameState = GAMEOVER;
	
	disable_timer(0);
	reset_timer(0);
	
	if (winningPlayer == 1) {
		GUI_Text_Upside_Down(97, 114, (uint8_t *) "ESOL UOY", White, Black);
		GUI_Text(92, 220, (uint8_t *) "YOU WIN", White, Black);
	}
	
	else if (winningPlayer == 2) {
		
		GUI_Text_Upside_Down(101, 114, (uint8_t *) "NIW UOY", White, Black);
		GUI_Text(88, 220, (uint8_t *) "YOU LOSE", White, Black);
	}
}

void CollisionCheck() {
	// controllo collisione con muro sinistro o muro destro
	if (ball.xPos == 5 || ball.xPos == 230) {
		ball.xSpeed *= -1;
		HitSound(0);
	}
	
	// controllo collisione con paddle 1
	if (ball.yPos == paddle1.yPos-5 && ball.xPos > paddle1.xPos-5 && ball.xPos < paddle1.xPos+100) {
		
		if (paddle1.xSpeed == -15) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 3;
			
			else ball.xSpeed = -4;
		}
		
		else if (paddle1.xSpeed == -10) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 2;
			
			else ball.xSpeed = -3;
		}
		
		else if (paddle1.xSpeed == -5) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 1;
			
			else ball.xSpeed = -2;
		}
		
		else if (paddle1.xSpeed == 5) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 1;
			
			else ball.xSpeed = 2;
		}
		
		else if (paddle1.xSpeed == 10) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 2;
			
			else ball.xSpeed = 3;
		}
		
		else if (paddle1.xSpeed == 15) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 3;
			
			else ball.xSpeed = 4;
		}
		
		ball.ySpeed = -1;
		
		HitSound(1);
	}
	
	// collisione laterale con paddle 1
	if (ball.yPos > paddle1.yPos-5 && ball.yPos < paddle1.yPos+10 && (ball.xPos == paddle1.xPos-5 || ball.xPos == paddle1.xPos+100)) {
		ball.xSpeed *= -1;
		HitSound(1);
	}
	
	// controllo collisione con paddle 2
	if (ball.yPos == paddle2.yPos+10 && ball.xPos > paddle2.xPos-5 && ball.xPos < paddle2.xPos+100) {
		
		if (paddle2.xSpeed == -5) {
			if (ball.xSpeed < 0)
				ball.xSpeed -= 1;
			
			else ball.xSpeed = -2;
		}
		
		else if (paddle2.xSpeed == 5) {
			if (ball.xSpeed > 0)
				ball.xSpeed += 1;
			
			else ball.xSpeed = 2;
		}
		
		ball.ySpeed = 1;
		
		HitSound(1);
	}
	
	// collisione laterale con paddle 2
	if (ball.yPos > paddle2.yPos-5 && ball.yPos < paddle2.yPos+10 && (ball.xPos == paddle2.xPos-5 || ball.xPos == paddle2.xPos+100)) {
		ball.xSpeed *= -1;
		HitSound(1);
	}
	
	if (ball.yPos == 315) {		// palla persa
		UpdateScore(2);
		DrawScore(2, Black);
		score2Hidden = 1;
		SetBall(230, 160, 1, 1);
	}
	
	else if (ball.yPos == 0) {		// palla vinta
		UpdateScore(1);
		DrawScore(2, Black);
		score2Hidden = 1;
		SetBall(230, 160, 1, 1);
	}
}
