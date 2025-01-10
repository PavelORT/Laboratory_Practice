#include "../Inc/init.h"
#include "../Inc/it_handlers.h"



uint8_t Led1flag = 0;
uint8_t Led2flag = 0;

float res; // переменная для результата
int adc;

uint32_t DelayTickCount = 0;
uint32_t TIM2_counter = 0;
uint32_t SYSTICK_counter = 0;

int main(void) 
{ 
    GPIO_Init_CMSIS();
    RCC_Init(); // Инициализация тактирования системы 
    ITR_Init();// Инициализация прерываний  
    SysTick_Init();
    TIM_Init();
    //ADC_Init();

    while(1) {
        /*
        SET_BIT(ADC1->CR2, ADC_CR2_SWSTART); // запуск преобразования
        // измерение сигнала
        // SET_BIT(ADC1->CR2, ADC_CR2_SWSTART); // запуск преобразования
        // while(!(ADC1->SR & ADC_SR_EOC)); // ожидание завершения преобразования
        res = (float)ADC1->DR * 3.3 / 4096.0 ; // пересчет в напряжение
        //adc = ADC1->DR;
        mydelay(300);
        */
    }
} 