#include <LPC17xx.h>
void initTimer0(void)
{
	//pconp and pclksel is not required for timer/counter 0,1
	LPC_TIM0->CTCR=0x0;//setting to timer mode
	LPC_TIM0->PR=(18-1);//PRESCALE REGISTER COUNT
	LPC_TIM0->TCR=0x02;//RESTING TIMER
}
void delayUS(unsigned int microsecond)//using timer0
{
	LPC_TIM0->TCR=0x02;//RESETTING TIMER
	LPC_TIM0->TCR=0X01;
	while(LPC_TIM0->TC< microsecond)
	{
	}
	LPC_TIM0->TCR=0x00;
}
int main()
{
	//--------LED-----------P1.29---------------
	LPC_GPIO1->FIOMASK3=0XDF; 
	LPC_GPIO1->FIODIR3=0X20; 
	LPC_GPIO1->FIOCLR3=0X20;
	LPC_GPIO0->FIOMASK3=0X0EF;//11101111
  LPC_GPIO0->FIODIR3=0X10;
	
	LPC_GPIO0->FIOMASK0=0XFE; 
	LPC_GPIO0->FIODIR0=0X01; 
	initTimer0();
	SystemInit();
	while(1)
	{ LPC_GPIO0->FIOSET3=0X10;
		LPC_GPIO1->FIOSET3=0X20;
		LPC_GPIO0->FIOSET0=0X01; 
		delayUS(100000);
		LPC_GPIO0->FIOCLR3=0X10;
		LPC_GPIO1->FIOCLR3=0X20;
		LPC_GPIO0->FIOCLR0=0X01; 
		delayUS(100000);
	}
}
