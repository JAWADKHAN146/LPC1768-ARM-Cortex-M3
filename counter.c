#include <LPC17xx.h>
void counterInit()
{
LPC_TIM0->CTCR=0X01;//ctcr{3:2]=00 for cap0.0;[1:0]=01for counter mode for rising edge
LPC_PINCON->PINSEL3|=0X00300000;//P1.26(21:20:)
LPC_TIM0->PR=0;
LPC_TIM0->TCR=0X02;
LPC_TIM0->TCR=0X01;
}
void initTimer0(void)
{
	//pconp and pclksel is not required for timer/counter 0,1
	LPC_TIM1->CTCR=0x0;//setting to timer mode
	LPC_TIM1->PR=(18-1);//PRESCALE REGISTER COUNT
	LPC_TIM1->TCR=0x02;//RESTING TIMER
}
void delayUS(unsigned int microsecond)//using timer0
{
	LPC_TIM1->TCR=0x02;//RESETTING TIMER
	LPC_TIM1->TCR=0X01;
	while(LPC_TIM1->TC< microsecond)
	{
	}
	LPC_TIM1->TCR=0x00;
}
void lcd_4(unsigned char chr)
{
    unsigned char chr1, chr2;
 
    // Upper nibble
    chr1 = chr & 0xF0;
    LPC_GPIO0->FIOPIN3 = chr1 >> 4;

    LPC_GPIO2->FIOSET1 = 0x20;   // EN=1
    delayUS(500);
    LPC_GPIO2->FIOCLR1 = 0x20;   // EN=0
    delayUS(50000);

    // Lower nibble
    chr2 = chr & 0x0F;
    LPC_GPIO0->FIOPIN3 = chr2;

    LPC_GPIO2->FIOSET1 = 0x20;   // EN=1
    delayUS(500);
    LPC_GPIO2->FIOCLR1 = 0x20;   // EN=0
    delayUS(50000);
}
int main()
{	
	int i;
	unsigned int count,temp[3];
	unsigned char str1[]="THE COUNT IS";
  unsigned char cmd[]  = {0x02,0x28,0x0E,0x06,0x01,0x80};
	SystemInit();
	counterInit();
	initTimer0();
	//-------COUNTER--------P1.26----
	LPC_GPIO1->FIOMASK3=0XFB;
	LPC_GPIO1->FIODIR3=0X00;
	//-------LCD----------P0.24-27---
	//--------LCD-----------P0.24-27(D4-D7)-----
	LPC_GPIO0->FIOMASK3=0xF0;
	LPC_GPIO0->FIODIR3=0x0F;
  //-------RS,R/W,EN-------P2.11,12,13--------
	LPC_GPIO2->FIOMASK1 = 0xC7;
	LPC_GPIO2->FIODIR1  = 0x38;
	LPC_GPIO2->FIOCLR1 = 0x08;   // RS=0
	LPC_GPIO2->FIOCLR1 = 0x10;   // RW=0
	for(i=0;i<6;i++){
	lcd_4(cmd[i]);}
	// First line
	LPC_GPIO2 ->FIOSET1=0x08;//RS=1
	for(i=0;str1[i]!='\0';i++) 	 	
		{
			lcd_4(str1[i]);
		}
	//resetting counter and enable
	while(1)
	{
	count=LPC_TIM0->TC;
	
					for(i=0; i<3; i++)
{
    temp[i] = count % 10;
    count = count / 10;
}
   // Second line
            LPC_GPIO2->FIOCLR1 = 0x08;
            lcd_4(0xC0);

            LPC_GPIO2->FIOSET1 = 0x08;
              for(i=2;i>=0;i--)
			  {
              lcd_4(temp[i]+0x30);
			  }
		delayUS(10000);
	}
}
