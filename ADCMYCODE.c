#include <LPC17xx.h>
void delay(unsigned int x)
{ int i;
for(i=0;i<x;i++);
}
int main()
{
	unsigned short int adc;
SystemInit();
LPC_SC -> PCONP|=0x00001000;//Power control for ADC is made as high
LPC_GPIO1 ->FIOMASK3=0xDF;//P0.29,LED
LPC_GPIO1 ->FIODIR3=0x20;
LPC_PINCON ->PINSEL3=|0xC0000000;//P1.31 AS AD0.5
LPC_ADC -> ADCR= 0x00210320;
while(1)
{
while((LPC_ADC -> ADSTAT & 0x20)!= 0x20)
{
}
adc=((LPC_ADC->ADDR5>>4)&0x00000FFF);
if (adc > 0x9B2) //0x9B2=2V
{
LPC_GPIO1->FIOSET3=0x20;
}
else
{
LPC_GPIO1->FIOCLR3=0x20;
}
}
}