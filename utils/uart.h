/*
 * uart.h
 *
 *  Created on: May 7, 2018
 *      Author: Contovasilis
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uartInit();
uint16_t checkCR();
void EUSCIA0_IRQHandler(void);

#endif /* UART_H_ */
