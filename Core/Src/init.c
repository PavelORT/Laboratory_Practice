#include "../Inc/init.h"
#include "../Inc/it_handlers.h"

void GPIO_Init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN); //Включение тактирования портов GPIOB и GPIOC
    //set LED (PB14)
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE14_0); 
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR14_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR14_0);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);

    //set (PA6) //выход ШИМ TIM3_CH1
    SET_BIT(GPIOA->MODER, GPIO_MODER_MODE6_0 | GPIO_MODER_MODE6_1);//альтернативная функция
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT6);//push-pull
    SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6_0 | GPIO_OSPEEDER_OSPEEDR6_1);//ставим скорость на максимум
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPDR6_0);//Отключение PU/PD
    MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL6, GPIO_AFRL_AFSEL6_1); //выбор альтернативной функции
  
    //Настройка пина PB1 на аналоговый вход для АЦП (ADC12_IN9)
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE1_Msk);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT1);//Настройка на Push-Pull 
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR1_0);//Настройка скорости работы 
    //SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR1_0 | GPIO_OSPEEDER_OSPEEDR11_1);//Настройка скорости работы 
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR1_0);//Отключение PU/PD резисторов
    
}

void RCC_Init(void){
    //Очистка всех битов, участвующих в настройке
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
    CLEAR_REG(RCC->CFGR);
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON); 
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON); 
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    /* Настройка главного регистра RCC */ 
    SET_BIT(RCC->CR, RCC_CR_HSEON); //Запускаем внешний кварцевый резонатор 
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET); //Ждём пока он запустится 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); //Сбросим бит обхода внешнего тактирования в 0, если вдруг там что-то лежит 
    SET_BIT(RCC->CR, RCC_CR_CSSON); //Запустим Clock detector (Clock security system)
    
    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); //Выставляем предделитель входной частоты PLL на 4
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7); //Настраиваем умножение частоты, полученной после деления (частоты VCO) на х180 
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk); //Настраиваем предделитель получившейся частоты после умножения. Иными словами, получаем итоговую частоту PLL 
    SET_BIT(RCC->CR, RCC_CR_PLLON); //Запустим PLL 
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY)); //Ждём запуска PLL
    
    //Настройка регистра RCC_CFGR
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); //Выбираем PLL в качестве System Clock 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); //Предделитель AHB без делителя
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4); //Предделитель APВ1, делим на 4 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2); //Предделитель APВ2, делим на 2 

    //количество циклов задержки памяти на 6 циклов CPU
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS); 
}

void SysTick_Init(void){ 
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); //предварительно выключим счётчик 
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); //Разрешаем прерывание по системному таймеру 
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); //Источник тактирования будет идти из AHB без деления 
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSTLOAD << SysTick_LOAD_RELOAD_Pos); //Будет тикать с частотой 1 кГц и вызывать прерывания 
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, SYSTLOAD << SysTick_VAL_CURRENT_Pos); //Начнём считать со значения 17999
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); //Включим счётчик 
} 

void TIM_Init()
{
    //Включение таймеров
    
    //TIM3
    SET_BIT(RCC->APB1ENR,RCC_APB1ENR_TIM3EN); //включение тактирования TIM3 
    MODIFY_REG(TIM3->PSC,TIM_PSC_PSC,899);//настройка предделителя тактирование от APB1 2*45МГц
    MODIFY_REG(TIM3->ARR,TIM_ARR_ARR,1000-1);//настрока значения перезагрузки
    
    SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE);//включение предварительной загрузки на CH1
    MODIFY_REG(TIM3->CCR1,TIM_CCR1_CCR1, 800);//настройка значения переключения ШИМ от 0 до 1000
    SET_BIT(TIM3->CCER,TIM_CCER_CC1E);//включение выхода
    SET_BIT(TIM3->CCMR1,TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);//Настройка на режим ШИМа
    SET_BIT(TIM3->DIER,TIM_DIER_UIE);//включение прерываний
    NVIC_EnableIRQ(TIM3_IRQn);//разрешаем прерывания в регистре контроллера прерываний NVIC
    NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); //Установка приоритета прерывания

    //SET_BIT(TIM2->EGR,TIM_EGR_UG);//Перезагружаем счётчик TIM2
    SET_BIT(TIM3->CR1,TIM_CR1_CEN);//включение TIM2
    
    /*
    CLEAR_BIT(TIM2->SMCR,TIM_SMCR_SMS);//выключаем slave mode, чтобы тактировался напрямую от APB
    CLEAR_REG(TIM2->CR1); //Сброс битов
    CLEAR_REG(TIM2->CR2); //Сброс битов
    CLEAR_BIT(TIM2->CR1,TIM_CR1_DIR);//up направление счёта 0-up 1-dowm
    CLEAR_BIT(TIM2->CR1,TIM_CR1_CMS);//center-aligned mode выключен
    SET_BIT(TIM2->DIER,TIM_DIER_UIE);//включение прерываний
    MODIFY_REG(TIM2->PSC,TIM_PSC_PSC,4499);//настройка предделителя тактирование от APB1 45МГц
    MODIFY_REG(TIM2->ARR,TIM_ARR_ARR,9999);//настрока значения перезагрузки
    //MODIFY_REG(TIM2->PSC,TIM_PSC_PSC,0);//настройка предделителя тактирование от APB1 45МГц
    //MODIFY_REG(TIM2->ARR,TIM_ARR_ARR,65535);//настрока значения перезагрузки
    
    
    CLEAR_BIT(TIM2->CR1, TIM_CR1_ARPE);//выключаем автоперезагрузку
    CLEAR_BIT(TIM2->SMCR, TIM_SMCR_ECE);//тактирование таймера от внутреннего источника
    SET_BIT(TIM2->CCMR1, TIM_CCMR1_OC1PE);//включение предварительной загрузки на CH1
    CLEAR_BIT(TIM2->CCER, TIM_CCER_CC1E);//Отключаем канал CH1 для дальнейшей настройки 
    CLEAR_BIT(TIM2->CCMR1, TIM_CCMR1_CC1S);//настройка направления канала на выход
    SET_BIT(TIM2->CCMR1,TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);//Настройка на режим ШИМа
    CLEAR_BIT(TIM2->CCER, TIM_CCER_CC1P);//настройка полярности выхода (0 это 0 или 5 В)
    
    CLEAR_BIT(TIM2->CCMR1, TIM_CCMR1_OC1FE);//Отключение быстрого сравнения выходов
    CLEAR_BIT(TIM2->CR2, TIM_CR2_MMS);//Отключение мастер-режима таймера

    SET_BIT(TIM2->CCER,TIM_CCER_CC1E);//включение выхода

    MODIFY_REG(TIM2->CCR1,TIM_CCR1_CCR1, 4999);//настройка значения переключения ШИМ от 0 до 10000
    NVIC_EnableIRQ(TIM2_IRQn);//разрешаем прерывания в регистре контроллера прерываний NVIC
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); //Установка приоритета прерывания
    
    SET_BIT(TIM2->EGR,TIM_EGR_UG);//Перезагружаем счётчик TIM2
    SET_BIT(TIM2->CR1,TIM_CR1_CEN);//включение TIM2
    */
    
}

void ADC_Init(void)
{
    SET_BIT(ADC->CCR,ADC_CCR_ADCPRE_0);//предделитель АЦП
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN); //Включение тактированияе АЦП
    CLEAR_REG(ADC1->CR1); //Сброс битов
    CLEAR_REG(ADC1->CR2); //Сброс битов
    CLEAR_BIT(ADC1->CR2, ADC_CR2_ADON);// Выключение АЦП1
    CLEAR_BIT(ADC1->CR1, ADC_CR1_RES);// Установка разрешения АЦП1 на 12 бит
    SET_BIT(ADC1->CR2, ADC_CR2_CONT);// Включение непрерывных преобразований
    //SET_BIT(ADC1->CR1,ADC_CR1_EOCIE); // Включение прерываний

    //NVIC_EnableIRQ(ADC_IRQn);//разрешение прерываний NVIC
    //NVIC_SetPriority(ADC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0)); //Установка приоритета прерывания
    CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L); // число регулярных каналов 1
    SET_BIT(ADC1->SQR3, ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_0);// первое преобразование - канал 9
    
    CLEAR_BIT(ADC1->SMPR2, ADC_SMPR2_SMP9); //установка времени обработки канала 9
    
    //CLEAR_BIT(ADC1->CR2, ADC_CR2_EXTSEL); // выбор TIM1 CC1 event как источника запуска преобразований
    //SET_BIT(ADC1->CR2, ADC_CR2_EXTEN_0); //Включение внешнего триггера для обычных каналов по фронту 1-подъём, 2-спуск, 3-подъём/спуск
    SET_BIT(ADC1->CR2, ADC_CR2_ADON);// Включение АЦП1
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART); // запуск преобразования
    //ADC_DR - место хранения данных
}