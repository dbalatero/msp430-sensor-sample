#include <msp430.h> 
#include "HMC5883L.h"
#include <driverlib.h>
#include "BackChannel.h"
#include <stdio.h>
#include <math.h>

void initClocks(uint32_t mclkFreq);
void AddValueToStringAt(char * text[], int16_t val, uint8_t position);
char * itoa(uint16_t val);
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClocks(16000000);
    UCS_setExternalClockSource(32768, 4194304);

    BackChannel_Open(38400);
    BackChannel_WriteLine("Back channel active.");
    HMC_initialize();
    if (HMC_testConnection() != STATUS_SUCCESS)
    {
        BackChannel_WriteLine("Magnometer connection failed.");
        for(;;)
        	LPM0;//Die to low power mode
    }
    BackChannel_WriteLine("Magnometer initialized.");
    int16_t x, y, z;
    float headingFactor, heading;
    headingFactor = 180.0 / 3.14159265;//M_PI;
    unsigned char * readingText = "Reading:\tX=#####\tY=#####\tZ=#####";
    while(1)
    {
    	HMC_getHeading(&x,&y,&z);
    	AddValueToStringAt(&readingText, x, 11);
    	AddValueToStringAt(&readingText, y, 19);
    	AddValueToStringAt(&readingText, z, 27);
    	BackChannel_WriteLine(readingText);
    	heading = atan2(x,y) * headingFactor;
    	BackChannel_Write("Heading:  ");
    	BackChannel_WriteLine(itoa((int)heading));
    	__delay_cycles(100000);  // sample rate ~100 samples/sec
    }
}

void AddValueToStringAt(char * text[], int16_t val, uint8_t position)
{
	//Add the 5 digit value into str at position
	char * str = *text;
	str += position + 5;
	uint8_t len = 5;
	   do
	   {
	      *str = val%10 + 48;
	      val /= 10;
	      str--;
	      len--;
	   } while (val);
	   do {
		   *str = 32;
	   } while (len--);
}

char * itoa(uint16_t val)
{
	   char res[5];
	   int len = 0;
	   do
	   {
	      res[len] = val%10+48;
	      val/=10;
	      ++len;
	   } while (val > 0);
	   while(len++ < 5)
		   res[len] = 32; //Pa
	   res[len] = 0; //null-terminating

	   //now we need to reverse res
	   for(len = 0; len < 2; ++len)
	   {
	       char c = res[len]; res[len] = res[5-len-1]; res[5-len-1] = c;
	   }
	   return res;
}

void initClocks(uint32_t mclkFreq)
{
	// Assign the XT2 as the MCLK reference clock
	UCS_clockSignalInit(
	   UCS_MCLK,
	   UCS_XT2CLK_SELECT,
	   UCS_CLOCK_DIVIDER_1);

    // Assign the XT1 as the FLL reference clock
	UCS_clockSignalInit(
	   UCS_FLLREF,
	   UCS_XT1CLK_SELECT,
	   UCS_CLOCK_DIVIDER_1);

	// Assign the XT1 as the source for ACLK
	UCS_clockSignalInit(
	   UCS_ACLK,
	   UCS_XT1CLK_SELECT,
	   UCS_CLOCK_DIVIDER_1);

    UCS_initFLLSettle(
        mclkFreq/1000,
        mclkFreq/32768);
        //use REFO for FLL and ACLK
        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
}
