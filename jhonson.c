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
LPC_GPIO0->FIOMASK1=0xF0;
LPC_GPIO0->FIODIR1=0x0F;
while(1)
{
LPC_GPIO0->FIOCLR1=0x0F;
LPC_GPIO0->FIOSET1=var1;
var1=var1>>1|((~var1&1)<<3);
delay(500000);

}
}