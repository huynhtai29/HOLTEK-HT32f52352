#include "ht32f5xxxx_01.h"              // Device header



int mode = 0;
int mode1 = 0;
int dem = 60000;
volatile uint32_t dem1=0;
volatile uint32_t i=0;


void delay(uint32_t value)
{
	dem1 =0;
	uint32_t value_current;
	value_current = dem1;
	while((dem1-value_current)<value)
		{
//			asm("nop");
		}
			
}

int main(void)
{
    // enable clock for peripheral modules
    HT_CKCU->AHBCCR |= (3 << 18);
//	HT_CKCU->AHBCCR |= (1 << 1);
    // initialization
// configure SysTick
		SysTick->CTRL =0;
		SysTick->LOAD = 4799;
		SysTick->VAL = 0;
		SysTick->CTRL =3;
// configure NVIC
		NVIC_EnableIRQ(SysTick_IRQn);
		NVIC_SetPriority(SysTick_IRQn, 1);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
// configure GPIO	
    HT_GPIOD->DIRCR &= ~(3 << 1);
    HT_GPIOD->INER |= (3 << 1);	
    HT_GPIOC->DIRCR |= (3 << 14);
    // gan gia tri cho LED
    HT_GPIOC->DOUTR = 0;
// configure EXTI vs NVIC
	// configure clock EXTI;
	//	HT_CKCU->AHBCCR |= (1 << 6);  register enable clok APB in sleep mode
 		HT_CKCU->APBCCR0 	|= (3 << 14);
		HT_CKCU->APBPCSR1 |=(2<<2);
	// configure EXIT
//BUTTON1 	PD1
		//configure GPIOD
		HT_AFIO->ESSR[0] |= (3<<4); // Select mode AF -EXTI
		HT_EXTI->CFGR1 |=(2<<28);
		HT_EXTI->CFGR1 |=(1<<31); // enable De-bounce
		HT_EXTI->CFGR1 |=32000; // time De-bounce 
		HT_EXTI->CR |=(1<<1); // enable interrupt
		NVIC_EnableIRQ(EXTI0_1_IRQn); // enable NVIC
		NVIC_SetPriority(EXTI0_1_IRQn, 2); // priority
		NVIC_ClearPendingIRQ(EXTI0_1_IRQn); // clear flag Pending
//BUTTON1 	PD2
		//configure GPIOD
		HT_AFIO->ESSR[0] |= (3<<8); // Select mode AF -EXTI
		HT_EXTI->CFGR2 |=(2<<28);
		HT_EXTI->CFGR2 |=(1<<31); // enable De-bounce
		HT_EXTI->CFGR2 |=32000; // time De-bounce 
		HT_EXTI->CR |=(1<<2); // enable interrupt
		NVIC_EnableIRQ(EXTI2_3_IRQn); // enable NVIC
		NVIC_SetPriority(EXTI2_3_IRQn, 3); // priority
		NVIC_ClearPendingIRQ(EXTI2_3_IRQn); // clear flag Pending		
//  SPI
// 	CONFIGURE GPIO PA4 = SCK  PA5 = MOSI PA7 = SEL/CS
		HT_CKCU->AHBCCR |= (1 << 16);
		HT_GPIOA->DIRCR &= ~(1 << 4);
		HT_GPIOA->DIRCR &= ~(1 << 5);
		HT_GPIOA->DIRCR &= ~(1 << 7);
    
// AFIO-SPI AF5

		HT_AFIO->GPACFGR[0] |= (5<<16); 
		HT_AFIO->GPACFGR[0] |= (5<<20);
		HT_AFIO->GPACFGR[0] |= (5<<28);
// CONFIGUR SPI
		// ENABLE CLOCK SPI
		HT_CKCU->APBCCR0 	|= (1 << 4);
		HT_CKCU->APBPCSR1 |=(0<<4); //Fclk - spi0 =48Mhz
		// SPI
		HT_SPI0->CR1 |= (1<<14);
		HT_SPI0->CR1 |= (1<<8);
		HT_SPI0->CR1 |= (0<<0); // 0x00 = 16 bit
		HT_SPI0->CR1 |= (1<<13);
		HT_SPI0->CR0 |= (1<<3);
		HT_SPI0->CR0 |= (1<<4);
		HT_SPI0->CPR = 23;
		HT_SPI0->CR0 |= 1;			
		HT_SPI0->DR = 0xC01;
		delay(5);
		HT_SPI0->DR = 0xA00;
		delay(5);
		HT_SPI0->DR = 0x9FF;
		delay(5);
		HT_SPI0->DR = 0xB07;
		delay(5);	
		HT_SPI0->DR = 0x60A;			
		delay(5);	
		HT_SPI0->DR = 0x30A;
		delay(5);		
		HT_SPI0->DR = 0x505;
		delay(5);		
		HT_SPI0->DR = 0x400;
		delay(5);		
		HT_SPI0->DR = 0x708;
		delay(5);		
		HT_SPI0->DR = 0x801;
		delay(5);		
	while (1)
    {

        //if (i == 1)
       //HT_GPIOC->DOUTR |=(1<<14);
	for(int i=0;i<=5;i++)
	{
		HT_SPI0->DR = (0x200 + i);
		for(int i=0;i<=9;i++)
		{
				HT_SPI0->DR = (0x100 + i);
				delay(1000);
		}
	}

		/*
          HT_GPIOC->DOUTR &= ~(1 << 14);
					delay(1000);
         HT_GPIOC->DOUTR |= (1 << 14);
					delay(1000);
        //}
        //else
        //{
		
        //}
*/
    }
    return 0;
}
void SysTick_Handler(void)
{
	dem1++;
//	i++;
/**
if(i==1)
	{
			HT_GPIOC->DOUTR =0;
		}
	else
		{
			HT_GPIOC->DOUTR =0x00004000;
			i=0;
		}
**/
		SysTick->CTRL &= ~(1 << 16);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
}

void EXTI0_1_IRQHandler(void)
{
	
	if((HT_EXTI->EDGEFLGR )!=0)
		{
/*
	i++;
	if(i==1)
	{
			HT_GPIOC->DOUTR &= ~(1 << 14);
	}
	else
		{
*/
			HT_GPIOC->DOUTR |= (1 << 14);
//			i=0;
//		}

			HT_EXTI->EDGEFLGR |= (1 << 1);
			NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
		}
	
}
void EXTI2_3_IRQHandler(void)
{
	if((HT_EXTI->EDGEFLGR)!=0)
		{
//	i++;

//	if(i==1)
//	{
			HT_GPIOC->DOUTR &= ~(1 << 14);
//	}
/*	else
		{
			HT_GPIOC->DOUTR |= (1 << 14);
			i=0;
		}
*/

			HT_EXTI->EDGEFLGR |= (1 << 2);

			NVIC_ClearPendingIRQ(EXTI2_3_IRQn);
		}
	
}