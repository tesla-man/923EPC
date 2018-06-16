#include "msp.h"
#include "interrupt.h"
#include "delay.h"
#include "gpio.h"
#include "timer.h"
#include "lcd.h"

/**
 * main.c
 */

volatile uint32_t adcResults[6], rawdist;
volatile uint8_t deb = 0;
int main(void) {

    initClock();
    lcdInit();

    P5->SEL1 |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5; // Configure ADC pins
    P5->SEL0 |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;


    P1->OUT |= BIT5; //Configure button pin
    P1->REN |= BIT5;


    // Enable global interrupt
    __enable_irq();

    // Enable ADC interrupt in NVIC module
    timerStruct timer;
    timer.channel = TIM0;
    timer.divide = DIV1;
    timer.mode = UPDN;
    timer.source = SMCLK;
    timer.intccr = CCR0;
    timer_init(&timer);

    timerCCR0(CCR0, 15000); //PWM timer frequency
    timerCCR0(CCR1, 1);
    timerCCR0(CCR2, 1);

    timer.channel = TIM1;
    timer.divide = DIV8;
    timer.mode = UP;
    timer.source = SMCLK;
    timer.intccr = CCR0;
    timer_init(&timer);
    TIMER_A1->EX0 = TIMER_A_EX0_TAIDEX_7;
    TIMER_A1->CCR[0] = 50000;   //display update timer frequency

    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_2;
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_2;

    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2      |
                  ADC14_CTL0_SHP         |
                  ADC14_CTL0_ON          |
                  ADC14_CTL0_SSEL__SMCLK |
                  ADC14_CTL0_DIV_7       |
                  ADC14_CTL0_MSC         |
                  ADC14_CTL0_CONSEQ_3;

    ADC14->CTL1 = ADC14_CTL1_RES_3;   // Use sampling timer, 14-bit conversion results

    initNVIC(ADC14_IRQn);

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0;
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_1;
    ADC14->MCTL[2] |= ADC14_MCTLN_INCH_2;
    ADC14->MCTL[3] |= ADC14_MCTLN_INCH_3;
    ADC14->MCTL[4] |= ADC14_MCTLN_INCH_4;
    ADC14->MCTL[5] |= ADC14_MCTLN_INCH_5 | ADC14_MCTLN_EOS;
    ADC14->IER0 |= ADC14_IER0_IE1;
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;

    sendStringXY("CPE 329 Maglev Project", 2, 1);
    sendStringXY("Project", 5, 2);

    delay_ms(3000);

    sendCommand(CLEAR);

    initNVIC(TA1_0_IRQn);
    sendStringXY("Levitation:", 1, 2);
    sendStringXY(" OFF", 12, 2);
    while(1){

    }
}

/*This is the ADC interrupt handler which reads the joystick
 * poteniometers and hall effect sensors. The raw ADC data
 * is filtered with an exponential moving average in order
 * to limit noise interference and improve stability. The
 * readings from the 4 sensors are averaged so that axial
 * motion of the magnet does not destabilize the levitation
 * control loop.
 *
 */
void ADC14_IRQHandler(void){
    static uint8_t alpha = 100;
    static uint16_t oldSample = 0;
    uint8_t i=0;

    static uint32_t negnormdist, posnormdist, avgRead = 0;

    //exponential moving average filter
    while(i<6){
        oldSample = adcResults[i];
        adcResults[i++] = (alpha * ADC14->MEM[i] + (256-alpha)*oldSample)/256;
    }

    //average 4 sensors
    avgRead = (adcResults[2] + adcResults[3] + adcResults[4] + adcResults[5])/4;

    //apply hard limits to distance range
     if(avgRead > 8770){
         if(avgRead < 12780){
             rawdist = avgRead;
         }
         else{
             rawdist = 12750;
         }
     }
     else{
         rawdist = 8770;
     }

     //calculate magnet distance referenced from the bottom and top of the levitator
     posnormdist = (rawdist*376 - 3297520)/100;
     negnormdist = 15000-posnormdist; //null = 4400

     //adjust pwm duty cycle based on magnet position
     if(posnormdist > 4500){
         TIMER_A0->CCR[1] = posnormdist+1000;
         TIMER_A0->CCR[2] = 1;
     }
     else{
         TIMER_A0->CCR[1] = 1;
         TIMER_A0->CCR[2] = negnormdist;
     }
}

/*This is the interrupt handler which updates the LCD and checks
 * the button at a frequency of approximately 1KHz. This is done
 * so that the MSP processor is not bogged down performing these
 * operations too often, which can cause excessive lag in the
 * levitation control loop
 */
void TA1_0_IRQHandler(void) {
    static uint8_t flag = 1;
    static uint32_t x = 0;

    if(deb){
        if(flag){
            //activate levitation
            P2->DIR |= BIT4 | BIT5;
            P2->SEL0 |= BIT4 | BIT5;
            sendStringXY(" ON ", 12, 2);
            delay_ms(100);
            deb = 0;
            flag ^= 1;
        }
        else
        {
            //disable levitation
            P2->SEL0 &= ~(BIT4 | BIT5);
            P2->OUT &= ~(BIT4 | BIT5);
            sendStringXY(" OFF", 12, 2);
            delay_ms(100);
            deb = 0;
            flag ^= 1;
        }
    }


    if(!gpioRead(PORT1, PIN5)){ //check button
        deb = 1;
    }

    //print x on lcd based on magnet position
    x = (rawdist*4020 - 35255400)/1000000;

    if(flag){
        sendStringXY("                ", 1, 1);
        sendStringXY("x", x, 1);
    }


    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}
