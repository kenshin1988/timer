/*
 * items.h
 *
 * Created: 5/15/2012 10:31:47 PM
 *  Author: Tushar
 */ 


#ifndef ITEMS_H_
#define ITEMS_H_

#include "main.h"

#define INPUT_DDR			DDRB
#define INPUT_PULLUP		PORTB
#define INPUT_PORT			PINB

#define OUTPUT_DDR			DDRC
#define OUTPUT_PORT			PORTC

#define RELAY1_PIN		0
#define RELAY2_PIN		1

#define RELAY1_OFF		OUTPUT_PORT&=~(1<<RELAY1_PIN)			
#define RELAY2_OFF		OUTPUT_PORT&=~(1<<RELAY2_PIN)

#define RELAY1_ON		OUTPUT_PORT|=(1<<RELAY1_PIN)
#define RELAY2_ON		OUTPUT_PORT|=(1<<RELAY2_PIN)

#define RUN_PIN			0
#define SET_PIN			1
#define INC_PIN			2
#define DEC_PIN			3
#define HR_PIN			3

#define INPUT_MASK		0x1F //(0x01<<HMD_PIN)|(0x01<<TEMP_PIN)|(0x01<<INC_PIN)|(0x01<<DEC_PIN)

/*
#define RUN_PIN_VAL		(INPUT_PORT&(1<<RUN_PIN))
#define HR_PIN_VAL		(INPUT_PORT&(1<<HR_PIN))
#define SET_PIN_VAL		(INPUT_PORT&(1<<SET_PIN))	//>>TEMP_PIN
#define INC_PIN_VAL		(INPUT_PORT&(1<<INC_PIN))	//>>INC_PIN
#define DEC_PIN_VAL		(INPUT_PORT&(1<<DEC_PIN))	//>>DEC_PIN
*/

#define RUN_PRESSED		0x1E	
#define SET_PRESSED		0x1D
#define INC_PRESSED		0x1B
#define DEC_PRESSED		0x17
#define HR_PRESSED		0x0F	
#define SET_INC_PRESSED	0x19
#define SET_DEC_PRESSED	0x15
#define NOTHING_PRESSED	INPUT_MASK	
		

void initInputPort(void);
void initOutputPort(void);

#endif /* ITEMS_H_ */