/*
 * BackChannel.c
 *
 *  Created on: Oct 3, 2014
 *      Author: gwilson
 */
#include <driverlib.h>
#include <strings.h>
#include "BCUart.h"
#include "BackChannel.h"
long BackChannel_BaudRate = 0;
bool BackChannel_Connected()
{
	return BackChannel_BaudRate > 0;
}

void BackChannel_Open(uint16_t baudrate)
{
	bcUartInit();
	//TODO:Support passing in the baud rate
	BackChannel_BaudRate = 57600;
}

void BackChannel_Write(unsigned char text[])
{
	uint16_t textLength;
	for(textLength = 0;text[textLength]!='\0'; ++textLength);
	bcUartSend(text, textLength);
}

void BackChannel_WriteLine(unsigned char text[])
{
	BackChannel_Write(text);
	BackChannel_Write("\r\n");
}
