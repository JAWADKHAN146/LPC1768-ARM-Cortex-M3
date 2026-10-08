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
unsigned char str1[]="ARM CORTEX M3", i=0, val[20],ch;
SystemInit();
LPC_PINCON->PINSEL0|=0X00000050;
LPC_GPIO1 -> FIOMASK3=0xDF;//11011111
LPC_GPIO1 -> FIODIR3=0x20;//00100000
uart_init();
while(1)
{
while((LPC_UART0->LSR & 0X01)!=0X01){}
ch = LPC_UART0->RBR;

        if(ch == '\r')          // Enter key
        {
            val[i] = '\0';

            if(strcmp(val, str1) == 0)
						{
                LPC_GPIO1->FIOSET3 = 0x20;
						}            
						else
               LPC_GPIO1->FIOCLR3 = 0x20;
            i = 0;              // Ready for next string
        }
        else
        {
            if(i < 19)
                val[i++] = ch;
        }
    }
}