/*
 * uart.h
 *
 *  Created on: May 7, 2018
 *      Author: Contovasilis
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

void uartInit();
uint16_t checkCR();
void EUSCIA0_IRQHandler(void);
void uartWrite(uint16_t data);
void uartWriteByte(uint8_t data);
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);

#endif /* UART_H_ */
