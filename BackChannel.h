/*
 * BackChannel.h
 *
 *  Created on: Oct 3, 2014
 *      Author: gwilson
 */

#ifndef BACKCHANNEL_H_
#define BACKCHANNEL_H_

void BackChannel_Open();
void BackChannel_Write(unsigned char text[]);
void BackChannel_WriteLine(unsigned char text[]);
bool BackChannel_Connected();
#endif /* BACKCHANNEL_H_ */
