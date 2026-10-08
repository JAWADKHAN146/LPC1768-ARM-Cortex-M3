#include <LPC17xx.h>
void delay(unsigned long int x)
{
	int i;
	for (i=0;i<x;i++);
}
int main()
{
	unsigned char cmd[]={0x38,0x0E,0x06,0x01,0x80};
	unsigned char str[] ="welcome";
	unsigned int i;
	//PIN P1.11-13 
	LPC_GPIO1->FIOMASK1=0XC7; //11000111
	LPC_GPIO1->FIODIR1=0X38;//00111000
	//PIN P0.21-28
	LPC_GPIO0->FIOMASKH=0XE01F;//1110000000011111
	LPC_GPIO0->FIODIRH=0X1FE0;//0001111111100000
	//SENDING CMD
	LPC_GPIO1->FIOCLR1=0X08;//RS=0
	LPC_GPIO1->FIOCLR1=0X01;//R/W=0
	for(i=0;i<5;i++)
	{
		LPC_GPIO0->FIOPINH=cmd[i]<<5;
		LPC_GPIO1->FIOSET1=0X02;//EN=1
		delay(0x5000);
		LPC_GPIO1->FIOCLR1=0X02;//EN=0;
	}
	LPC_GPIO1->FIOSET1=0X08;//RS=1
			for(i=0;str[i]!='\0';i++)
		{
			LPC_GPIO0->FIOPINH=str[i]<<5;
			LPC_GPIO1->FIOSET1=0X20;//EN=1
			delay(0x5000);
			LPC_GPIO1->FIOCLR1=0X20;//EN=0
			delay(0x5000);
		}
}