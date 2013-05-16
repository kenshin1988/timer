/*
 * inputs.c
 *
 * Created: 5/15/2012 10:31:37 PM
 *  Author: Tushar
 */ 

#include "inputs.h"

void initInputPort(void)
{
	INPUT_DDR |= 0x00;
	INPUT_PULLUP |= 0x1F;//INPUT_MASK;
}

void initOutputPort(void)
{
	OUTPUT_DDR |= (SET<<RELAY1_PIN)|(SET<<RELAY2_PIN);
	//OUTPUT_PORT |= (SET<<RELAY1_PIN)|(SET<<RELAY2_PIN);
	RELAY1_OFF;
	RELAY2_OFF;
	DDRB |= 0x20;
}