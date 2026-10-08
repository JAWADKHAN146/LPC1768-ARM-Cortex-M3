#include <LPC17xx.h>
#include <string.h>
#define LCD_ADDR 0x27
void delay(unsigned long int x)
{
	int i;
	for (i=0;i<x;i++);
}
void i2c_init(void)
{
	LPC_SC->PCONP|=0X80;//7TH BIT FOR I2C0
	LPC_PINCON->PINSEL1|=0X01400000;//PIN 27&28
	LPC_I2C0->I2SCLH=0X5A;
	LPC_I2C0->I2SCLL=0X5A;
	LPC_I2C0->I2CONSET=0X40;        // I2EN = 1 (Enable I2C)  //6th bit is fo enabling i2c interface
}
void i2c_start(void)
{
	LPC_I2C0->I2CONSET=0X20;// Set START bit  i.e., 5th bit
	while(LPC_I2C0->I2STAT!=0X08);
	LPC_I2C0->I2CONCLR=0X120;
}
void i2c_stop(void)
{
    LPC_I2C0->I2CONSET = 0x00000010;        // STOP bit   //4th bit is STOP bit 0001 0000
    LPC_I2C0->I2CONCLR = 0x00000008;        // Clear SI
    delay(0x2000);
}
void i2c_write(unsigned char data)
{
	LPC_I2C0->I2DAT=data;
	LPC_I2C0->I2CONCLR=0X08;
	while((LPC_I2C0->I2CONSET&0X08)==0);
}
void lcd_write(unsigned char ch,unsigned char rs)
{
	unsigned char upper_nib,lower_nib;
	unsigned char data;
	upper_nib=ch&0XF0;
	lower_nib=(ch<<4)&0XF0;
	//send upper nible
	data=upper_nib|rs|0X08;
	i2c_start();
	i2c_write(LCD_ADDR<<1);
	i2c_write(data|0x04);//en=1;
	delay(0x1000);
	i2c_write(data&~0x40);
	delay(0x1000);
	i2c_stop();
	delay(0x30000);
	 // Send lower nibble
   data = lower_nib | (rs ? 0x01 : 0x00) | 0x08;   
   i2c_start();
   i2c_write(LCD_ADDR << 1);          // Slave address + write
   i2c_write(data | 0x04);            // EN = 1
	delay(0x1000);
  i2c_write(data & ~0x04);           // EN = 0
	delay(0x10000);
	i2c_stop();
  delay(0x3000);
}
void uart0_init()
{
	LPC_PINCON->PINSEL0|=0X50;//(5:4 P0.2 TXD0  && 7:6 P0.3  RXD0 )1ST alternative
	LPC_UART0->LCR=0X83;
	LPC_UART0->DLM=0X00;
	LPC_UART0->DLM=0X75;
	LPC_UART0->LCR=0X03;
}
void uart3_init()
{ 
	LPC_SC->PCONP=0X02000000;
	LPC_PINCON->PINSEL0=0X0A;
	LPC_UART3->LCR=0X83;
	LPC_UART3->DLM=0X00;
	LPC_UART3->DLL=0X75;
	LPC_UART3->LCR=0X03;
}
void uart3_write(char atcmd[50])
{
	int i=0;
	while(atcmd[i]!='\0')
	{
		LPC_UART3->THR=atcmd[i];
	  i++;
	}
}
int  main()
	
{
		unsigned char cmd[]={0x33,0x32,0x28,0x0E,0x01,0X06,0x80},var;
	 char arr1[100],lon[25],lat[25];
		int i,count,k,l;
		LPC_GPIO0->FIOMASK0=0XFE;
		LPC_GPIO0->FIODIR0=0X00;
		SystemInit();
		i2c_init();
		uart0_init();
		uart3_init();
		delay(0x100);
		for(i=0;i<7;i++)
		{
			lcd_write(cmd[i],0);
			delay(50000);
		}i=0;
		while(1)
		{
			var=LPC_GPIO0->FIOPIN0;
			//----------wait for '$'----------
			do
			{
				while((LPC_UART0->LSR&0X01)!=0X01);
				arr1[0]=LPC_UART0->RBR;
			}while(arr1[0]!='$');
			i=1;
			while(i<99)
			{
				while((LPC_UART0->LSR&0X01)!=0X01);
				arr1[i]=LPC_UART0->RBR;
				if(arr1[i]=='\n')
					break;
				i++;
			}
			
			arr1[i+1]='\0';
			//-------check wether it is GPRMC ---------
		if(strncmp(arr1,"$GPRMC",6) != 0)
        continue;                 // Ignore all other sentences
			lat[0]='\0';
			lon[0]='\0';
			count=0;
			k=0;
			l=0;
			i=0;
			//-----count of comma(,)--------
			while(arr1[i]!='\0')
			{
				if(arr1[i]==',')
				{
					count++;
					i++;
					continue;
				}
				//------check wether the gps is valide------
				if(count==2)
				{
					if(arr1[i]=='V')
						break;   //invalid gps format
				}
				//---latitude---
				else if(count==3)
				{
					while(arr1[i]!=',')
					{
						lat[k++]=arr1[i];
						i++;
					}lat[k]='\0';
			}
				//------longitude----
			else if(count==5)
			{
				while(arr1[i]!=',')
				{
					lon[l++]=arr1[i];
					i++;
				}
				lon[l]='\0';
			}
			else
			{
				i++;
			}
		}
			 /*---------------- Display on LCD ----------------*/

    lcd_write(0x01,0);
    delay(0x5000);

    lcd_write(0x80,0);

    for(i=0;lat[i]!='\0';i++)
    {
        lcd_write(lat[i],1);
    }

    lcd_write(0xC0,0);

    for(i=0;lon[i]!='\0';i++)
    {
        lcd_write(lon[i],1);
    }

    delay(0x100000);
		if(var==0x01)
		{
		/*-----sending sms ---------*/
		uart3_write("AT\r");
		delay(50000);
		uart3_write("AT+CREG?\r");
		delay(50000);
		uart3_write("AT+CMGF=1\r");
		delay(50000);
		uart3_write("AT+CMGS=\"9008271317\"\r");
		delay(50000);
		while((LPC_UART3->LSR&0X02)!=0X02);
		LPC_UART3->THR = 0x1A;      // Ctrl + Z (sends the SMS)
		uart3_write("Latitude: ");
		uart3_write(lat);

		uart3_write("\r\nLongitude: ");
		uart3_write(lon);

		while(!(LPC_UART3->LSR & 0x20));
		LPC_UART3->THR = 0x1A;
		}
	}
}
