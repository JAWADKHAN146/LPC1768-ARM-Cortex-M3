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
	LPC_TIM0->TCR=0X01;//TIMER IS ENBLED
	while(LPC_TIM0->TC<  microsecond)
	{
	}
	LPC_TIM0->TCR=0x00;
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
	unsigned int time, dis,temp[3];
	unsigned char str1[]="THE DISTANCE IS";
  unsigned char cmd[]  = {0x02,0x28,0x0E,0x06,0x01,0x80};
	SystemInit();
	initTimer0();
	//--------LCD-----------P0.24-27(D4-D7)-----
	LPC_GPIO0->FIOMASK3=0xF0;
	LPC_GPIO0->FIODIR3=0x0F;
  //-------RS,R/W,EN-------P2.11,12,13--------
	LPC_GPIO2->FIOMASK1 = 0xC7;
	LPC_GPIO2->FIODIR1  = 0x38;
	//---------buzzer-------P0.7---------------
	LPC_GPIO0->FIOMASK0=0X7F;
	LPC_GPIO0->FIODIR0=0X80;
	//--------LED-----------P1.29---------------
	LPC_GPIO1->FIOMASK3=0XDF; 
	LPC_GPIO1->FIODIR3=0X20; 
	LPC_GPIO1->FIOCLR3=0X20;
	//---------Trig(P0.21)--Echo(P0.22)-----------
	LPC_GPIO0->FIOMASK2=0x9F;//10011111
	LPC_GPIO0->FIODIR2=0x20;//001000000
  SystemInit();
	initTimer0();
	delayUS(50000);
	// LCD Initialization
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
	while(1)
	{ 
		LPC_GPIO0->FIOCLR2 = 0x20;
    delayUS(2);
		LPC_GPIO0->FIOSET2=0X20;//00100000
		delayUS(10);
		LPC_GPIO0->FIOCLR2=0X20;
		while((LPC_GPIO0->FIOPIN2&0x40)==0);
		LPC_TIM0->TCR=0x02;//RESETTING TIMER
		LPC_TIM0->TCR=0X01;//TIMER IS ENBLED
		while((LPC_GPIO0->FIOPIN2 &0x40)!=0);
		LPC_TIM0->TCR = 0x00;
		time = LPC_TIM0->TC;
		dis = (time * 0.0343)/2;
			  if(dis<=5)
			  {  
					LPC_GPIO1->FIOSET3=0X20;
					delayUS(100000);
					LPC_GPIO1->FIOCLR3=0X20;
					delayUS(100000);
					LPC_GPIO0->FIOSET1=0X80;
					delayUS(100000);
					LPC_GPIO0->FIOCLR1=0X80;
		}
				else 
				{
					LPC_GPIO1->FIOCLR3=0X20;
				}
				for(i=0; i<3; i++)
{
    temp[i] = dis % 10;
    dis = dis / 10;
}
   // Second line
            LPC_GPIO2->FIOCLR1 = 0x08;
            lcd_4(0xC0);

            LPC_GPIO2->FIOSET1 = 0x08;
              for(i=2;i>=0;i--)
			  {
              lcd_4(temp[i]+0x30);
			  }
	}
}
