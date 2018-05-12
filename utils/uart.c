/*
 * uart.c
 *
 *  Created on: May 7, 2018
 *      Author: Contovasilis
 */
#include "uart.h"
#include "gpio.h"
#include "interrupt.h"

volatile uint16_t result;
volatile uint8_t flag = 0;

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
    if(flag){
        tmp = result;
        result = 0;
        flag = 0;
        return tmp;
    }
    return 4096;
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

void EUSCIA0_IRQHandler(void){
    uint8_t data = 0;

    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        if((EUSCI_A0->RXBUF) != 0x0D){
            data = (EUSCI_A0->RXBUF) & 0x0F;
            result = result*10 + data;
        }
        else{
            flag = 1;
            while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
            EUSCI_A0->TXBUF = 0x0A;
        }

        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
    }
}
