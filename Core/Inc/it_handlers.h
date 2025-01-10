#include "init.h" 
 

void EXTI9_5_IRQHandler(void); 
void EXTI15_10_IRQHandler(void); 
void SysTick_Handler(void); 
void ADC_IRQHandler(void);
void TIM1_UP_TIM10_IRQHandler(void);
void TIM2_IRQHandler(void);
void mydelay(uint32_t delay); 