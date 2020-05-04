
#include "ht32f5xxxx_01.h"              // Device header

int mode = 0;
int mode1 = 0;
int dem = 60000;
void delay(int a)
{
    int a1;
    while (a > 0)
    {
        a--;
        a1 = 1000;
        while (a1 > 0)
        {
            a1--;
        }
    }
}

int main(void)
{
    // enable clock for peripheral modules
    HT_CKCU->AHBCCR |= (3 << 18);
    // initialization

    HT_GPIOD->DIRCR &= ~(3 << 1);
    HT_GPIOD->INER |= (3 << 1);
    HT_GPIOC->DIRCR |= (3 << 14);
    // gan gia tri cho LED
    HT_GPIOC->DOUTR = 0;
		
    while (1)
    {
        if ((HT_GPIOD->DINR & (1 << 1)) == 0)
        //HT_GPIOC->DOUTR |=(1<<14);
        {
            HT_GPIOC->DOUTR &= ~(1 << 14);
        }
        else
        {
            HT_GPIOC->DOUTR |= (1 << 14);
        }
    }
    return 0;
}
void SysTick_Handler()
{
}
