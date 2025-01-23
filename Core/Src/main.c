#include "../Inc/init.h"
#include "../Inc/it_handlers.h"


uint16_t DelayTickCount = 0;
uint8_t Led1flag = 0;

float res; // переменная для результата
uint16_t adc;

uint32_t TIM3_counter = 0;

uint32_t PWM_Load = 4000;

uint32_t TIM3_CNT = 0;

void timer3(void);

int main(void) 
{ 
    GPIO_Init();
    RCC_Init(); // Инициализация тактирования системы 
    SysTick_Init();
    TIM_Init();
    ADC_Init();
    //timer3();
    //SET_BIT(TIM3->DIER,TIM_DIER_UIE);//включение прерываний
    //NVIC_EnableIRQ(TIM3_IRQn);//разрешаем прерывания в регистре контроллера прерываний NVIC
    //NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); //Установка приоритета прерывания
    //WRITE_REG(TIM3->CCR1,PWM_Load);
    while(1) {
        TIM3->CCR1 = 2*adc;
        // TIM3->ARR = 15000-1;
        //TIM3_CNT = READ_REG(TIM3->CNT);
        //MODIFY_REG(TIM2->CCR1,TIM_CCR1_CCR1, PWM_Load);
        //WRITE_REG(TIM3->CCR1,PWM_Load);
        //res = (float)ADC1->DR * 3.3 / 4096.0 ; // пересчет в напряжение
        
        //mydelay(1);
       
    }
} 


