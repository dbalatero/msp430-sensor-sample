#include <msp430.h>
#include <msp430f5529.h>
#include <inttypes.h>
#include <stdbool.h>
#define NUM_BYTES_TX 2                         // How many bytes?#define NUM_BYTES_RX 6
#define HMC5883     0x1E
int RXByteCtr, RPT_Flag = 0;       // enables repeated start when 1
volatile unsigned char RxBuffer[6];         // Allocate 6 byte of RAM
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr, RX = 0;
unsigned int magnetometer_data[3];
unsigned char MSData[3];
void Setup_TX(unsigned char);
void Setup_RX(unsigned char);
void Transmit(unsigned char, unsigned char);
uint8_t Receive(unsigned char);
void Receive6(unsigned char Reg_AD, volatile unsigned char * rBuffer);
const unsigned long int MaxWait = 100000;
unsigned long maxWait;
unsigned char dataReady = 0;
int main0(void) {
	int i;
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P4SEL |= BIT1 + BIT2;                          // Assign I2C pins to USCI_B1
	P1DIR |= BIT5;
	P1DIR |= BIT1;
	P1OUT |= BIT5;
	P2DIR &= ~BIT6;
	P2IE |= BIT6;
	P2IES |= BIT6;
	Setup_TX(HMC5883);
	RPT_Flag = 0;
	Transmit(0x00, 0x70);
	while (UCB1CTL1 & UCTXSTP)
		;              // Ensure stop condition got sent
	Setup_TX(HMC5883);
	RPT_Flag = 0;
	Transmit(0x01, 0xA0);
	while (UCB1CTL1 & UCTXSTP)
		;
	Setup_TX(HMC5883);
	RPT_Flag = 0;
	Transmit(0x02, 0x00);
	while (UCB1CTL1 & UCTXSTP)
		;
	while (1) {
		//Receive process
		Receive6(0x03, RxBuffer);
//          RxBuffer[0]= Receive(0x03);           //Data Output X MSB Register
//          RxBuffer[1]=Receive(0x04);            //Data Output X LSB Register
//          RxBuffer[2]=Receive(0x07);            //Data Output Y MSB Register
//          RxBuffer[3]=Receive(0x08);            //Data Output Y LSB Register
//          RxBuffer[4]=Receive(0x05);            //Data Output Z MSB Register
//          RxBuffer[5]=Receive(0x06);            //Data Output Z LSB Register
		while (UCB1CTL1 & UCTXSTP)
			;             // Ensure stop condition got sent
		for (i = 0; i < 3; ++i) {
			magnetometer_data[i] = (int) RxBuffer[2 * i + 1]
					+ (((int) RxBuffer[2 * i]) << 8);
		}
		__delay_cycles(1000);  // sample rate ~100 samples/sec
							   // you can change by changing delay
	}
}
//-------------------------------------------------------------------------------
// The USCI_B1 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
	if (RX == 1) {                              // Master Recieve?
		RXByteCtr--;                              // Decrement RX byte counter
		if (RXByteCtr) {
			*PRxData++ = UCB1RXBUF;           // Move RX data to address PRxData
			_NOP();
		} else {
			if (RPT_Flag == 0)
				UCB1CTL1 |= UCTXSTP;        // No Repeated Start: stop condition
			if (RPT_Flag == 1) {                // if Repeated Start: do nothing
				RPT_Flag = 0;
			}
			*PRxData = UCB1RXBUF;               // Move final RX data to PRxData
			__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
		}
	} else {                                     // Master Transmit
		if (TXByteCtr)                        // Check TX byte counter
		{
			UCB1TXBUF = MSData[TXByteCtr];          // Load TX buffer
			TXByteCtr--;                            // Decrement TX byte counter
		} else {
			if (RPT_Flag == 1) {
				RPT_Flag = 0;
				TXByteCtr = NUM_BYTES_TX;                // Load TX byte counter
				__bic_SR_register_on_exit(CPUOFF);
			} else {
				UCB1CTL1 |= UCTXSTP;                  // I2C stop condition
				UCB1IFG &= ~UCTXIFG;                // Clear USCI_B1 TX int flag
				__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
			}
		}
	}
}
void Setup_TX(unsigned char Dev_ID) {
	_DINT();
	RX = 0;
	UCB1IE &= ~UCRXIE;
	while (UCB1CTL1 & UCTXSTP)
		;               // Ensure stop condition got sent// Disable RX interrupt
	UCB1CTL1 |= UCSWRST;                      // Enable SW reset
	UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB1BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	UCB1BR1 = 0;
	UCB1I2CSA = Dev_ID;                         // Slave Address is 048h
	UCB1CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	UCB1IE |= UCTXIE;                          // Enable TX interrupt
}
void Transmit(unsigned char Reg_ADD, unsigned char Reg_DAT) {
	maxWait = MaxWait;
	MSData[2] = Reg_ADD;
	MSData[1] = Reg_DAT;
	TXByteCtr = NUM_BYTES_TX;                  // Load TX byte counter
	while (UCB1CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
	UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}
uint8_t Receive(unsigned char Reg_AD) {
	uint8_t receivedByte;
	Setup_TX(HMC5883);
	while (UCB1CTL1 & UCTXSTP && maxWait-- > 0)
		;             // Ensure stop condition got sent
	UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	while ((UCB1IFG & UCTXIFG) == 0 && maxWait-- > 0)
		;
	UCB1TXBUF = Reg_AD;          // Load TX buffer
//        while((UCB1IFG & UCTXIFG)==0);
	UCB1CTL1 &= ~UCTR;
	UCB1CTL1 |= UCTXSTT + UCTXNACK;
	while (UCB1CTL1 & UCTXSTT && maxWait-- > 0)
		;
	receivedByte = UCB1RXBUF;
	UCB1CTL1 |= UCTXSTP;
	return receivedByte;
}
void Receive6(unsigned char Reg_AD, volatile unsigned char rBuffer[6]) {
	maxWait = MaxWait;
	Setup_TX(HMC5883);
	while (UCB1CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
	UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	while ((UCB1IFG & UCTXIFG) == 0 && maxWait-- > 0)
		;
	UCB1TXBUF = Reg_AD;          // Load TX buffer
	while ((UCB1IFG & UCTXIFG) == 0 && maxWait-- > 0)
		;
		UCB1CTL1 &= ~UCTR;
		__delay_cycles(100);
		UCB1CTL1 |= UCTXSTT;// + UCTXNACK;
	int i;
	for (i = 0; i < 6; i++) {
		while (UCB1CTL1 & UCTXSTT && maxWait-- > 0)
			;
		rBuffer[i] = UCB1RXBUF;
	}
	UCB1CTL1 |= UCTXSTP;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
	if (P2IFG & BIT6)
		dataReady = true;
	P2IFG &= ~BIT6;
	LPM0_EXIT;
}
