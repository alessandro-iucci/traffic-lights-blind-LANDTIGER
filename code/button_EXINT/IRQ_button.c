#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"

extern int down;
extern int down1;
extern int down2;
extern unsigned int state;
extern unsigned int count;
extern unsigned int i;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn); 		/*disable button interrupt				 */
	LPC_PINCON->PINSEL4			&= ~(1 <<	20); 		/*GPIO pin selection		 */
	down=1;
	
	enable_timer(2);
	
	switch (state) {
		case(0): {
			i=1;
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			break;	
		}
		case(1): {
			state=0;
			i=1;
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			LED_Out(0x60);
			break;
		}
		case(2):{
			reset_timer(0);
			enable_timer(0);
			break;
		}
	}
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn); 		/*disable button interrupt				 */
	LPC_PINCON->PINSEL4			&= ~(1 <<	22); 		/*GPIO pin selection		 */
	down1=1;	
	
	switch (state) {
		case(0): {
			enable_timer(2);
			i=1;
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			break;	
		}
		case(1): {
			state=0;
			i=1;
			enable_timer(2);
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			LED_Out(0x60);
			break;
		}
		case(2):{
			reset_timer(0);
			enable_timer(0);
			break;
		}
	}
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt				*/
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn); 		/*disable button interrupt				 */
	LPC_PINCON->PINSEL4			&= ~(1 <<	24); 		/*GPIO pin selection		 */
	down2=1;
	
	switch (state) {
		case(0): {
			enable_timer(2);
			i=1;
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			break;	
		}
		case(1): {
			state=0;
			i=1;
			enable_timer(2);
			reset_timer(0);
			enable_timer(0);
			reset_timer(1);
			enable_timer(1);
			count=0;
			LED_Out(0x60);
			break;
		}
		case(2):{
			reset_timer(0);
			enable_timer(0);
			break;
		}
	}
	
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt				*/
}

