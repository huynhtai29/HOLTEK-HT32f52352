#include "ht32f5xxxx_01.h"              // Device header
#include "math.h"
#include "stdint.h"


volatile uint32_t dem1=0;
volatile uint32_t i=0;
int16_t data[2];
int16_t temp[2];
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
void I2C_Init(void)
{
	// Set fclock = 400kHz with (f = 48MHz )
		HT_I2C1->SHPGR = 108;
		HT_I2C1->SLPGR =108;
		// select mode 7 bit address
		HT_I2C1->CR &=(1<<7);
		
		HT_I2C1->CR |=1<<3;
			
}

void writeReg(uint8_t address,uint8_t data)
{
			while(HT_I2C1->SR & (1<<19));
	// Tao  S bit và address +W
			HT_I2C1->TAR =0;
			HT_I2C1->TAR |= 0x68; 
	// Wait until ADRS = 1
			while(!(HT_I2C1->SR & (1<<2))); // Wait ACK 	
			HT_I2C1->DR = address;
			while(!(HT_I2C1->SR&(1<<17)));	
			HT_I2C1->DR = data;
	
			while(!(HT_I2C1->SR&(1<<17)));
			HT_I2C1->CR|=(1<<1);	// Send Stop-bit			
			while(HT_I2C1->SR & (1<<19));
}

void Clock_Init(void)
	{	
		//Enable clock 
		HT_CKCU->AHBCCR |= (1<<16); // ENABLE clock GPIOA
		HT_CKCU->APBCCR0 	|= (3 << 14); // ENABLE clock EXTI vs AFIO
		HT_CKCU->APBCCR0 	|= (1 << 1); // Enable clock I2C1
		HT_CKCU->APBPCSR1 =0;  // PLL EXTI vs AFIO + I2C = CK_AHB	
	}

int16_t readReg(uint8_t reg)
{
		HT_I2C1->CR &= ~(1<<1);	
	while(HT_I2C1->SR & (1>>19));	
	HT_I2C1->TAR =0;
	HT_I2C1->TAR |= 0x68;
// Waiting ACK signal
	while(!(HT_I2C1->SR&(1<<2))); 
	HT_I2C1->DR =reg;		
	while(!(HT_I2C1->SR&(1<<17)));	

	HT_I2C1->TAR |=(1<<10);// mode Read
	HT_I2C1->TAR |= 0x68;
	while(!(HT_I2C1->SR&(1<<2)));	
	while(HT_I2C1->SR&(1<<21)); // kiem tra mode receiver
	HT_I2C1->CR&=~(1<<0);		
	while(!(HT_I2C1->SR&(1<<16)));// Waiting RXBF
	data[0] = HT_I2C1->DR;
	HT_I2C1->CR|=(1<<1);	//send stop bit	

	return data[0];
}
void Led_Init(void)
{
		HT_CKCU->AHBCCR |= (1 << 18);
		HT_GPIOC->DIRCR |= (3 << 14);
		HT_GPIOC->DOUTR = 0;
}
void Config_GPIO_AFIO()
{
		// Configuration AF PA1 PA0 AF 
		HT_GPIOA->DIRCR |= (3 << 0); // output
		HT_GPIOA->PUR |= (3<<0);
		HT_AFIO->GPACFGR[0] |= (7<<0); // PA0 - I2C
		HT_AFIO->GPACFGR[0] |=(7<<4); // PA1 - I2C
}
void MPU_Init(void)
{
	
}

int main(void)
{
	
	Clock_Init();
	Led_Init();
	Config_GPIO_AFIO();
	I2C_Init();

// configure SysTick
		SysTick->CTRL =0;
		SysTick->LOAD = 4799;
		SysTick->VAL = 0;
		SysTick->CTRL =3;
// configure NVIC
		NVIC_EnableIRQ(SysTick_IRQn);
		NVIC_SetPriority(SysTick_IRQn, 1);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
	writeReg(0x19,0x01);
	temp[0] =readReg(0x41);
	delay(10);
	temp[1] =readReg(0x42);
		while(1)
			{
		// doc thanh ghi nhiet do
		
			/*
			int16_t temp_low= readReg(0x42);
			int16_t temp_high= readReg(0x41);

			*/
			int16_t _temp = temp[0]|(temp[1]<<8);				
			HT_GPIOC->DOUTR |= (1 << 14);
			delay(1000);
			HT_GPIOC->DOUTR &= ~(1 << 14);
			delay(1000);
			}
}

void EXTI4_15_IRQHandler(void)
{
			i++;
			HT_GPIOC->DOUTR &= ~(1 << 14);
			HT_EXTI->EDGEFLGR |= (1 << 11);
}

void SysTick_Handler(void)
{
	dem1++;

		SysTick->CTRL &= ~(1 << 16);
		NVIC_ClearPendingIRQ(SysTick_IRQn);
}
