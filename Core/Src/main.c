#include "../Inc/init.h"
#include "../Inc/it_handlers.h"



uint8_t Led1flag = 0;
uint8_t Led2flag = 0;

float res; // переменная для результата
int adc;

uint32_t TIM3_counter = 0;

uint32_t PWM_Load = 800;

uint32_t TIM3_CNT = 0;


int main(void) 
{ 
    GPIO_Init();
    RCC_Init(); // Инициализация тактирования системы 
    ITR_Init();// Инициализация прерываний  
    TIM_Init();
    ADC_Init();

    while(1) {


        TIM3_CNT = READ_REG(TIM3->CNT);
        //MODIFY_REG(TIM2->CCR1,TIM_CCR1_CCR1, PWM_Load);
        //WRITE_REG(TIM2->CCR1,PWM_Load);
        res = (float)ADC1->DR * 3.3 / 4096.0 ; // пересчет в напряжение
        adc = ADC1->DR;
        mydelay(1);
       
    }
} 