#include "msp.h"
#include "delay.h"
#include <stdint.h>



uint8_t TXData[255]= {};
uint8_t RXData[255]= {};
uint8_t flag = 1;

void i2cWrite(uint8_t slaveAddress, char *dataArray, uint8_t dataNum);
void i2cRead(uint8_t slaveAddress, char *dataAddress);
int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    P1->OUT &= ~BIT0;                       // Clear P1.0 output latch
    P1->DIR |= BIT0;                        // For LED


    P1->SEL0 |= BIT6 | BIT7;                // I2C pins
    P1->REN |= BIT6 | BIT7;




    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIB0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    // Configure USCI_B0 for I2C mode
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // Master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_TR  |             // Transmit mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK

    EUSCI_B0->CTLW1 |= EUSCI_B_CTLW1_ASTP_2;// Automatic stop generated
    //EUSCI_B0->TBCNT = 0x0001;               // number of bytes to be received

    EUSCI_B0->BRW = 30;                     // baudrate = SMCLK / 30 = 100kHz


    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Release eUSCI from reset

    EUSCI_B0->IE |= EUSCI_B_IE_TXIE0 |      // Enable transmit interrupt
            EUSCI_A_IE_RXIE |
            EUSCI_B_IE_BCNTIE |
            EUSCI_B_IE_NACKIE;

    char *data = "ABC";
    char *address = "AB";

    //i2cWrite(0x50, data, 3);
    //delay_ms(20);

    i2cRead(0x50, address);




        while(1){

        }
}

// I2C interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    static uint8_t i = 0;

    if (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_NACKIFG;
        P1->OUT |= BIT0;
        // I2C start condition
        UCB0CTL1 |= EUSCI_B_CTLW0_TXSTT;
    }

    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0;
        EUSCI_B0->TXBUF = TXData[i];
        i++;



    }

    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG0;
        RXData[i] = EUSCI_B0->RXBUF;
        i++;

    }

    if (EUSCI_B0->IFG & EUSCI_B_IFG_BCNTIFG)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_BCNTIFG;
        P1->OUT ^= BIT0;
        i=0;
        flag = 0;
    }
}

void i2cWrite(uint8_t slaveAddress, char *dataArray, uint8_t dataNum){
    uint8_t i = 0;
    EUSCI_B0->TBCNT = dataNum;
    EUSCI_B0->I2CSA = slaveAddress;
    while(i < dataNum){
        TXData[i] = dataArray[i];
        i++;
    }
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR | EUSCI_B_CTLW0_TXSTT;

}

void i2cRead(uint8_t slaveAddress, char *dataAddress){
    flag = 1;
    i2cWrite(slaveAddress, dataAddress, 2);
    delay_ms(20);
    while(flag);
    EUSCI_B0->TBCNT = 1;
    EUSCI_B0->I2CSA = slaveAddress;

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR;
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

}

