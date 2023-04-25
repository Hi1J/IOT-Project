#include "stm32f4xx.h"
#include "led.h"


void delay(uint32_t t)
{
	while(t--);
	
}


int main(void)
{
	
	uint8_t buf[4]={4,5,6,7};
	uint8_t i;
	LED_Init();
	
	while(1){
//		LED_ON;
//		delay(1000000);
//		LED_OFF;
//		delay(1000000);
		for(i=0;i<4;i++){
			GPIOC->ODR &= ~(1<<buf[i]);
			delay(500000);
			GPIOC->ODR |= 1<<buf[i];
			delay(500000);
		
		}
		
		
		
	}

}