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
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int down=0;
int down1=0;
int down2=0;
extern unsigned int state;
extern unsigned int i;

void RIT_IRQHandler (void)
{					
	
	/* button management */
	if(down!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			//continue to make sound			
		}
		else {	/* button released */
			down=0;			
			disable_timer(2);
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */			
		}
		else {	/* button released */
			down1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */		
		}
		else {	/* button released */
			down2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
		
	/* joystick management */
	if(state==0 || state==4){
		if(((LPC_GPIO1->FIOPIN & (1<<29)) == 0) && state==0){	/* Joytick Right pressed */
			state=4;
			enable_timer(2);
			reset_timer(1);
			enable_timer(1);
			i=1;
			LED_Out(0x90);
		}
		else if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && state==4) {
			enable_timer(2);
			reset_timer(1);
			enable_timer(1);
			i=1;
			state=0;
			LED_Out(0x60);
		}
	}
	
		
	/* ADC management */
	if(state==4)
		ADC_start_conversion();		
			
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
