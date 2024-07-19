#include <lpc17xx.h>  // Includes the header file for the LPC17xx microcontroller.
#include<stdio.h>
#define s LPC_SC  // Defines shortcuts for the system control block.
#define u LPC_UART0  // Defines shortcuts for the UART0 registers.

void pll() {
    s->SCS = (1 << 5);  // Sets the main oscillator enable bit.

    while ((s->SCS & (1 << 6)) == 0);  // Waits for the main oscillator to be ready.
    s->CLKSRCSEL = (1 << 0);  // Selects the main oscillator as the clock source.
    s->PLL0CON = (1 << 0);  // Enables the PLL.
    s->PLL0CFG = 14;  // Configures the PLL multiplier and divider.
    s->PLL0FEED = 0xAA;  // Sequence to update the PLL settings.
    s->PLL0FEED = 0x55;
    s->CCLKCFG = 5;  // Sets the CPU clock divider.
    while ((s->PLL0STAT & (1 << 26)) == 0);  // Waits for the PLL to lock.

    s->PLL0CON |= (1 << 1);  // Connects the PLL.

    s->PLL0FEED = 0xAA;															    
    s->PLL0FEED = 0x55;  // Sequence to update the PLL settings.
}

void basic_inituart0() {
    // Power on the UART0--PCONP--3rd bit (1)
    LPC_SC->PCONP |= (1 << 3);

    // Frequency
    // Pins TXD0, RXD0
    LPC_PINCON->PINSEL0 = (1 << 4) | (1 << 6);  // Selects the TXD0 and RXD0 pins for UART0.
    u->LCR = (1 << 0) | (1 << 1) | (1 << 7);  // Configures the word length and enables the DLAB (Divisor Latch Access Bit).
    u->DLL = 98;  // Sets the baud rate divisor.
    u->DLM = 0;   // Sets the baud rate divisor.
    u->LCR &= ~(1 << 7);  // Disables the DLAB.
}

   char name[20] = "Md Sajid Alam";int i=0;
int main()
 {
    pll();  // Calls the PLL initialization function.
    basic_inituart0();  // Calls the UART0 initialization function.

    // Transmit the full name "Md Sajid Alam"

 
    for (i=0;name[i]!='\0';i++) {
    while ((u->LSR & (1 << 5)) == 0);  // Waits for the THR to be empty.
    u->THR = name[i];
    }

    while (1);
}
