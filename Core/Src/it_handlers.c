#include "it_handlers.h" 

void ADC_IRQHandler(void)
{
    CLEAR_BIT(ADC1->SR,ADC_SR_EOC);//сброс флага прерывания
}

extern uint8_t Led1flag;
extern uint8_t Led2flag;


extern uint32_t TIM3_counter;
extern uint32_t PWM_Load;

void TIM3_IRQHandler(void)
{
    //сброс флага прерывания таймера UIF
    CLEAR_BIT(TIM3->SR,TIM_SR_UIF);
    //MODIFY_REG(TIM3->CCR1,TIM_CCR1_CCR1, PWM_Load);//настройка значения переключения ШИМ
    
    
}

void mydelay(uint32_t delay){  
    if(DelayTickCount >= delay) DelayTickCount = 0;
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы 
} 