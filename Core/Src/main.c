#include "../Inc/init.h"
#include "../Inc/it_handlers.h"


uint16_t DelayTickCount = 0;
uint32_t GlobalTickBut1Wait = 0, GlobalTickBut2Wait = 0;
//float Ledfreq[3][3]           = {{0.4, 1.3, 2},{0.6, 1.6, 2.2},{0.8, 1.9, 2.5}};
//Numtimes = freqAHB / ((SysTick_LOAD+1)*2*freq) (2 так как включение и выключение)
uint16_t LedSetLoad[3][3] = {{1250, 384, 250},{833, 312, 227},{625, 263, 200}};
uint8_t LedCurrfreq[6][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};
uint16_t LedLoad[6] = {0, 0, 0, 0, 0, 0};
uint16_t LedCount[6] = {0, 0, 0, 0, 0, 0};
uint8_t Ledflag[2][6] = {{1, 1, 1, 1, 1, 1},{0, 0, 0, 0, 0, 0}};

uint8_t Led1flag = 0;

uint8_t Led2flag = 0;
const uint8_t LedOffset[6] = {0U, 7U, 14U, 8U, 9U, 10U};
uint8_t CurrentState = 6;
uint8_t CurrentLed = 0;
uint8_t Numb = 0;
uint8_t counterbut1 = 0;
uint8_t counterbut2 = 0;
uint8_t flagbut1 = 0, flagbut2 = 0;
uint8_t flagbut1long = 0, flagbut2long = 0;

void LedVal_Init();
void Led_light();


float res; // переменная для результата
int adc;

int main(void) 
{ 
    GPIO_Init_CMSIS();
    GPIO_Init_Self();
    RCC_Init(); // Инициализация тактирования системы 
    ITR_Init();// Инициализация прерываний  
    SysTick_Init();
    TIM_Init();
    ADC_Init();

    while(1) {
        SET_BIT(ADC1->CR2, ADC_CR2_SWSTART); // запуск преобразования
        // измерение сигнала
        // SET_BIT(ADC1->CR2, ADC_CR2_SWSTART); // запуск преобразования
        // while(!(ADC1->SR & ADC_SR_EOC)); // ожидание завершения преобразования
        res = (float)ADC1->DR * 3.3 / 4096.0 ; // пересчет в напряжение
        //adc = ADC1->DR;
        mydelay(300);
    }
} 