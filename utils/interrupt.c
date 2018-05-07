/*
 * interrupt.c
 *
 *  Created on: Apr 16, 2018
 *      Author: Contovasilis
 */

#include "interrupt.h"
#include "msp.h"

static uint32_t GPIO_PORT_TO_BASE[] =
{   0x00,
    (uint32_t)P1,
    (uint32_t)P1+1,
    (uint32_t)P3,
    (uint32_t)P3+1,
    (uint32_t)P5,
    (uint32_t)P5+1,
    (uint32_t)P7,
    (uint32_t)P7+1,
    (uint32_t)P9,
    (uint32_t)P9+1
};

void initExtInt(uint8_t selectedPort, uint32_t pins, uint8_t ies){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];
    if(ies){
        HWREG32(baseAddress + HAL_IES) |= pins;
    }
    else{
        HWREG32(baseAddress + HAL_IES) &= ~pins;
    }
}

void enableExtInt(uint8_t selectedPort, uint32_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];
    writeIFG(selectedPort, pins);
    HWREG32(baseAddress + HAL_IE) |= pins;
}

void disableExtInt(uint8_t selectedPort, uint32_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];
    HWREG32(baseAddress + HAL_IE) &= ~pins;
}
void writeIFG(uint8_t selectedPort, uint32_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];
    HWREG32(baseAddress + HAL_IFG) &= ~pins;
}
uint8_t readIFG(uint8_t selectedPort, uint8_t mask){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];
    return HWREG32(baseAddress + HAL_IFG) & mask;
}

void initNVIC(uint32_t irqNum){
    if(irqNum > 31){
        NVIC->ISER[1] = 1<<((irqNum)&31);
    }
    else{
        NVIC->ISER[0] = 1<<((irqNum)&31);
    }
}

void disableNVIC(uint32_t irqNum){
    if(irqNum > 31){
        NVIC->ICER[1] = 1<<((irqNum)&31);
    }
    else{
        NVIC->ICER[0] = 1<<((irqNum)&31);
    }
}
