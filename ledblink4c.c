#include <LPC17xx.h>

void delay(unsigned long int x)
{
    unsigned long int i;
    for(i = 0; i < x; i++);
}

int main()
{
    unsigned char ring;

    SystemInit();

    LPC_GPIO0->FIOMASK1 = 0xF0;
    LPC_GPIO0->FIODIR1  = 0x0F;

    while(1)
    {
        for(ring = 0x01; ring <= 0x08; ring = ring << 1)
        {
            LPC_GPIO0->FIOCLR1 = 0x0F;
            LPC_GPIO0->FIOSET1 = ring;

            delay(0x500000);
        }
    }
}