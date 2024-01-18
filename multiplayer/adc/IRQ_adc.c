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
				SetPaddle1Speed(-15);
				break;
			
			case 1:
				SetPaddle1Speed(-10);
				break;
			
			case 2:
				SetPaddle1Speed(-5);
				break;
			
			case 3:
				SetPaddle1Speed(0);
				break;
			
			case 4:
				SetPaddle1Speed(5);
				break;
			
			case 5:
				SetPaddle1Speed(10);
				break;
			
			case 6:
				SetPaddle1Speed(15);
				break;
			
			case 7:
				SetPaddle1Speed(15);
				break;
			
			default:
				SetPaddle1Speed(0);
				break;
		}
		
		AD_last = AD_current;
  }	
}
