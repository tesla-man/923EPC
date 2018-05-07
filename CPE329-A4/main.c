#include "msp.h"
#include "gpio.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"


/**
 * main.c
 */

void updateDisplay();

void main(void){
    set_DCO(FREQ_24_MHZ);           //set DCO to 24MHZ
    lcdInit();                      //initialize display
    initKeypad();                   //initialize keypad

    while(1){
        delay_ms(50);
        updateDisplay();

    }
}

void updateDisplay(){
    sendCommand(CLEAR);
    sendCharacter(key);
    sendCommand(HOME);
}

