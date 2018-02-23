#include <stdio.h>
#include <math.h>

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "tm_stm32f4_spi.h"

#include "cur_reader.h"


#define PERIOD (10000)/(5) - (1)
#define PRESCALER 8399


void TIM4_NVIC_Init(void);

void TIM4_Int_Init() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    TIM4_NVIC_Init();
    
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_NVIC_Init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void TIM4_IRQHandler(void) {
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        read_data();
    }

}

/* For now it will be init for pwm on TIM4
 *
 *
 */
void Init_ADS(void) {
    TIM4_Int_Init();
}


float a = 0.0;

void read_data(void) {
    float step = 0.1;
    printf("Currency:%f.5\n\r", sinf(a));
    if(a > 0xffffff)
    {
        a = 0;
    }
    a += step;

}
