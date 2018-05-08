/*
 * delay.h
 *
 *  Created on: Apr 4, 2018
 *      Author: Contovasilis
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

#define FREQ_1_5_MHZ    (uint16_t)15
#define FREQ_3_MHZ      (uint16_t)30
#define FREQ_6_MHZ      (uint16_t)60
#define FREQ_12_MHZ     (uint16_t)120
#define FREQ_24_MHZ     (uint16_t)240
#define FREQ_48_MHZ     (uint16_t)480

#ifndef FCPU
#define FCPU    FREQ_12_MHZ
#endif

inline delay_ms(uint32_t time);
inline delay_us(uint32_t time);
void set_DCO(uint16_t freq);
void initClock();

#endif /* DELAY_H_ */
