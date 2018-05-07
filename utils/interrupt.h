/*
 * interrupt.h
 *
 *  Created on: Apr 16, 2018
 *      Author: Contovasilis
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

#define HAL_IES          (uint32_t)&P1->IES -   (uint32_t)P1
#define HAL_IE           (uint32_t)&P1->IE -  (uint32_t)P1
#define HAL_IFG          (uint32_t)&P1->IFG -  (uint32_t)P1
#define HAL_IV           (uint32_t)&P1->IV -  (uint32_t)P1
#define RISING              0
#define FALLING             1
#define HWREG32(x)        (*((volatile uint32_t *)(x)))


void initExtInt(uint8_t selectedPort, uint32_t pins, uint8_t ies);
void enableExtInt(uint8_t selectedPort, uint32_t pins);
void disableExtInt(uint8_t selectedPort, uint32_t pins);
void writeIFG(uint8_t selectedPort, uint32_t pins);
uint8_t readIFG(uint8_t selectedPort, uint8_t mask);
void initNVIC(uint32_t irqNum);
void disableNVIC(uint32_t irqNum);
#endif /* INTERRUPT_H_ */
