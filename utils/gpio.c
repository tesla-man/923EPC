/*
 * gpio.c
 *
 *  Created on: Apr 9, 2018
 *      Author: Contovasilis
 */

#include "gpio.h"

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


void gpioSet(uint_fast8_t selectedPort, uint_fast16_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + HAL_OUT) |= pins;
}

void gpioClear(uint_fast8_t selectedPort, uint_fast16_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + HAL_OUT)  &= ~pins;
}

void gpioToggle(uint_fast8_t selectedPort, uint_fast16_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + HAL_OUT)  ^= pins;
}

uint8_t gpioRead(uint_fast8_t selectedPort, uint_fast16_t pins){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    return HWREG16(baseAddress + HAL_IN) & pins;
}

void gpio_init(gpioStruct* gpio){
    uint32_t baseAddress = GPIO_PORT_TO_BASE[gpio -> port];

    switch(gpio -> type){
        case(OUTPUT):
            HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_DIR) |= gpio -> pins;
            HWREG16(baseAddress + HAL_OUT) &= ~(gpio -> pins);
            break;
        case(INPUT):
            HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_DIR) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_REN) &= ~(gpio -> pins);
            break;
        case(INPUT_PU):
            HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_DIR) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_REN) |= gpio -> pins;
            HWREG16(baseAddress + HAL_OUT) |= gpio-> pins;
            break;
        case(INPUT_PD):
            HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_DIR) &= ~(gpio -> pins);
            HWREG16(baseAddress + HAL_REN) |= gpio -> pins;
            HWREG16(baseAddress + HAL_OUT) &= ~(gpio -> pins);
            break;
    }
        switch(gpio -> af){
            case(AF0):
                HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
                HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
            break;
            case(AF1):
                HWREG16(baseAddress + HAL_SEL0) |= gpio -> pins;
                HWREG16(baseAddress + HAL_SEL1) &= ~(gpio -> pins);
                break;
            case(AF2):
                HWREG16(baseAddress + HAL_SEL0) &= ~(gpio -> pins);
                HWREG16(baseAddress + HAL_SEL1) |= gpio -> pins;
                break;
            case(AF3):
                HWREG16(baseAddress + HAL_SEL0) |= gpio -> pins;
                HWREG16(baseAddress + HAL_SEL1) |= gpio -> pins;
                break;
    }
}

