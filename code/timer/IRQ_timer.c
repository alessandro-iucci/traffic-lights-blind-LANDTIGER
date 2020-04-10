/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"

extern unsigned int AD_current;
extern unsigned int state;
unsigned int count=0;
unsigned int i=1;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


void TIMER0_IRQHandler (void)
{
  switch (state) {
		case(0): {
			if(count<2){
				count++;
			}
			else {
				state=1;
				reset_timer(1);
				enable_timer(1);
				if((LPC_GPIO2->FIOPIN & (1<<10)) != 0)
					disable_timer(2);
				count=0;
			}
			break;	
		}
		case(1): {
			state=2;
			disable_timer(0);
			disable_timer(1);
			if((LPC_GPIO2->FIOPIN & (1<<10)) != 0)
				disable_timer(2);
			LED_Out(0x88);
			break;
		}
		case(2):{
			state=3;
			enable_timer(0);
			LED_Out(0x90);
			break;
		}
		case(3):{
			enable_timer(2);
			reset_timer(1);
			enable_timer(1);
			i=1;
			state=0;
			LED_Out(0x60);
			break;
		}
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{	
	switch(state) {
		case(0):{
			if(i%4==0){
				i=1;
				enable_timer(2);
			}
			else if (i%4==1){
				i++;
			}
			else if (i%4==2){
				i++;
				if((LPC_GPIO2->FIOPIN & (1<<10)) != 0)
					disable_timer(2);
			}
			else if (i%4==3){
				i++;
			}
			break;
		}
		case(1):{
			if((LPC_GPIO2->FIOSET & (1 << 6)) == 0) {
				LED_On(6);
				enable_timer(2);
			}
			else {
				LED_Off(6);
				if((LPC_GPIO2->FIOPIN & (1<<10)) != 0)
					disable_timer(2);
			}
			break;
		}
		case(4):{
			if(i%4==0){
				i=1;
				LED_Out(0x90);
				enable_timer(2);
			}
			else if (i%4==1){
				i++;
			}
			else if (i%4==2){
				i++;
				LED_Out(0x0);
				disable_timer(2);
			}
			else if (i%4==3){
				i++;
			}
			break;
		}
	}
		
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
	return;
}

void TIMER2_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (AD_current*SinTable[ticks]/0xFFF)<<5;
	ticks++;
	if(ticks==45) ticks=0;

	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
