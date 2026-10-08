#include<LPC17xx.h>
#include "led_headerfile.h"
void uart0_init(void);
void CAN_Init(void);
void CAN_ACC(void);
void looktable(void); 
void delay(unsigned long int z);
void can_tx(unsigned long int id,unsigned char msg);
void can_rx(void);
unsigned char rx=0;
int main()
{
SystemInit ();	
uart0_init();
CAN_Init();
CAN_ACC();
looktable(); 	
	while(1);
	
	
}
void CAN_Init(void)

{
LPC_SC->PCONP|=0x00002000;
LPC_SC->PCLKSEL0|=0X00000000;
LPC_PINCON->PINSEL0|=0X00000005;
LPC_CAN1->MOD=0x00000001;
LPC_CAN1->CMR=0X00000000;
LPC_CAN1->GSR=0x00000000;
LPC_CAN1->IER=0x00000001;
LPC_CAN1->BTR=0X001C0007;
LPC_CAN1->MOD=0x00000000;	
	NVIC_EnableIRQ(CAN_IRQn);
}

void CAN_ACC(void)
{
LPC_CANAF->AFMR=0x00000001;
LPC_CANAF->SFF_sa=0x00000000;
LPC_CANAF->SFF_GRP_sa=0x00000000;
LPC_CANAF->EFF_sa=0x00000000;
LPC_CANAF->EFF_GRP_sa=0x00000008;
LPC_CANAF->ENDofTable=0x00000008;
LPC_CANAF->AFMR=0x00000000;	
}

void looktable(void) 
{
	LPC_CANAF->AFMR=0x00000001;
LPC_CANAF_RAM->mask[0]=0x00000020;
LPC_CANAF_RAM->mask[1]=0x00000021;
	LPC_CANAF->AFMR=0x00000000;
}

void delay(unsigned long int z)
{unsigned long int x;
for(x=0;x<z;x++);
}

void uart0_init(void)
{ LPC_SC->PCONP|=0X00000000;
  LPC_PINCON->PINSEL0|=0X00000050;
  LPC_SC->PCLKSEL0|=0X00000000;
  LPC_UART0->LCR=0X83;
  LPC_UART0->DLM=0X00;
  LPC_UART0->DLL=0X75;
  LPC_UART0->FDR=0X00000010;
  LPC_UART0->LCR=0X03;
	//LPC_UART0->FCR=0X06;
}
  
  void can_tx(unsigned long int id,unsigned char msg)
	{
	while((LPC_CAN1->SR&0X00000004)!=0X00000004);

		LPC_CAN1->TFI1=0X80010000;
		LPC_CAN1->TID1=id;
		LPC_CAN1->TDA1=msg;
		LPC_CAN1->CMR=0X21;
		
	}
 
void 	CAN_IRQHandler(void)
{
can_rx();



}	
void can_rx(void)
{
unsigned long int CA = 0,CB = 0, CC = 0, CD = 0;
	CA = LPC_CAN1->RFS;
	CA = CA & 0x20000000;
	if (CA == 0x00000000)
	{
		CB = LPC_CAN1->RFS;
		CB = CB & 0x000003FF;
		if(CB == 0x00000001)
		{
			CC = LPC_CAN1->RFS;
			CC = CC & 0x000F0000;
			if(CC == 0x00010000)
			{
				CD = LPC_CAN1->RDA;
				CD = CD & 0x000000FF;
					LPC_UART0->THR = CD;
				if(CD == '1')
				{
					led_blink();
				}
			//  }

			}
		}
		LPC_CAN1->CMR = 0X04;		
	}
}