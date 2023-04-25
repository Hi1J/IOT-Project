#include "key.h"


void KEY_Init(void)
{
	RCC->AHB1ENR |= 1<<4;
	
	GPIOE->MODER &= ~(3<<(2*2));
	
	GPIOE->PUPDR &=~(3<<(2*2));
	
	
	GPIOE->MODER &= ~(3<<(2*3));
	
	GPIOE->PUPDR &=~(3<<(2*3));
	
	
}