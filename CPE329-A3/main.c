#include "msp.h"
#include "lcd.h"
#include "gpio.h"

#define FCPU FREQ_24_MHZ
#include "delay.h"

void main(void){
	set_DCO(FCPU);


	lcdInit();
	moveCursor(3,1);
	sendCharacter(0x00);
	sendCharacter(0x01);
	sendCharacter(0x02);
	//moveCursor(3,2);
	sendCharacter(0xFF);
	sendCharacter(0xFF);
	sendCharacter(0xFF);

	while(1){
	}

}