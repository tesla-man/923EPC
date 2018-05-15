/*
 * gpio.h
 *
 *  Created on: Apr 9, 2018
 *      Author: Contovasilis
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "msp.h"
#include "lcd.h"

#define PIN0            0x0001
#define PIN1            0x0002
#define PIN2            0x0004
#define PIN3            0x0008
#define PIN4            0x0010
#define PIN5            0x0020
#define PIN6            0x0040
#define PIN7            0x0080

#define HAL_IN          (uint32_t)&P1->IN -   (uint32_t)P1
#define HAL_OUT         (uint32_t)&P1->OUT -  (uint32_t)P1
#define HAL_DIR         (uint32_t)&P1->DIR -  (uint32_t)P1
#define HAL_REN         (uint32_t)&P1->REN -  (uint32_t)P1
#define HAL_DS          (uint32_t)&P1->DS -   (uint32_t)P1
#define HAL_SEL0        (uint32_t)&P1->SEL0 - (uint32_t)P1
#define HAL_SEL1        (uint32_t)&P1->SEL1 - (uint32_t)P1


#define HWREG16(x)      (*((volatile uint16_t *)(x)))


typedef enum GPIO_AF{
    AF0 = 0,
    AF1,
    AF2,
    AF3
}GPIO_AF;

typedef enum GPIO_TYPE{
    OUTPUT=0,
    INPUT,
    INPUT_PU,
    INPUT_PD,
    AF
}GPIO_TYPE;

typedef enum GPIO_PORT{
    PORT1=1,
    PORT2,
    PORT3,
    PORT4,
    PORT5,
    PORT6
}GPIO_PORT;

typedef struct gpioStruct{
    GPIO_PORT port;
    uint32_t pins;
    GPIO_TYPE type;
    GPIO_AF af;
}gpioStruct;

void gpio_init(gpioStruct* gpio);
void gpioSet(uint_fast8_t selectedPort, uint_fast16_t pins);
void gpioClear(uint_fast8_t selectedPort, uint_fast16_t pins);
void gpioToggle(uint_fast8_t selectedPort, uint_fast16_t pins);
uint8_t gpioRead(uint_fast8_t selectedPort, uint_fast16_t pins);

#endif /* GPIO_H_ */
