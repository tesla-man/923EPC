#include "msp.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"
#include "interrupt.h"
/**
 * main.c
 */
volatile uint16_t dacval = 0;

void main(void)
{
    initClock();            //enable HFXT @ 48MHZ

    P1->DIR |= BIT0;

    timerStruct timer;      //Initialize timer A0
    timer.channel = TIM0;
    timer.divide = DIV1;
    timer.mode = UP;
    timer.source = SMCLK;
    timer.intccr = CCR0;
    timer_init(&timer);

    timerCCR(CCR0, 47);    //set CCR0 value to 47

    initNVIC(TA0_0_IRQn);

    __enable_irq();

    spiInit();              //set up SPI peripheral

    while(1){
        driveDAC(dacval);   //write to DAC
    }
}

//Triangle Wave Generation
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear IFG

    static uint8_t dir = 0;                    //count dir flag

    if(dir){
        dacval++;                              //count up
    }
    else{
        dacval--;                               //count down
    }

    if(dacval == 2500){                         //change dir at 2V
        dir = 0;
    }

    if(dacval == 0){                            //change dir at 0V
        dir = 1;
    }
}
