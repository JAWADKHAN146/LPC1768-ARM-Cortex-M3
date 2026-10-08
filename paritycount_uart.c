#include <LPC17xx.h>
#define LCD_ADDR 0x27 //here 0x27 is lcd display address

void delay(unsigned x)
{
	int i;
	for(i=0;i<x
	;i++);
}
void i2c_start(void)
{
    LPC_I2C0->I2CONSET = 0x00000020;        // Set START bit  i.e., 5th bit
	  while(LPC_I2C0->I2STAT != 0x08)  // Wait for START transmitted //after start & interupt(SI)bit set on I2CONSET,status code is 0x08
		{
		}
    LPC_I2C0->I2CONCLR = 0x00000028;        // Clear SI //3rd bit is SI and 5th bit is START bit
}


void i2c_stop(void)
{
    LPC_I2C0->I2CONSET = 0x00000010;        // STOP bit   //4th bit is STOP bit 0001 0000
    LPC_I2C0->I2CONCLR = 0x00000008;        // Clear SI
    delay(0x2000);
}


void i2c_write(unsigned char data)
{
    LPC_I2C0->I2DAT = data;                 // Load data into I2DAT
    LPC_I2C0->I2CONCLR = 0x00000008;        // Clear SI i.e.,3rd bit gor interupt clear bit
    while((LPC_I2C0->I2CONSET & 0x00000008) == 0); // Wait till SI set  i.e., 3rd bit is inteript flag
}


void lcd_write(unsigned char ch, unsigned char rs)
{
    unsigned char high_nib, low_nib;
    unsigned char data;

    high_nib = ch & 0xF0;              // Upper nibble
    low_nib = (ch << 4) & 0xF0;        // Lower nibble

    // Send upper nibble first
    data = high_nib | (rs ? 0x01 : 0x00) | 0x08;   // 11th bit is RS in chosen ARM 
    i2c_start();
    i2c_write(LCD_ADDR << 1);          // Slave address + write //when left shifted lsb is 0 which is write operation
    i2c_write(data | 0x04);            // EN = 1   //13th bit is EN in chosen ARm
	  delay(0x1000);
    i2c_write(data & ~0x04);           // EN = 0
		delay(0x10000);
    i2c_stop();

    // Send lower nibble
    data = low_nib | (rs ? 0x01 : 0x00) | 0x08;   
    i2c_start();
    i2c_write(LCD_ADDR << 1);          // Slave address + write
    i2c_write(data | 0x04);            // EN = 1
		delay(0x1000);
    i2c_write(data & ~0x04);           // EN = 0
		delay(0x10000);
    i2c_stop();

    delay(0x3000);
}
void i2c_init(void)
{
    LPC_SC->PCONP |= 0x00000080;      //power control for I2C0 is 7th pin //1000 0000       

    LPC_PINCON->PINSEL1 = 0x01400000;  //01 for mux to be select SDA &SCL in P0.27 and 28
	
			//data rate and duty cycle for standard freq 100KHz and 18Mhz for cpu
	
    LPC_I2C0->I2SCLH = 0x0000005A;          // SCL High time (90) for 100khz standard mode
    LPC_I2C0->I2SCLL = 0x0000005A;          // SCL Low time  (90) for 100khz standard mode
    LPC_I2C0->I2CONSET = 0x00000040;        // I2EN = 1 (Enable I2C)  //6th bit is fo enabling i2c interface
}
void uart_init(void)
{
	LPC_UART0->LCR=0X9B;//10011011
	LPC_UART0->DLM=0X00;
	LPC_UART0->DLL=0X75;
	LPC_UART0->LCR=0X0B;
}
int main()
{
unsigned char cmd[]={0x33,0x32,0x28,0x0E,0x01,0X06,0x80},str1[]="Even",str2[]="odd";
unsigned char i,count=0,val;
SystemInit();
LPC_PINCON->PINSEL0|=0X00000050;
uart_init();
i2c_init();
delay(0x100);
for(i=0;i<7;i++)
    {
        lcd_write(cmd[i],0); 
        delay(0x5000);
    }
while(1)
{
while((LPC_UART0->LSR & 0X01)!=0X01);
val=LPC_UART0->RBR;
count=0;
lcd_write(0x01,0);   // Clear display
delay(0x5000);
lcd_write(0x80,0);   // Cursor to first line
for(i=0;i<8;i++)
{
	if(val&0x01){
		count++;
	}val=val>>1;
	}
if(count%2==0)
{
for(i=0;str1[i]!='\0';i++)
	{
 lcd_write(str1[i],1);
	}		
        delay(0x8000);
}
else
{
	for(i=0;str2[i]!='\0';i++)
	{
 lcd_write(str2[i],1);
	}		
        delay(0x8000);
}
}
}