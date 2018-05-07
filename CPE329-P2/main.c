#include "msp.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include <math.h>
#include "timer.h"
#include "interrupt.h"
#include "lut.h"
#include "keypad.h"
/**
 * main.c
 */

volatile uint16_t freq = 1;
volatile uint8_t edge = 0;

void main(void){
    uint16_t duty = 50;     //set initial duty cycle (50%)

    uint16_t freqc = 30000; //set initial square wave frequency (100Hz)
    uint16_t DC = 15000;    //set initial square wave duty cycle (50%)

    initClock();            //enable HFXT @ 48MHZ

    timerStruct timer;      //Initialize timer A0
    timer.channel = TIM0;
    timer.divide = DIV1;
    timer.mode = UP;
    timer.source = SMCLK;
    timer.intccr = CCR0;
    timer_init(&timer);
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;

    timer.channel = TIM1;   //Initialize timer A1
    timer.divide = DIV8;
    timer.mode = UPDN;
    timer.source = SMCLK;
    timer.intccr = CCR1;
    timer_init(&timer);

    timerCCR0(CCR0, 233);   //DAC update frequency (496kHz) (sine)
    timerCCR0(CCR1, 116);   //DAC update frequency (496kHz) (ramp)
    timerCCR1(CCR0, freqc); //set square wave frequency
    timerCCR1(CCR1, DC);    //set square wave dc

    initNVIC(TA1_N_IRQn);   //enable square
    initKeypad();           //init keypad
    spiInit();              //init spi


    /*MAIN LOOP:
     * The main loop of the program checks if the key has been pressed
     * and then performs an action based on this press. The different
     * waveforms are enabled by enabling and disabling specific
     * NVIC interrupt vectors which generate the waveforms.
     */
    while(1){

        if(flag){
           switch(key){
           case('1'):
               freq = 1;
               break;
           case('2'):
               freq = 2;
               break;
           case('3'):
               freq = 3;
               break;
           case('4'):
               freq = 4;
               break;
           case('5'):
               freq = 5;
               break;
           case('7'):
               disableNVIC(TA0_0_IRQn);
               disableNVIC(TA0_N_IRQn);
               initNVIC(TA1_N_IRQn);
               break;
           case('8'):
               disableNVIC(TA1_N_IRQn);
               disableNVIC(TA0_N_IRQn);
               initNVIC(TA0_0_IRQn);
               break;
           case('9'):
               disableNVIC(TA1_N_IRQn);
               disableNVIC(TA0_0_IRQn);
               initNVIC(TA0_N_IRQn);
               break;
           case('*'):
               if(duty > 10){
                   duty -= 10;
               }

               break;
           case('#'):
               if(duty < 90){
                   duty += 10;
               }
               break;
           case('0'):
               duty = 50;
               break;

           }
           flag = 0;
           DC = ((30000/freq)*duty)/100;
           freqc = 30000/freq;
           timerCCR1(CCR0, freqc);
           timerCCR1(CCR1, DC);
           TIMER_A1->CTL |= TIMER_A_CTL_CLR;
           edge = 0;
        }

    }
}

/* Sine wave generation
 * This interrupt handler is run at 496KHz when the
 * TA0_0_IRQn NVIC vector is enabled. The handler
 * writes a value from the sine look up table with
 * the dacval key. This key is incremented after each
 * spi write and is reset after it reaches 4095.
 * The frequency of the wave is achieved by skipping
 * values in the LUT in order to maintain constant DAC
 * output points per second
 */
void TA0_0_IRQHandler(void) {
    static uint16_t dacval = 0;
    EUSCI_A1->TXBUF = ((sinlut[dacval] & 0xF00) | 0x7000)>>8;
    EUSCI_A1->TXBUF = (sinlut[dacval] & 0xFF);
    dacval += freq;
    if(dacval > 4095){
        dacval = 0;
    }

    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear IFG
}

/* Ramp wave generation
 * This interrupt handler is run at 496KHz when the
 * TA0_N_IRQn NVIC vector is enabled. The handler
 * writes the dacval value to the DAC which is
 * incremented after each interrupt and is reset
 * after it reaches 4095.  * The frequency of the
 * wave is achieved by skipping values in the LUT
 * in order to maintain constant DAC output points
 * per second
 */
void TA0_N_IRQHandler(void) {
    static uint16_t dacval = 0;
    EUSCI_A1->TXBUF = ((dacval & 0xF00) | 0x7000)>>8;
    EUSCI_A1->TXBUF = (dacval & 0xFF);
    dacval += freq;
    if(dacval > 4095){
        dacval = 0;
    }

    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; //clear IFG
}

/*Square wave generation
 * THis interrupt handler is run whenever timer A1
 * reaches the CCR1 value. The CCR0 value controls the
 * frequency of the square wave and the CCR1 value
 * controls the duty cycle. The "edge" variable is toggled
 * after each interrupt and it multiplied by 4095 in order
 * to toggle the dac write value.
 */
void TA1_N_IRQHandler(void){
    driveDAC(4095*edge);
    edge ^= 1;
    TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; //clear IFG
}
