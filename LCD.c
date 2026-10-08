#include <LPC17xx.h>
void delay(long int x)
{
int i;
for(i=0;i<x;i++);
}
int main()
{
unsigned char cmd[]={0x38,0x0E,0x06,0x01,0x80};
unsigned char msg1[]="JAWADKHAN";
unsigned char i;
//ENABLE AND DIR FOR PIN 11,12,13
LPC_GPIO2 -> FIOMASK1=0xC7;
LPC_GPIO2 -> FIODIR1=0x38;
//ENABLE AND DIR PIN 21-28
LPC_GPIO0 -> FIOMASKH=0xF00E;
LPC_GPIO0 -> FIODIRH=0x0FF0;
//Command
LPC_GPIO2 -> FIOCLR1=0x08;//RS=0
LPC_GPIO2 -> FIOCLR1=0x10;//R/W=0
for(i=0;i<5;i++)
{
LPC_GPIO0 -> FIOPINH=cmd[i]<<5;
LPC_GPIO2 -> FIOSET1=0x20;//EN=1
delay(500);
LPC_GPIO2 -> FIOCLR1=0x20;//EN=1
delay(50000);
}
//DISPLAY
LPC_GPIO2 -> FIOSET1=0x08;//RS=1
for(i=0;msg1[i]!='\0';i++)
{
LPC_GPIO0 -> FIOPINH=msg1[i]<<5;
	LPC_GPIO2 -> FIOSET1=0x20;//EN=1
delay(500);
LPC_GPIO2 -> FIOCLR1=0x20;//EN=0
delay(50000);
}
}