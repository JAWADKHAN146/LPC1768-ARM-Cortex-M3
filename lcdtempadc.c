#include <LPC17xx.h>
void delay(unsigned long int x)
{
int i;
for(i=0;i<x;i++);
}
void lcd_4(unsigned char chr)
{
    unsigned char chr1, chr2;
 
    // Upper nibble
    chr1 = chr & 0xF0;
    LPC_GPIO0->FIOPIN3 = chr1 >> 4;

    LPC_GPIO2->FIOSET1 = 0x20;   // EN=1
    delay(500);
    LPC_GPIO2->FIOCLR1 = 0x20;   // EN=0
    delay(50000);

    // Lower nibble
    chr2 = chr & 0x0F;
    LPC_GPIO0->FIOPIN3 = chr2;

    LPC_GPIO2->FIOSET1 = 0x20;   // EN=1
    delay(500);
    LPC_GPIO2->FIOCLR1 = 0x20;   // EN=0
    delay(50000);
}
int main()
{
int i;
unsigned int temp[3];
unsigned int v,tp,adc;
unsigned char str1[]="O/P TEMP";
unsigned char cmd[]  = {0x02,0x28,0x0E,0x06,0x01,0x80};
SystemInit();
delay(50000);
//--------LED-----------P1.29---------------
LPC_GPIO1->FIOMASK3=0XDF; 
LPC_GPIO1->FIODIR3=0X20; 
//--------LCD-----------P0.24-27(D4-D7)-----
LPC_GPIO0->FIOMASK3=0xF0;
LPC_GPIO0->FIODIR3=0x0F;
//-------RS,R/W,EN-------P2.11,12,13--------
LPC_GPIO2->FIOMASK1 = 0xC7;
LPC_GPIO2->FIODIR1  = 0x38;
//--------ADC-----------
LPC_SC->PCONP|=1<<12;//Power control for ADC is made as high
LPC_PINCON ->PINSEL1|=0x00004000;
LPC_ADC->ADCR=0x00210301;
// LCD Initialization
LPC_GPIO2->FIOCLR1 = 0x08;   // RS=0
LPC_GPIO2->FIOCLR1 = 0x10;   // RW=0
for(i=0;i<6;i++){
lcd_4(cmd[i]);}
// First line
LPC_GPIO2 ->FIOSET1=0x08;//RS=1
for(i=0;str1[i]!='\0';i++) 	 	
	{
		lcd_4(str1[i]);
	}

while(1)
{
while((LPC_ADC->ADSTAT & 0X00000001)!=0x00000001)
{
}
adc=((LPC_ADC-> ADDR0>>4)&0x00000FFF);
v = (adc * 3300) / 4095;
tp = v / 10;

for(i=0; i<3; i++)
{
    temp[i] = tp % 10;
    tp = tp / 10;
}

//TURN ON LED
	LPC_GPIO1->FIOSET3=0X20; 
   // Second line
            LPC_GPIO2->FIOCLR1 = 0x08;
            lcd_4(0xC0);

            LPC_GPIO2->FIOSET1 = 0x08;
              for(i=2;i>=0;i--)
			  {
              lcd_4(temp[i]+0x30);
			  }
		}
	}



