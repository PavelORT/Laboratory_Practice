#include "it_handlers.h" 

extern uint32_t SYSTICK_counter;
extern uint32_t DelayTickCount;

void SysTick_Handler(void) 
{  
    DelayTickCount++;
    SYSTICK_counter++;
} 

/*void ADC_IRQHandler(void)
{
    CLEAR_BIT(ADC1->SR,ADC_SR_EOC);//сброс флага прерывания
}*/

extern uint8_t Led1flag;
extern uint8_t Led2flag;

void TIM1_UP_TIM10_IRQHandler(void)
{
    CLEAR_BIT(TIM1->SR,TIM_SR_UIF);
    if (Led1flag == 0)
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);//включение светодиода 7-го пина GPIOB
        Led1flag = 1;
    }
    else{
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);//выключение светодиода 7-го пина GPIOB
        Led1flag = 0;
    }
    //сброс флага прерывания таймера UIF
}

extern uint32_t TIM2_counter;

void TIM2_IRQHandler(void)
{
    CLEAR_BIT(TIM2->SR,TIM_SR_UIF);
    if (Led2flag == 0)
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);//включение светодиода 14-го пина GPIOB
        Led2flag = 1;
    }
    else{
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);//выключение светодиода 14-го пина GPIOB
        Led2flag = 0;
    }
    TIM2_counter++;
    //сброс флага прерывания таймера UIF
}



void mydelay(uint32_t delay){  
    if(DelayTickCount >= delay) DelayTickCount = 0;
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы 
} 