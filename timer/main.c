/*
 * setpointapp.c
 *
 * Created: 5/15/2012 14:10:58
 *  Author: tnhindlekar
 */ 

#include "main.h"

#define BLINK_ENABLE

#if defined (BLINK_ENABLE)
#define BLINK_INTERVAL	15
#endif

int gTempVal;
int gHumidityVal;

char globalBuff[4];
volatile uint32_t EEMEM gSavedHours=0,gSavedMinutes=0,gSavedSeconds=0;  // saved time
volatile float EEMEM gDaysM=0,gHoursM=0,gMinutesM=0;   //saved hour meter
char setPressed=0,hrPressed=0;
	
extern volatile uint8_t globalPinStatus;
extern volatile float timeToBeAdded,oldTimeToBeAdded;

volatile char updateData=0;

volatile char gInStateIndicate=0;
volatile char gDontUpdate=0;

volatile float savedDays,savedHours,savedMins;   //For hourmeter
	
extern volatile char gHours,gMinutes,gSeconds;    //current clock
signed int gSetHours=0,gSetMinutes=0,gSetSeconds=0,gProgrammedMinutes=0;   // set current 
volatile char gUpdateHourData=0,gTimerIsON;

extern volatile int gTimer1Overflow,gTimer1OverflowMin,gOneMinuteOver;

volatile int gIncPressed=0,gDecPressed=0,debounceNumber=10;

int main(void)
{
	char setModeOn=0;
	uint8_t someBuffer[8];
	uint16_t updateCounter=0;
	char mybuff[5];
	char validPress = 1;
	int hello;
	int localCountForBlink=0;
	
#ifndef USE_SEVEN_SEGMENT
	LCDinit();
	uartInit(UART_BAUD);
#else
	initSevenSegment();
#endif 

initInputPort();
initOutputPort();

i2c_init();
timerInitialise();
timer1Initialise();

sei();
timer1Start();

_delay_ms(1000);

/*
EEPROM_erase();
_delay_ms(200);
writeWordToEEPROM(SAVED_SET_TIME_ADDRESS,0);
_delay_ms(200);
EEPROM_write(0,SAVED_HOUR_ADDRESS,"000.0");
_delay_ms(1000);
while(1);
*/

#ifdef	INTERNAL_EEPROM
gSetMinutes = 	eeprom_read_word(&gSavedMinutes);
#endif

#ifdef	EXTERNAL_EEPROM
gSetMinutes = 	readWordFromEEPROM();
#endif

gProgrammedMinutes = gSetMinutes;


   while(1)
    {
		globalPinStatus = INPUT_PORT & INPUT_MASK;
        
	
			_delay_ms(PRESS_DELAY);
			switch(globalPinStatus)
			{		
				
				case HR_PRESSED:	//SW5
					if(validPress)
					{
						validPress=0;						
						if(hrPressed==0)
						{
							readHourData();
							hrPressed=1;
							gInStateIndicate=3;
						}					
						else
							hrPressed=0;
						gDontUpdate=1;
						timerStart();
					}
					setModeOn=0;
					
				break;
		
				case SET_PRESSED:	//SW2
					gIncPressed=0;
					gDecPressed=0;
					debounceNumber=DEBOUNCE_SLOW;
					setModeOn=1;
					gInStateIndicate=1;
				break;			

				case RUN_PRESSED:	//SW1
					if(gUpdateHourData==0)
					{
						setModeOn=0;
						gInStateIndicate=2;
						updateMinutes();
						gProgrammedMinutes = gSetMinutes;
						gHours=0;
						gMinutes=0;
						gSeconds=0;
					
						TCNT1H = 0x00;
						TCNT1L = 0x00;
						gTimer1OverflowMin=0;
						gTimer1Overflow=0;	
					
						RELAY1_ON;
						RELAY2_ON;
						timer2Initialise();	
						//timer1Start();
						gTimerIsON=1;
						gUpdateHourData=1;
					}					
					
				break;		
				
				case SET_INC_PRESSED:	//SW2 and SW3
					if(validPress)
					{	
						validPress=0;
						gIncPressed++;							
						if(setModeOn)
						gSetMinutes++;
						if(gSetMinutes>99)
						gSetMinutes=99;
						gDontUpdate=1;
						timerStart();
					}					
					
				break;			

				case SET_DEC_PRESSED:	//SW2 and SW4
					if(validPress)
					{		
						validPress=0;
						gDecPressed++;		
						if(setModeOn)
						gSetMinutes--;
						if(gSetMinutes<0)
						gSetMinutes=0;
						gDontUpdate=1;
						timerStart();
					}					
				break;
				
				case NOTHING_PRESSED:
						if(gTimerIsON==1)
							gInStateIndicate=2;
						else
							gInStateIndicate=4;	
										
				break;
			}
		
				if(gDontUpdate==0)
				{
					validPress=1;
					timerStop();
				}
								
				if(gInStateIndicate==1)
				{
					#if defined (BLINK_ENABLE)
					if(localCountForBlink<BLINK_INTERVAL)
					{
						sprintf(mybuff,"%04d",gSetMinutes);
						printData(mybuff, DISPLAY1);
					}						
					else
						displayBlank();
						
					localCountForBlink++;
					
					if(localCountForBlink>(BLINK_INTERVAL*2))
						localCountForBlink=0;
					#else
						sprintf(mybuff,"%04d",gSetMinutes);
						printData(mybuff, DISPLAY1);
					#endif
						
				}
	
				if(gInStateIndicate==2)
				{
					
					sprintf(mybuff,"%02d.%02d",gProgrammedMinutes-gMinutes-1,59-gSeconds);
					//sprintf(mybuff,"%02d.%02d",gMinutes,gSeconds);
					printData(mybuff, DISPLAY1);
					if(gMinutes>(gProgrammedMinutes-1))
					{
						RELAY1_OFF;
						RELAY2_OFF;
						gInStateIndicate=0;
						timer2Stop();
						updateData=1;			//update hr data last time
						//timer1Stop();
						gUpdateHourData=0;
						gTimerIsON=0;
					}

				}

				if(gInStateIndicate==3)
				{
					//sprintf(mybuff,"%04d",(int)(ceil(savedHours)));
					sprintf(mybuff,"%05.1f",((savedHours)));
					printData(mybuff, DISPLAY1);
				}

				if(gInStateIndicate==4)
				{
					//sprintf(mybuff,"%04d",(int)(ceil(savedHours)));
					sprintf(mybuff,"%04d",gSetMinutes);
					printData(mybuff, DISPLAY1);
				}


				if(gOneMinuteOver==1)
				{
					if((gUpdateHourData==1)||(updateData==1))
					timeToBeAdded = timeToBeAdded + 0.01667;
					gOneMinuteOver=0;
				}

				
				if((updateData==1) && (timeToBeAdded>0.099))
				{
					#ifdef	INTERNAL_EEPROM
					while(!eeprom_is_ready());
					savedHours = 	eeprom_read_float(&gHoursM);
					savedHours = savedHours + timeToBeAdded;
					while(!eeprom_is_ready());
					eeprom_update_float(&gHoursM, savedHours);
					#endif

					#ifdef	EXTERNAL_EEPROM
					readHourData();
					savedHours = savedHours + timeToBeAdded;
					updateHourData();
					#endif
					timeToBeAdded=0;
					
					updateData=0;
				}
				
	/*
				if(gUpdateHourData && (gMinutes>0) && ((gMinutes%4)==0) && (gSeconds>58))
				{
					updateData=1;
				}
	*/
    }	

}


void printCurrentData(char type)
{
	uint8_t transmitBuffer[64];
	float localHumidity=0;


}


void printSetScreen(char type)
{
	
	uint8_t transmitBuffer[64];

		if(type == HUMIDITY)
		{
//			printFloatLED(gHumidityVal, DISPLAY1);
		}		
	
		if(type == TEMPERATURE)
		{
//			printFloatLED(gTempVal, DISPLAY1);
		}

}

void updateHourData(void)
{	  
#ifdef	INTERNAL_EEPROM
	while(!eeprom_is_ready());
	eeprom_update_float(&gHoursM, savedHours);
#endif

#ifdef	EXTERNAL_EEPROM
	char mystring[5];
	sprintf(mystring,"%05.1f",savedHours);
	EEPROM_write(0,SAVED_HOUR_ADDRESS,mystring);
#endif

}


void readHourData(void)
{
	
#ifdef	INTERNAL_EEPROM
	while(!eeprom_is_ready());
	savedHours = 	eeprom_read_float(&gHoursM);
#endif

#ifdef	EXTERNAL_EEPROM
	char mystring[5];
	EEPROM_read(0,SAVED_HOUR_ADDRESS,5,mystring);
	savedHours = stringToFloat(mystring);
#endif

}

void updateMinutes(void)
{
	
#ifdef	INTERNAL_EEPROM
	while(!eeprom_is_ready());
	eeprom_update_word(&gSavedMinutes, gSetMinutes);
#endif

#ifdef	EXTERNAL_EEPROM
	writeWordToEEPROM(SAVED_SET_TIME_ADDRESS, gSetMinutes);
#endif

}

float stringToFloat(unsigned char* string)
{
	float data=0;
	int divisor,dotPosition=0;
	for(divisor=1000;divisor!=0;)
	{
		if(*string!='.')
		{
				data += (*(string) - 0x30)*divisor;
				divisor/=10;
		}		
		else
			dotPosition = divisor*10;
			
			string++;		
	}
	
	return (data/dotPosition);
}

void writeWordToEEPROM(unsigned int address, int data)
{
	char dataWord[3];
	
	sprintf(dataWord,"%02d",data);
	
	EEPROM_write(0,address,dataWord);
}

int readWordFromEEPROM(unsigned int address)
{
	char dataWord[2];
	int data;
	EEPROM_read(0,address,2,dataWord);
	
	data = (dataWord[0]-0x30)*10 + (dataWord[1]-0x30);
	
	return data;
}