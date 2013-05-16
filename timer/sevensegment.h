/*
 * _7segment.h
 *
 * Created: 5/17/2012 12:01:51
 *  Author: tnhindlekar
 */ 


#ifndef SEVENSEGMENT_H_
#define SEVENSEGMENT_H_

#include "main.h"

#define USE_SEVEN_SEGMENT	1

#define SEVEN_SEGMENT_DATA_PORT	PORTD
#define SEVEN_SEGMENT_DATA_DIR	DDRD
#define SEVEN_SEGMENT_CTRL_PORT	PORTC
#define SEVEN_SEGMENT_CTRL_DIR	DDRC
#define SEVEN_SEGMENT_SEL_PORT	PORTB
#define SEVEN_SEGMENT_SEL_DIR	DDRB

#define SEG1_PIN		0x03
#define SEG2_PIN		0x02
#define SEG3_PIN		0x01
#define SEG4_PIN		0x00
#define DISPLAY_SEL_PIN	5

#define SEL_DISPLAY_MASK	(SET<<DISPLAY_SEL_PIN)

#define SEL_SEG1		SEVEN_SEGMENT_CTRL_PORT&=(0xF3);SEVEN_SEGMENT_CTRL_PORT|=(SEG1_PIN<<2)
#define SEL_SEG2		SEVEN_SEGMENT_CTRL_PORT&=(0xF3);SEVEN_SEGMENT_CTRL_PORT|=(SEG2_PIN<<2)
#define SEL_SEG3		SEVEN_SEGMENT_CTRL_PORT&=(0xF3);SEVEN_SEGMENT_CTRL_PORT|=(SEG3_PIN<<2)
#define SEL_SEG4		SEVEN_SEGMENT_CTRL_PORT&=(0xF3);SEVEN_SEGMENT_CTRL_PORT|=(SEG4_PIN<<2)
#define SEL_SEG_N		SEVEN_SEGMENT_CTRL_PORT&=(0xF3);
#define SEL_DISPLAY1	SEVEN_SEGMENT_SEL_PORT&=~SEL_DISPLAY_MASK
#define SEL_DISPLAY2	SEVEN_SEGMENT_SEL_PORT|=SEL_DISPLAY_MASK

#define DISPLAY1	1
#define DISPLAY2	2

void initSevenSegment(void);
void printData(char* string, char display);
void displayBlank(void);
uint8_t returnHex(char character);
//void printFloatLED(float number, char display);

#endif /* SEVENSEGMENT_H_ */