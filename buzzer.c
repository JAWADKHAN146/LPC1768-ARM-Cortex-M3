#include <LPC17xx.h>
void delay(long int x)
{ int i;
for(i=0;i<x;i++);
}
int main()
{
	SystemInit();
LPC_GPIO0 -> FIOMASK2=0xDF;
LPC_GPIO0 -> FIODIR2=0x20;
while(1)
{
LPC_GPIO0-> FIOSET2=0x20;
delay(0x500000);
LPC_GPIO0-> FIOCLR1=0x20;
delay(0x500000);
}
}