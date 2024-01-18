#include "button.h"
#include "lpc17xx.h"
#include "../RIT/RIT.h"
#include "../game/game.h"

extern uint8_t key1;
extern uint8_t key2;
extern uint8_t int0;
extern enum gameStateEnum gameState;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	if (gameState != TITLESCREEN) {
		enable_RIT();										/* enable RIT to count 50ms				 */
		NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
		int0 = 1;
	}
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	if (gameState == TITLESCREEN) {		// posso fare start solo in TitleScreen
		enable_RIT();										/* enable RIT to count 50ms				 */
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
		key1 = 1;
	}
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	if (gameState == INGAME || gameState == GAMEPAUSED) {
		enable_RIT();										/* enable RIT to count 50ms				 */
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
		key2 = 1;
	}
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


