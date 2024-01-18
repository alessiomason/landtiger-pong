/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile uint8_t key1=0;
volatile uint8_t key2=0;
volatile uint8_t int0=0;
uint8_t buttonReleased = 0;

void RIT_IRQHandler (void)
{
	if (int0 != 0) {
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0) {			// verifico se pulsante è ancora premuto
			int0++;
			reset_RIT();
			switch(int0) {
				case 2:
					GameReset();
					break;
				default:
					break;
			}
		}
		
		else buttonReleased = 1;
	}
	
	else if (key1 != 0) {
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0) {			// verifico se pulsante è ancora premuto
			key1++;
			reset_RIT();
			switch(key1) {
				case 2:
					GameStart();
					break;
				default:
					break;
			}
		}
		
		else buttonReleased = 1;
	}
	
	else if (key2 != 0) {
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0) {			// verifico se pulsante è ancora premuto
			key2++;
			reset_RIT();
			switch(key2) {
				case 2:
					GamePause();
					break;
				default:
					break;
			}
		}
		
		else buttonReleased = 1;
	}

	if ((key1!=0 || key2!=0 || int0!=0) && buttonReleased) {	/* button released */
		key1 = 0;
		key2 = 0;
		int0 = 0;
		buttonReleased = 0;
		disable_RIT();
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		NVIC_EnableIRQ(EINT1_IRQn);
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		NVIC_EnableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */	
	}
		
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
