#include <LPC17xx.h>
void delay(unsigned long int x)
{
int i;
for(i=0;i<x;i++);
}
int main()
{
SystemInit();
LPC_GPIO0 -> FIOMASK3=0xF0;//11110000
LPC_GPIO0 -> FIODIR3=0x0F;
while(1)
{
LPC_GPIO0-> FIOSET3=0x0F;
delay(0x500000);
LPC_GPIO0-> FIOCLR3=0x0F;
delay(0x500000);
}
}