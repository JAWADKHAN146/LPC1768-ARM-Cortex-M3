#include <LPC17xx.h>
void delay(long int x)
{int i;
for(i=0;i<x;i++);
}
int main()
{
	unsigned char var;
unsigned char cmd[]={0x38,0x0E,0x06,0x01,0x80};
unsigned char msg1[]="JAWADKHAN";
unsigned char msg2[]="PESU";
unsigned char i;
SystemInit();
	//switch
LPC_GPIO0 -> FIOMASK0 =0xFE;//11110111
LPC_GPIO0 -> FIODIR0 =0x00;//11110111
LPC_GPIO1 -> FIOMASK3=0xDF;//11101111
LPC_GPIO1 -> FIODIR3=0x20;//00010000
//ENABLE AND DIR FOR PIN 8,9,10
LPC_GPIO2 -> FIOMASK1=0xC7;
LPC_GPIO2 -> FIODIR1=0x38;
//ENABLE AND DIR PIN 0-7
LPC_GPIO0 -> FIOMASKH=0xE01F;
LPC_GPIO0 -> FIODIRH=0x1FE0;
//Command
LPC_GPIO2 -> FIOCLR1=0x08;//RS=0
LPC_GPIO2 -> FIOCLR1=0x10;//R/W=0

//led

while(1)
{
var=LPC_GPIO0 ->FIOPIN0;//00001000
if(var==0x01)
{
	//CMD
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
  // MOVE CURSOR TO LINE 2 POSITION 1
LPC_GPIO2->FIOCLR1=0x08; // RS=0
LPC_GPIO0->FIOPINH=0xC0<<5;

LPC_GPIO2 -> FIOSET1=0x20;//EN=1
delay(500);
LPC_GPIO2 -> FIOCLR1=0x20;//EN=0
delay(50000);
    // DISPLAY SECOND LINE
LPC_GPIO2->FIOSET1=0x08; // RS=1
for(i=0;msg2[i]!='\0';i++)
{
LPC_GPIO0 -> FIOPINH=msg2[i]<<5;
LPC_GPIO2 -> FIOSET1=0x20;//EN=1
delay(500);
LPC_GPIO2 -> FIOCLR1=0x20;//EN=0
delay(50000);
    }
}
else
{
LPC_GPIO2->FIOCLR1=0x08; //RS=0
LPC_GPIO0->FIOPINH=0x01<<5;
	LPC_GPIO2->FIOSET1=0x20;
delay(500);
LPC_GPIO2->FIOCLR1=0x20;
delay(50000);
}
}
}
//switch pin =P0.0