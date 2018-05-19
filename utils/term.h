/*
 * terminal.h
 *
 *  Created on: May 14, 2018
 *      Author: Contovasilis
 */

#ifndef TERM_H_
#define TERM_H_

#include <stdio.h>
#include <stdint.h>

#define TERM_COL    100
#define TERM_ROW    30

#define BRIGHT      1
#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

void termDmmSplash(void);
void termMain(void);
void drawBox(uint8_t x, uint8_t y, uint8_t height, uint8_t width);
void drawHLine(uint8_t x, uint8_t y, uint8_t width);
void drawVLine(uint8_t x, uint8_t y, uint8_t height);
void drawBarGraph(uint8_t x, uint8_t y, uint8_t width);
void text(uint8_t x, uint8_t y, char *string);
void bigText(uint8_t x, uint8_t y, char *string);
void textColor(uint8_t fcolor, uint8_t bcolor);
void highlight(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void updateTermMode(uint8_t mode);
void updateRMS(uint16_t rms);
void updateFREQ(uint16_t freq);
void updatedcavg(int dcavg);
void updatevpp(int vpp);
#endif /* TERM_H_ */
