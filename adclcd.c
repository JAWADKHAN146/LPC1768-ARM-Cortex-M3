#include <LPC17xx.H>
#include <stdio.h>
void delay(unsigned long int dk)
{
    unsigned long int i;
    for(i = 0; i < dk; i++);
}
unsigned long int di=0;
unsigned short int adc=0;
int main()
{
unsigned int mv;
unsigned char i;
unsigned char cmd[]={0x38,0x0E,0x06,0x01,0x80};
char str[20];
SystemInit();
LPC_SC->PCONP|=0X00001000; 
LPC_GPIO1->FIOMASK3=0XDF; 
LPC_GPIO1->FIODIR3=0X20; 
LPC_PINCON->PINSEL3|=0XC0000000; 
LPC_ADC->ADCR=0X00210320; 
//ENABLE AND DIR FOR PIN 11,12,13
LPC_GPIO2 -> FIOMASK1=0xC7;
LPC_GPIO2 -> FIODIR1=0x38;
//ENABLE AND DIR PIN 21-28
LPC_GPIO0 -> FIOMASKH=0xE01F;
LPC_GPIO0 -> FIODIRH=0x1FE0;
	//SENDING CMD
	LPC_GPIO2->FIOCLR1=0X08;//RS=0
	LPC_GPIO2->FIOCLR1=0X10;//R/W=0
	for(i=0;i<5;i++)
	{
		LPC_GPIO0->FIOPINH=cmd[i]<<5;
		LPC_GPIO2->FIOSET1=0X20;//EN=1
		delay(0x5000);
		LPC_GPIO2->FIOCLR1=0X20;//EN=0;
	}
while(1) 		
{                         
while((LPC_ADC->ADSTAT&0X00000020)!=0X00000020)
	{
	}
adc=((LPC_ADC->ADDR5>>4)& 0x00000FFF); 
mv=(adc*3300)/4095;
sprintf(str,"%u",mv);
}			
//DISPLAY

LPC_GPIO2 -> FIOSET1=0x08;//RS=1
for(i=0;str[i]!='\0';i++)
{
LPC_GPIO0 -> FIOPINH=str[i]<<5;
	LPC_GPIO2 -> FIOSET1=0x20;//EN=1
delay(50000);
LPC_GPIO2 -> FIOCLR1=0x20;//EN=0
delay(50000);
} 
}
