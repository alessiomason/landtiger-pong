/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../game/game.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
unsigned int potmeterValue;

void ADC_IRQHandler(void) {
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
	
  if(AD_current != AD_last) {
		potmeterValue = AD_current*7/0xFFF;	// ad_current : AD_max = potmeterValue : 7 --> divido potenziometro in 6 aree (6a e 7a sono uguali)
		switch (potmeterValue) {
			case 0:
				SetPaddleSpeed(-15);
				break;
			
			case 1:
				SetPaddleSpeed(-10);
				break;
			
			case 2:
				SetPaddleSpeed(-5);
				break;
			
			case 3:
				SetPaddleSpeed(0);
				break;
			
			case 4:
				SetPaddleSpeed(5);
				break;
			
			case 5:
				SetPaddleSpeed(10);
				break;
			
			case 6:
				SetPaddleSpeed(15);
				break;
			
			case 7:
				SetPaddleSpeed(15);
				break;
			
			default:
				SetPaddleSpeed(0);
				break;
		}
		
		AD_last = AD_current;
  }	
}
