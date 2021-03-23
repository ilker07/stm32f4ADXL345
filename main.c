

#include "stm32f4xx.h"                  // Device header
#include "adxl345_i2c.h"

uint32_t deger;



int8_t dusukX,dusukY,dusukZ;
int8_t yuksekX,yuksekY,yuksekZ;


uint8_t giden_veri=0;
uint32_t count=0;
int i=0;
int donenAdres=0;





void veri_yolla(int a);
void bekle(uint32_t sure);
void delay(uint32_t sure);


void SysTick_Handler(void)
{

 if(count>0)
	  count--;

}

void delay(uint32_t sure)
{
	count=sure;
	
	while(count);

}


void hazirla()
{

	while(donenAdres==0)
	{
	veri_yolla(3);
  bekle(42000000);
	donenAdres=I2C1_Oku(0x00);
	}
	i2cYaz(0x2D,0x08);//Olcum moduna al.

}
void normal()
{
 
	veri_yolla(150);
  bekle(42000000);


}

void i2cUart()
{
  dusukX=I2C1_Oku(0x32);
	veri_yolla('*');
	veri_yolla('!');
	veri_yolla(dusukX);
	yuksekX=I2C1_Oku(0x33);
	veri_yolla(yuksekX);
  bekle(42000000);
  
}

void bekle(uint32_t sure)
{

while(sure--);
}

void veri_yolla(int a)
{
	 USART3->DR=a;
   while(!(USART3->SR&(1<<6)));  // TX Buffer dolu ise bekle (Rehber Sayfa 646)

	 	
		 
}




void GPIO_Config()
{
	RCC->AHB1ENR  |= 0x00000002;		// GPIOB Clock Enable

	GPIOB->MODER  |= (1 << 21) | (1 << 23);		// AF PB10 AND PB11
	GPIOB->AFR[1] |= (1 << 8) | (1 << 9) |(1<<10);			// PB10  AF7 (USART3)
}

void USART_Config()
{
	RCC->APB1ENR |= 1 << 18;

	
	//USART3->CR1 |= 1 << 2;		// Rx enable
	USART3->CR1 |= 1 << 3;		// Tx enable
	USART3->CR1 |= 0 << 10;		// Parity control disable
	USART3->CR1 |= 0 << 12;		// Word length 8bit
	USART3->CR1 |= 0 << 15;		
	USART3->CR2 |= 0 << 12;		// Stop bit 1
	USART3->CR2 |= 0 << 13;		// Stop bit 1
	USART3->BRR = 0x1117;		// BaudRate 9600
	USART3->CR1 |= 1 << 13;		// Usart enable
}



void RCC_Config()
{
	 
	RCC->CR &=~(1<<0);//hsi off yap
	
	RCC->CR |= 0x00010000;	// HSEON
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	
	RCC->CR |=(1<<19);//css 
  ////cr bit 19  enable etmis burada
	

	RCC->PLLCFGR = 0x00402A04; 
	RCC->CR |= 0x01000000;			// PLL On
	//pll bayrak kontolu.
	while(!(RCC->CR & (1<<25)));	// /pll bayrak kontolu.
	
	
	RCC->CFGR |= 0x00000000;		// AHB Prescaler 1   
	RCC->CFGR |= 0x00009400;		// APB1 Prescaler 4  APB2 Prescaler 2
	
	
	FLASH->ACR = 0x00000605;  //5 ONEMLI
	RCC->CFGR |= 0x00000002; //Sistem PLL icin
	while(!(RCC->CFGR & (1<<1)));	// /pll bayrak kontolu.
	
	//pll bayrak kontrolu??
	
}








int main()
	
{
	
	RCC_Config();
	RCC->AHB1ENR |= 0x00000008; //GPIOA Hatti Aktif.
  GPIOD->MODER = 0x55000000; //d12,d13,d14,d15 cikis.
	GPIOD ->OTYPER =0x00000000;
	GPIOD ->OSPEEDR=0xFF000000;
	GPIOD ->PUPDR = 0x00000000;
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);//1000  ve 100:  1680 000
	deger=SystemCoreClock;
	//GPIO_Config();
	//USART_Config();
	
	//i2cBasla();
	
	//hazirla();
	
	
 	
while(1)
{
	
	GPIOD->ODR = 0x0000F000;
	delay(1000);
	GPIOD->ODR = 0x00000000;
	delay(1000);
	//i2cUart();
	
	//normal();
	
}

}
















