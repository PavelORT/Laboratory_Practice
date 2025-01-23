#include "it_handlers.h" 

extern uint16_t adc;
extern uint32_t TIM3_CNT;
void ADC_IRQHandler(void)
{
    CLEAR_BIT(ADC1->SR,ADC_SR_EOC);//сброс флага прерывания
    adc = ADC1->DR;
    //TIM3_CNT = READ_REG(TIM3->CNT);
}

extern uint16_t DelayTickCount;

void SysTick_Handler(void) 
{  
    DelayTickCount++;
} 

extern uint8_t Led1flag;

extern uint32_t TIM3_counter;
extern uint32_t PWM_Load;

void TIM3_IRQHandler(void)
{
    //сброс флага прерывания таймера UIF
    CLEAR_BIT(TIM3->SR,TIM_SR_UIF);
    //CLEAR_BIT(TIM3->SR,TIM_SR_CC1IF);
    TIM3_counter++;
    //MODIFY_REG(TIM3->CCR1,TIM_CCR1_CCR1, PWM_Load);//настройка значения переключения ШИМ
}

//странное из интернета
/*
void TIM3_IRQHandler (void)
{ 
  if(TIM3->SR & TIM_SR_UIF == TIM_SR_UIF){
    TIM3->SR = 0; 
    CLEAR_BIT(TIM3->SR,TIM_SR_UIF);
    CLEAR_BIT(TIM3->SR,TIM_SR_CC1IF);
    GPIOA->BSRR |= BS(10);         // поднимим ножку
  }
 
  if(Read_BIT(TIM3->SR, TIM_SR_CC1IF) == TIM_SR_CC1IF){       
    TIM3->SR =0;
    GPIOA->BSRR |= BR(10);       //опустим ножку         
  }
}
*/

void mydelay(uint32_t delay){  
    if(DelayTickCount >= delay) DelayTickCount = 0;
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы 
} 