#include <LPC17xx.h>
void delay(long int x)
{ int i;
	for(i=0;i<x;i++);
}
int main()
{
int var1=0;
int i;
 SystemInit();
LPC_GPIO0->FIOMASK2=0xDF;
LPC_GPIO0->FIODIR2=0x20;
while(1)
{
LPC_GPIO0->FIOCLR2=0x20;
LPC_GPIO0->FIOSET2=var1;
var1=var1>>1|((~var1&1)<<3);
delay(500000);

}
}