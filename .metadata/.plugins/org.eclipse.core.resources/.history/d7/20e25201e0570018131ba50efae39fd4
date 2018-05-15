/*
 * timer.c
 *
 *  Created on: Apr 23, 2018
 *      Author: Contovasilis
 */

#include "timer.h"


void timer_init(timerStruct* timer){
    uint32_t baseAddress = TIMER_NUM_TO_BASE[timer -> channel];

    HWREG16(baseAddress + HAL_CTL) |= ((timer->source)<<8) | ((timer->divide)<<6) | ((timer->mode)<<4) | TIMER_A_CTL_CLR | 0x01;

    HWREG16(baseAddress + HAL_CCTL(timer->intccr)) |= TIMER_A_CCTLN_CCIE;
}

void timerCCR0(uint8_t channel, uint16_t ccrvalue){
    TIMER_A0->CCR[channel] = ccrvalue;
}

void timerCCR1(uint8_t channel, uint16_t ccrvalue){
    TIMER_A1->CCR[channel] = ccrvalue;
}
