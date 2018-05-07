/*
 * keypad.h
 *
 *  Created on: Apr 16, 2018
 *      Author: Contovasilis
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "msp.h"
#include "gpio.h"
#include <stdint.h>

#define R1_P            PORT5
#define R2_P            PORT5
#define R3_P            PORT5
#define R4_P            PORT5

#define R1              PIN4
#define R2              PIN5
#define R3              PIN6
#define R4              PIN7

#define C1_P            PORT2
#define C2_P            PORT2
#define C3_P            PORT2

#define C1              PIN5
#define C2              PIN6
#define C3              PIN7

#define RWPORT           R1_P | R2_P | R3_P | R4_P
#define CLPORT           C1_P | C2_P | C3_P
#define RWPINS           R1 | R2 | R3 | R4
#define CLPINS           C1 | C2 | C3

void initKeypad();
void scanKeypad(uint8_t iflg);

volatile uint8_t key;
volatile uint8_t flag;


#endif /* KEYPAD_H_ */
