
#include "stm32f4xx.h"  
#include "adxl345_i2c.h"

#define ADXL345_WRITE 0xA6
#define ADXL345_READ  0xA7


//CS ==>VDD
//I2C1 ==>APB1 
//PB6 ==>SCL
//PB7 ==>SDA
//AF4

//OTYPER OPEN-DRAIN SCL ve SDA
//MODER 10



void  i2cBasla()
	
{

RCC->AHB1ENR |= 0x00000002; //GPIOB aktif..
RCC->APB1ENR |= 0x00200000; //I2C1  aktif.
GPIOB->AFR[0]= 0x44000000; //PB7 AF4 ayari ve PB6 AF4 ayari.
GPIOB->MODER |= 0x0000A000; //PB7 ve PB6 Alter.Function
GPIOB->OTYPER =0x000000C0; //PB7 ve PB6 Open-Drain.
	
I2C1->CR2 = 0x0008 ; //8mhz APB1 hat.
I2C1->CCR = 0x0028;  //i2c 100khz.
I2C1->TRISE=0x09;// Maximum yukselme zamani(Datasheette yaziyor) 
I2C1->CR1=0x0001;// I2C aktif.
}

void i2cYaz(uint8_t adres,uint8_t veri)
{
 I2C1->CR1 |= 0x0100; // START biti
 while (!(I2C1->SR1 & 0x0001)); // SB=1 olmasini bekle.
 I2C1->DR = ADXL345_WRITE; // Slave adresi.
 while (!(I2C1->SR1 & 0x0002)); // ADDR=1 olmasini bekle.
 while (!(I2C1->SR2 & 0x0001));  //Master
 int Status2 = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)); // TXE=1 olmasini bekle.
 I2C1->DR = veri; //Veriyi gonder.
 while (!(I2C1->SR1 & 0x0080)); // TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)); // BTF=1 olmasini bekle.
 I2C1->CR1 |= 0x0200; // STOP biti.
 
	
}

int I2C1_Oku(int adres) { 

	
 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = ADXL345_WRITE; // Slave Adresi.(Yazma)
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 int temp = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)) {}; //TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)) {}; // BTF=1 olmasini bekle.

 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = ADXL345_READ; // Slave Adresi.(Okuma)
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 temp= I2C1->SR2; // Bayrak temizle.
 	 
 while (!(I2C1->SR1 & 0x0040)) {}; // RxNE=1 olmasini bekle. 
 I2C1->CR1 |= 0x0200; // STOP biti.

 return I2C1->DR; // gelen veriyi al. 
	 
}








