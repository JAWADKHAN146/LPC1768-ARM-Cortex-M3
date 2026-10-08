#include <LPC17xx.h>
#include <string.h>
void delay(unsigned int x)
{
	int i;
	for(i=0;i<x;i++);
}
void uart_init(void)
{
//pconp is enabled by default for uart0 and pclksel is taken as default value
LPC_UART0->LCR=0X00000083;
LPC_UART0->DLM=0X00000000;
LPC_UART0->DLL=0X00000075;
LPC_UART0->FDR=0X00000010;
LPC_UART0->LCR=0X00000003;
}
int main()
{
 unsigned char arr[3][13]={"090082070589","09008206C944","090084336CD2"};
unsigned char  i=0,ch[13];
SystemInit();
LPC_PINCON->PINSEL0|=0X00000050;
LPC_GPIO1 -> FIOMASK3=0xDF;//11011111
LPC_GPIO1 -> FIODIR3=0x20;//00100000
uart_init();
while(1)
{
	for(i=0;i<12;i++)
	{
		while((LPC_UART0->LSR & 0X01)!=0X01);//WATINT FOR THE DATA TO BR RECIVED 
		ch[i] = LPC_UART0->RBR;
	}ch[12]='\0';
			if(strcmp((char *)ch,(char *)arr[0])==0 ||
				 strcmp((char *)ch,(char *)arr[1])==0 ||
				 strcmp((char *)ch,(char *)arr[2])==0)
			{
			LPC_GPIO1-> FIOSET3=0x20;
			}
			else
			{
				
			LPC_GPIO1-> FIOSET3=0x20;
			delay(500000);
			LPC_GPIO1-> FIOCLR3=0x20;
			
		}
	}		
	}
