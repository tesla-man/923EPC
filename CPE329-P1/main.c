#include "msp.h"
#include "gpio.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

/**
 * main.c
 */

void updateDisplay(uint8_t state);
uint8_t arrayCmp(uint8_t keyArray[4]);
void checkPwd();



void main(void){

    set_DCO(FREQ_24_MHZ);   //set DCO frequency to 24MHZ
    lcdInit();              //Initialize LCD
    //initKeypad();           //Initialize keypad pins and interrupts
    updateDisplay(0);       //Print lock screen

    while(1){
        //delay_ms(50);
        //checkPwd();         //check key array, and '*'
    }
}

/*
 * This function manages writes to the LCD. It takes in an integer
 * as an argument and uses it to run 1 of 4 possible cases. Case 0
 * prints the lock screen and clears any printed keystrokes.
 * Case 1 prints the pressed key. Case 2 prints the unlock screen.
 * Case 3 prints the lock vibration animation for an incorrectly
 * entered passcode.
 */
void updateDisplay(uint8_t state){
    static uint8_t flag=0;
    uint8_t i;

    switch(state){
    case 0:
        sendCommand(CLEAR);
        sendCharacterXY(0x00, 2, 1);
        sendCharacter(0x01);
        sendCharacter(0x02);
        sendCharacterXY(0xFF, 2, 2);
        sendCharacter(0xFF);
        sendCharacter(0xFF);
        sendStringXY("LOCKED", 9, 1);
        sendStringXY("KEY:", 8, 2);
        flag = 0;
        break;
    case 1:
        if(!flag){
            moveCursor(12, 2);
            flag = 1;
        }
        sendCharacter(key);
        break;
    case 2:
        delay_ms(500);
        sendCommand(CLEAR);
        sendCharacterXY(0x00, 4, 1);
        sendCharacter(0x01);
        sendCharacter(0x02);
        sendCharacterXY(0xFF, 2, 2);
        sendCharacter(0xFF);
        sendCharacter(0xFF);
        sendStringXY("HELLO", 9, 1);
        sendStringXY("WORLD", 9, 2);
        break;
    case 3:
        delay_ms(500);
        for(i=1;i<7;i++){
            delay_ms(80);
            sendCharacterXY(0x20, 2, 1);
            sendCharacterXY(0x20, 2, 2);
            sendCharacterXY(0x20, 5, 1);
            sendCharacterXY(0x20, 5, 2);

            sendCharacterXY(0x00, 2+(i%2), 1);
            sendCharacter(0x01);
            sendCharacter(0x02);
            sendCharacterXY(0xFF, 2+(i%2), 2);
            sendCharacter(0xFF);
            sendCharacter(0xFF);
        }
        updateDisplay(0);
    }
}

/*
 * This function checks if a key was recently pressed using the
 * interrupt flag variable. It then checks if an asterisk was
 * pressed. Finally, it checks if the key array and the passcode
 * array are equal after the 4th keystroke.
 */
void checkPwd(){
    static uint8_t keyArray[4];     //Initialize array for keystrokes
    static uint8_t i=0;             //Initialize interator

    if(flag){                       //check interrupt flag
        updateDisplay(1);           //print pressed key to LCD
        keyArray[i] = key;          //set key to next array position
        i++;                        //increment iterator
        flag = 0;                   //clear interrupt flag

        if(key == '*'){             //check for asterisk
            updateDisplay(0);       //clear keystrokes if asterisk
            i=0;                    //reset iterator
        }

        if(i>3){                    //check for 4 keystrokes
            i=0;                    //reset iterator
            if(arrayCmp(keyArray)){ //check if key array matches the passcode
                updateDisplay(2);   //display unlock screen if correct
            }
            else{
                updateDisplay(3);   //display lock screen if incorrect
            }
        }
    }
}

/*This function is used by checkPwd to determine if the key
 * array and the passcode array are equal.
 */
uint8_t arrayCmp(uint8_t keyArray[]){
    static uint8_t pwd[4] = {'1','3','5','7'};
    uint8_t i;
    for(i=0;i<4;i++){
        if(keyArray[i] != pwd[i]){
            return 0;
        }
    }
    return 1;
}
