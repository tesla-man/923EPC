#include "msp.h"
#include "delay.h"
#include "spi.h"

volatile uint16_t result = 0;
volatile uint8_t flag = 0;
int main(void)
{

/*
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses
*/
    // Configure UART pins
    initClock();
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    spiInit();


    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

    while(1){
        if(flag){
            driveDAC(result);
            result = 0;

            flag = 0;
        }
    }

}
// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{
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
    P1->OUT ^= BIT0;
    delay_ms(5);
    P1->OUT ^= BIT0;
}
