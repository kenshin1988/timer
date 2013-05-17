/*
 * main.h
 *
 * Created: 5/15/2012 14:13:24
 *  Author: tnhindlekar
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include <stdio.h>
#include <string.h>

#include <stddef.h>

#include "EEPROM_routines.h"
#include "inputs.h"
#include "sevensegment.h"


#define SET	1
#define CLR	0
#define STRING_COUNT	5
//State machine states

#define DORMANT_STATE	0
#define SET_STATE		1
#define INC_STATE		2
#define DEC_STATE		3

#define TEMPERATURE		0
#define HUMIDITY		1

#define UPDATE_COUNT    255

#define PRESS_DELAY		1

#define FACTOR_TEMP			100
#define FACTOR_HUMIDITY     100

#define DEBOUNCE_FAST	3
#define DEBOUNCE_SLOW	10

//#define INTERNAL_EEPROM
#define EXTERNAL_EEPROM


#ifdef EXTERNAL_EEPROM

#define SAVED_SET_TIME_ADDRESS	0
#define SAVED_HOUR_ADDRESS		16

#endif


void updateHourData(void);
void readHourData(void);
void updateMinutes(void);
float stringToFloat(unsigned char* string);


#endif /* MAIN_H_ */