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
LPC_GPIO0 -> FIOMASK0 =0xFC;//input switch P0.0,relay output P0.1
LPC_GPIO0 -> FIODIR0 =0x02;
	
//led

LPC_GPIO1 -> FIOMASK3=0xDF;
LPC_GPIO1 -> FIODIR3=0x20;

while(1)
{
var=(LPC_GPIO0 ->FIOPIN0&0X01);
if(var==0x01)
{
LPC_GPIO1 -> FIOSET3=0x20;
LPC_GPIO0 -> FIOSET0=0x02;
}
else
{
LPC_GPIO1 -> FIOCLR3=0x20;
LPC_GPIO0 -> FIOCLR0=0x02;
}
}
}
