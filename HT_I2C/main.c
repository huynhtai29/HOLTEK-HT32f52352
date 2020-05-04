#include "ht32f5xxxx_01.h"              // Device header



volatile uint32_t dem1=0;
volatile uint32_t i=0;
uint8_t data[2];
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
void BS112_Init(void)
{
	// Write I2C Configutation BS812 Enable Interrup Register 0xC0
			// send Start Bit + addres slave + mode write
			while(((HT_I2C1->SR>>19)&1)!=0)
			{
			}
			HT_I2C1->TAR =0;
			HT_I2C1->TAR |= 0x50;
			while(((HT_I2C1->SR>>2)&1)==0);
			HT_I2C1->DR =0xB2;
			while(((HT_I2C1->SR>>17)&1)==0); 
			HT_I2C1->DR =0x04;		
			while(((HT_I2C1->SR>>17)&1)==0);
			for(int i =0;i<3;i++)
			{
				HT_I2C1->DR =0x02;		
				while(((HT_I2C1->SR>>17)&1)==0);				
			}
			HT_I2C1->DR =0xD8;		
			while(((HT_I2C1->SR>>17)&1)==0);
			for(int i =0;i<11;i++)
			{
			HT_I2C1->DR =0x14 ;		
			while(((HT_I2C1->SR>>17)&1)==0);
			}
			HT_I2C1->CR =0x54;//check sum
			while(((HT_I2C1->SR>>17)&1)==0);

			HT_I2C1->DR =0x12;//check sum
			HT_I2C1->CR|=(1<<1);
			//while(((HT_I2C1->SR>>9)&1)==0);
			//HT_I2C1->CR&=~(1<<0);	// Send NACK
			//HT_I2C1->CR=(1<<1);	// Send Stop-bit	
			
	
}
void read_data()
{
	HT_I2C1->CR &=~(1<<1);
	HT_I2C1->CR |= 1<<0;
	//while(((HT_I2C1->SR)>>19)!=0);
	HT_I2C1->TAR =0;
	HT_I2C1->TAR = 0x50;
	while(((HT_I2C1->SR>>2)&1)==0); // Waiting ACK signal
	HT_I2C1->DR =0x08;
	while(((HT_I2C1->SR>>17)&1)==0) // Waiting TXD
	{ 
		HT_GPIOC->DOUTR &= ~(1 << 14);
	}
	
	HT_I2C1->TAR |=(1<<10);
	HT_I2C1->TAR |= 0x50;
	while((HT_I2C1->SR>>2)&1);//Waiting ACK


	while(((HT_I2C1->SR>>16)&1)==0) // Waiting RXBF 
		{
		}
		data[0] = HT_I2C1->DR;
		HT_I2C1->CR&=~(1<<0);		
	while(((HT_I2C1->SR>>16)&1)==0) // Waiting RXBF
	
		HT_I2C1->CR|=(1<<1);		
		data[1] = HT_I2C1->DR; 
 // Send NACK
   // Send STOP-bit

}
int main(void)
{
	
// Test - Led
		HT_CKCU->AHBCCR |= (1 << 18);
		HT_GPIOC->DIRCR |= (3 << 14);
		HT_GPIOC->DOUTR = 0;	
// Configuration EXTI_
		HT_CKCU->AHBCCR |= (1<<17); // GPIOB	
	 	HT_CKCU->APBCCR0 	|= (3 << 14); // ENABLE clock EXTI vs AFIO
    HT_GPIOB->DIRCR &= ~(1 << 11); // input
		HT_GPIOB->INER |= (1 << 11);
		HT_AFIO->ESSR[1] |= (1<<12); // PB11 - EXTI	
		HT_EXTI->CFGR11 |=(3<<28); // Negative-edge 			
		HT_CKCU->APBPCSR1 =0;  // PLL EXTI vs AFIO + I2C = CK_AHB	
		HT_EXTI->CR |=(1<<11); // enable interrupt
		NVIC_EnableIRQ(EXTI4_15_IRQn); // enable NVIC	
	
// configure SysTick
		SysTick->CTRL =0;
		SysTick->LOAD = 4799;
		SysTick->VAL = 0;
		SysTick->CTRL =3;
// configure NVIC
		NVIC_EnableIRQ(SysTick_IRQn);
		NVIC_SetPriority(SysTick_IRQn, 1);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
	// Configuration GPIO PA1 PA2 , PB11 AF I2C
	// GPIO
	//Enable clock for GPIO
		HT_CKCU->AHBCCR |= (1<<16); // GPIOA
		
		// Configuration AF PA1 PA0 AF 
		HT_GPIOA->DIRCR |= (3 << 0); // output
		HT_GPIOA->PUR |= (3<<0);
//		HT_GPIOA->DOUTR |=(3<<1);

	// Configuration AFIO	

		HT_AFIO->GPACFGR[0] |= (7<<0); // PA0 - I2C
		HT_AFIO->GPACFGR[0] |=(7<<4); // PA1 - I2C
	// Configuration EXTI


		
	// Configuration 	I2C
		HT_CKCU->APBCCR0 	|= (1 << 1); // Enable clock I2C1
		// Set mode Master
//		HT_I2C1->CR |= 1<<0; // ACK signal
//	HT_I2C1->CR |=(1<<1); // STOP bit
		
		// Set 100MHz
		HT_I2C1->SHPGR = 468;
		HT_I2C1->SLPGR =468;
// Address slave 
//		HT_I2C1->TAR |=0x50; // adress slave 
//		HT_I2C1->TAR |= (1<<10);// select read
		// Enable Interrupt
	/*	
		HT_I2C1->IER |= (1<<16); //Not Empty IEB in Received Mode
		HT_I2C1->IER |= (1<<10); //Received NACK IEB
		HT_I2C1->IER |= (1<<1);  //Detected STOP bit
		HT_I2C1->IER |= 1;       // Detected STAR bit
	*/
		// Enable I2C
				HT_I2C1->IER |= (1<<9);
		HT_I2C1->CR |=(1<<3);
		NVIC_EnableIRQ(I2C1_IRQn); // enable NVIC

/*
// Write I2C Configutation BS812 Enable Interrup Register 0xC0
			// send Start Bit + addres slave + mode write
			while(((HT_I2C1->SR>>19)&1)!=0)
			{
			}
			HT_I2C1->TAR =0;
			HT_I2C1->TAR |= 0x50;
			while(((HT_I2C1->SR>>2)&1)==0);
			HT_I2C1->DR =0xB0;
			while(((HT_I2C1->SR>>17)&1)==0); // Waiting ACK signal
			HT_I2C1->DR =0x01;		
			while(((HT_I2C1->SR>>17)&1)==0);
			for(int i =0;i<2;i++)
			{
				HT_I2C1->DR =0x01;		
				while(((HT_I2C1->SR>>17)&1)==0);				
			}
				HT_I2C1->DR =0x01;		
				while(((HT_I2C1->SR>>17)&1)==0);
			HT_I2C1->DR =0xD9;		
			while(((HT_I2C1->SR>>17)&1)==0);
			for(int i =0;i<11;i++)
			{
			HT_I2C1->DR =0x15 ;		
			while(((HT_I2C1->SR>>17)&1)==0);
			}
			HT_I2C1->DR =0x55;//check sum
			while(((HT_I2C1->SR>>17)&1)==0);
			HT_I2C1->DR =0xC3;
			while(((HT_I2C1->SR>>17)&1)==0);
			HT_I2C1->CR=(1<<1);		
		
			//HT_I2C1->CR=(1<<1);	// Send Stop-bit			

*/

//------------------Read register--------------//	
	//read_data();
		while(1)
			{
				
				read_data();
				HT_GPIOC->DOUTR |= (1 << 14);
			delay(1000);
			HT_GPIOC->DOUTR &= ~(1 << 14);
				delay(1000);

	//			HT_EXTI->SSCR |=(1<<11);
			}
}

void EXTI4_15_IRQHandler(void)
{
			i++;
			HT_GPIOC->DOUTR &= ~(1 << 14);
			HT_EXTI->EDGEFLGR |= (1 << 11);
}
void I2C1_IRQHandler(void)
{
	if(((HT_I2C1->SR>>9)&1)==0)
	{

	}
}
void SysTick_Handler(void)
{
	dem1++;

		SysTick->CTRL &= ~(1 << 16);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
}
