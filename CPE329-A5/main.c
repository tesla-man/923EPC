#include "msp.h"
#include "delay.h"
#include "interrupt.h"
#include "timer.h"
#include "msp.h"
#include "gpio.h"


int main(void) {

    //set_DCO(FREQ_24_MHZ);

    //CS->KEY = CS_KEY_VAL;
    //CS->CTL1 |= CS_CTL1_SELS__DCOCLK;
    //CS->KEY = 0;
    initClock();

    gpioStruct gpio;
    gpio.port = PORT1;
    gpio.pins = PIN0;
    gpio.type = OUTPUT;
    gpio_init(&gpio);
    gpioClear(PORT1, PIN0);

    gpio.port = PORT4;
    gpio.pins = PIN3;
    gpio.type = OUTPUT;
    gpio.af = AF1;
    gpio_init(&gpio);

    gpio.port = PORT6;
    gpio.pins = PIN0;
    gpio.type = OUTPUT;
    gpio_init(&gpio);
    gpioClear(PORT6, PIN0);

    timerStruct timer;
    timer.channel = TIM0;
    timer.divide = DIV1;
    timer.mode = UPDN;
    timer.source = SMCLK;
    timer.intccr = CCR2;
    timer_init(&timer);

    timerCCR0(CCR0, 5000);
    timerCCR0(CCR2, 2500);

    initNVIC(TA0_N_IRQn);

    __enable_irq();

    while (1)
    {

    }
}


void TA0_N_IRQHandler(void) {
    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG;

        P1->OUT ^= BIT0;

}
