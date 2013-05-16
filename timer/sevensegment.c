/*
 * _7segment.c
 *
 * Created: 5/17/2012 12:01:39
 *  Author: tnhindlekar
 */ 

#include "sevensegment.h"

char gPattern[64] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0E,0x7F,0x10,0x09,0x0B,0x7B,0x61,0x47,0x2B,0x13,0x0C,0x18,0x2F,0x12,0x07,0x41,0x11,0x24,0xFF};

void initSevenSegment(void)
{
	SEVEN_SEGMENT_DATA_DIR = 0xFF;
	SEVEN_SEGMENT_DATA_PORT = 0xFF;
	SEVEN_SEGMENT_CTRL_DIR = 0x0C;//(SEG4_PIN<<2);
	SEVEN_SEGMENT_CTRL_PORT = 0x00;//~(SEG4_PIN<<2);
	SEVEN_SEGMENT_SEL_DIR |= (SET<<DISPLAY_SEL_PIN);
	SEVEN_SEGMENT_SEL_PORT |= (SET<<DISPLAY_SEL_PIN);
}

//12.34

void printData(char* string, char display)
{
	uint8_t localArray[8],length,count,innerCount;
	char string2[5];

	switch(display)
	{
		case DISPLAY1:
			SEL_DISPLAY1;
			break;
		case DISPLAY2:
			SEL_DISPLAY2;
			break;
	}	
	/*
	string2[5]=0;
	string2[4]=string[0];
	string2[3]=string[1];
	string2[2]=string[2];
	string2[1]=string[3];
	string2[0]=string[4];
*/	
		length = strlen(string);
		
		for(count=0;count<length;count++)
		{
			localArray[count] = gPattern[returnHex(string[count])];
		}
	
		for(count=0;count<length;count++)
		{
			if(localArray[count]==0x7F)
			{
				localArray[count-1]&=0x7F;
				for(innerCount=count;innerCount<length;innerCount++)
					localArray[innerCount]=localArray[innerCount+1];

			}			
			else
				localArray[count-1]|=0x80;
		}

		PORTB|=0x20;
		_delay_ms(2);
		SEVEN_SEGMENT_DATA_PORT = localArray[0];
		SEL_SEG4;
		_delay_ms(1);
		PORTB&=0xDF;
		_delay_ms(1);
		
		PORTB|=0x20;
		_delay_ms(2);
		SEVEN_SEGMENT_DATA_PORT = localArray[1];
		SEL_SEG3;
		_delay_ms(1);
		PORTB&=0xDF;
		_delay_ms(1);
		
		PORTB|=0x20;
		_delay_ms(2);
		SEVEN_SEGMENT_DATA_PORT = localArray[2];
		SEL_SEG2;
		_delay_ms(1);
		PORTB&=0xDF;
		_delay_ms(1);
		
		PORTB|=0x20;
		_delay_ms(2);
		SEVEN_SEGMENT_DATA_PORT = localArray[3]|0x80;
		SEL_SEG1;
		_delay_ms(1);
		PORTB&=0xDF;
		_delay_ms(1);
		PORTB|=0x20;
		
	//	_delay_ms(1);
}


void displayBlank(void)
{
			PORTB|=0x20;
			_delay_ms(2);
			SEVEN_SEGMENT_DATA_PORT = 0xFF;
			SEL_SEG4;
			_delay_ms(1);
			PORTB&=0xDF;
			_delay_ms(1);
			
			PORTB|=0x20;
			_delay_ms(2);
			SEVEN_SEGMENT_DATA_PORT = 0xFF;
			SEL_SEG3;
			_delay_ms(1);
			PORTB&=0xDF;
			_delay_ms(1);
			
			PORTB|=0x20;
			_delay_ms(2);
			SEVEN_SEGMENT_DATA_PORT = 0xFF;
			SEL_SEG2;
			_delay_ms(1);
			PORTB&=0xDF;
			_delay_ms(1);
			
			PORTB|=0x20;
			_delay_ms(2);
			SEVEN_SEGMENT_DATA_PORT = 0xFF;
			SEL_SEG1;
			_delay_ms(1);
			PORTB&=0xDF;
			_delay_ms(1);
			PORTB|=0x20;
}

/*	
void printFloatLED(float number, char display)
{
	uint8_t localArray[8],length,count,innerCount;
	char string[8];
	
	if(number<10)
		sprintf(string,"%0.3f",(double)number);
	else
	if(number<100)
		sprintf(string,"%0.2f",(double)number);
	else
	if(number<1000)
		sprintf(string,"%0.1f",(double)number);
	else
		sprintf(string,"0f");
	
	switch(display)
	{
		case DISPLAY1:
		SEL_DISPLAY1;
		break;
		case DISPLAY2:
		SEL_DISPLAY2;
		break;
	}
	
	length = strlen(string);
	
	for(count=0;count<length;count++)
	{
		localArray[count] = gPattern[returnHex(string[count])];
	}
	
	for(count=0;count<length;count++)
	{
		if(localArray[count]==0x7F)
		{
			localArray[count-1]&=0x7F;
			for(innerCount=count;innerCount<length;innerCount++)
			localArray[innerCount]=localArray[innerCount+1];
		}
		else
		localArray[count-1]|=0x80;
	}
	
	_delay_ms(1);
	SEL_SEG1;
	SEVEN_SEGMENT_DATA_PORT = localArray[0];
	_delay_ms(1);
	SEL_SEG2;
	SEVEN_SEGMENT_DATA_PORT = localArray[1];
	_delay_ms(1);
	SEL_SEG3;
	SEVEN_SEGMENT_DATA_PORT = localArray[2];
	_delay_ms(1);
	SEL_SEG4;
	SEVEN_SEGMENT_DATA_PORT = localArray[3];
	_delay_ms(1);
	SEL_SEG_N;
	_delay_ms(1);

}
*/
/*
void printNumber(char* string, char display)
{
	switch(display)
	{
		case DISPLAY1:
		SEL_DISPLAY1;
		break;
		case DISPLAY2:
		SEL_DISPLAY2;
		break;
	}
	_delay_ms(1);
	SEL_SEG1;
	SEVEN_SEGMENT_DATA_PORT = gPattern[returnHex(string[0])];
	_delay_ms(1);
	SEL_SEG2;
	SEVEN_SEGMENT_DATA_PORT = gPattern[returnHex(string[1])];
	_delay_ms(1);
	SEL_SEG3;
	SEVEN_SEGMENT_DATA_PORT = gPattern[returnHex(string[2])];
	_delay_ms(1);
	SEL_SEG4;
	SEVEN_SEGMENT_DATA_PORT = gPattern[returnHex(string[3])];
	_delay_ms(1);
	SEL_SEG_N;
	_delay_ms(1);

}	
*/

uint8_t returnHex(char character)
{
	switch(character)
	{
		case '0':
		return 0x00;
		break;
		
		case '1':
		return 0x01;
		break;

		case '2':
		return 0x02;
		break;

		case '3':
		return 0x03;
		break;

		case '4':
		return 0x04;
		break;

		case '5':
		return 0x05;
		break;

		case '6':
		return 0x06;
		break;

		case '7':
		return 0x07;
		break;

		case '8':
		return 0x08;
		break;

		case '9':
		return 0x09;
		break;

		case 'a':
		case 'A':
		return 0x0A;
		break;

		case 'b':
		case 'B':
		return 0x0B;
		break;

		case 'c':
		case 'C':
		return 0x0C;
		break;

		case 'd':
		case 'D':
		return 0x0D;
		break;

		case 'e':
		case 'E':
		return 0x0E;
		break;
		
		case 'f':
		case 'F':
		return 0x0F;
		break;

		case '.':
		return 0x10;
		break;

		case 'g':
		case 'G':
		return 0x11;
		break;
		
		case 'H':
		return 0x12;
		break;
		
		case 'h':
		return 0x13;
		break;

		case 'i':
		case 'I':
		return 0x14;
		break;

		case 'j':
		case 'J':
		return 0x15;
		break;

		case 'l':
		case 'L':
		return 0x16;
		break;

		case 'n':
		case 'N':
		return 0x17;
		break;

		case 'o':
		case 'O':
		return 0x18;
		break;

		case 'p':
		case 'P':
		return 0x19;
		break;

		case 'q':
		case 'Q':
		return 0x1A;
		break;

		case 'r':
		case 'R':
		return 0x1B;
		break;

		case 's':
		case 'S':
		return 0x1C;
		break;

		case 't':
		case 'T':
		return 0x1D;
		break;

		case 'u':
		case 'U':
		return 0x1E;
		break;

		case 'y':
		case 'Y':
		return 0x1F;
		break;

		case 'z':
		case 'Z':
		return 0x20;
		break;

		case ' ':
		return 0x21;
		break;

		default:
		break;

	}

	return 0;

}


