#include <LPC17xx.h>
void delay(long int x)
{int i;
for(i=0;i<x;i++);
}
int main()
{
unsigned char var;
SystemInit();
	//switch
LPC_GPIO0 -> FIOMASK0 =0xFE;//11110111
LPC_GPIO0 -> FIODIR0 =0x00;//11110111
LPC_GPIO1 -> FIOMASK3=0xDF;//11011111
LPC_GPIO1 -> FIODIR3=0x20;//00010000

//led

while(1)
{
var=LPC_GPIO0 ->FIOPIN0;
if(var==0x01)
{
LPC_GPIO1 -> FIOSET3=0x20;//00010000
}
else
{
LPC_GPIO1 -> FIOCLR3=0x20;//00010000
}
}
}