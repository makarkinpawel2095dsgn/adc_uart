#include "apm32f10x.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_tmr.h"
#include "tim_adc.h"

// freqwensy 1us 
// F_osc(72_000_000)/F_tim
#define TIMER_PRESCALE 36000

uint16_t previousState;

TMR_BaseConfig_T TIMER_init;

void init_timer()
{
    // enable clock periph gpio tim
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_GPIOD); 
    RCM_EnableAPB1PeriphClock( RCM_APB1_PERIPH_TMR4 );
    GPIO_Config_T GPIO_Config_portD;
    // output pin 
    GPIO_Config_portD.mode  = GPIO_MODE_OUT_PP;
    GPIO_Config_portD.pin   = GPIO_PIN_10;
    GPIO_Config_portD.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOD, &GPIO_Config_portD);

    //
    TIMER_init.period = 50;
    TIMER_init.division = TIMER_PRESCALE-1;
    // init struct timer
    TMR_ConfigTimeBaseStructInit(&TIMER_init);
    // enable timer 4
    TMR_Enable(TMR4);
    
    /*
    //TMR_ICConfig_T timer_ic;
    timer_ic.channel = TMR_CHANNEL_1;
    timer_ic.polarity =TMR_IC_POLARITY_RISING;
    timer_ic.prescaler = TMR_IC_PSC_1;
    timer_ic.selection = TMR_IC_SELECTION_DIRECT_TI;
    timer_ic.filter = 0x00;
    //
    TMR_ConfigICStructInit(&timer_ic);
    */
}

void TIM4_IRQHendler ()
{
    // 
    if (previousState == 0)
    {
        // enable state 
        previousState = 1;
        GPIO_SetBit(GPIOD, GPIO_PIN_10);
        TIMER_init.period = 50;
        // init struct timer
        TMR_ConfigTimeBaseStructInit(&TIMER_init);
        // enable timer 4
        TMR_Enable(TMR4);
        TMR_ClearStatusFlag(TMR4, TMR_FLAG_UPDATE);
    }
    else
    {
        previousState = 0;
        GPIO_ResetBit(GPIOD, GPIO_PIN_10);
        TIMER_init.period = 200;
        // init struct timer
        TMR_ConfigTimeBaseStructInit(&TIMER_init);
        // enable timer 4
        TMR_Enable(TMR4);
    }
}