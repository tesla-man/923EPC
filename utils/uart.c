/*
 * uart.c
 *
 *  Created on: May 7, 2018
 *      Author: Contovasilis
 */
#include "uart.h"
#include "gpio.h"
#include "interrupt.h"

volatile uint16_t result, uartflag = 0;


void uartInit(){
    gpioStruct gpio;        //Configure UART pins
    gpio.port = PORT1;
    gpio.pins = PIN2 | PIN3;
    gpio.type = OUTPUT;
    gpio.af = AF1;
    gpio_init(&gpio);

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    EUSCI_A0->BRW = 26;                     // 48000000/16/115200
    EUSCI_A0->MCTLW = EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    // Enable eUSCIA0 interrupt in NVIC module
    initNVIC(EUSCIA0_IRQn);
}

uint16_t checkCR(){
    uint16_t tmp;
    if(uartflag){
        //tmp = result;
        //result = 0;
        uartflag = 0;
        return 1;
    }
    return 50;
}

void uartWrite(uint16_t data){
    uint8_t num[4] = {0, 0, 0, 0};
    uint8_t i=3;

    while (data != 0)
    {
        int rem = data % 10;
        num[i--] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        data = data/10;
    }
    i = 0;
    while(i<4){
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = num[i++];
    }

    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;     //write New line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0D;     //write carraige return

}

void uartWriteByte(uint8_t data){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(void){
    uint8_t datau = 0;
    static uint8_t i = 0;
    char *click="\033[M";
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        datau = (EUSCI_A0->RXBUF);

        while(datau == click[i]){
            i++;
        }
        if(i==3){
            uartflag = 1;
            i = 0;
        }

    }
}

int fputc(int _c, register FILE *_fp)
{
  uartWriteByte((unsigned char) _c);

  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++){
      uartWriteByte((unsigned char) _ptr[i]);
  }

  return len;
}
