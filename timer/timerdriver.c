
#include "timerdriver.h"
#include "main.h"


volatile uint8_t globalPinStatus=0;
volatile int gOverflowCount=0,gTimer1Overflow=0,gTimer1OverflowMin=0,gOneMinuteOver=0;
volatile char gRequestCleared=0;
volatile char gInStateIndicate;
volatile float timeToBeAdded=0,oldTimeToBeAdded=0;

//volatile int gHourOverflowCount=HOUR_UPDATE_COUNT;

volatile char gHours=0,gMinutes=0,gSeconds=0;

extern volatile char gUpdateHourData;
extern signed volatile int gSetHours,gSetMinutes,gSetSeconds;
extern volatile char gDontUpdate,updateData;
extern volatile float savedDays,savedHours,savedMins;   //For hourmeter
extern volatile float EEMEM gDaysM,gHoursM,gMinutesM;   //saved hour meter

extern volatile int gIncPressed,gDecPressed,debounceNumber;

void timerInitialise(void)
{

	TCCR0 = (SET<<CS02) | (CLR<<CS01) | (SET<<CS00);
	TIMSK |= (SET<<TOIE0);
}

void timerStart(void)
{
	TCCR0 = (SET<<CS02) | (CLR<<CS01) | (SET<<CS00);
}

void timerStop(void)
{
	TCCR0 = (CLR<<CS02) | (CLR<<CS01) | (CLR<<CS00);
	TCNT0 = 0x00;
}


void timer1Initialise(void)
{
	TIMSK |= (SET<<TOIE1);
}

void timer1Start(void)
{
	TCCR1B = (SET<<CS02) | (CLR<<CS01) | (SET<<CS00);
}

void timer1Stop(void)
{
	TCCR1B = (CLR<<CS02) | (CLR<<CS01) | (CLR<<CS00);
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}


void timer2Initialise(void)
{
	ASSR = (SET<<AS2);
	TCCR2 = (SET<<CS02) | (CLR<<CS01) | (SET<<CS00);
	TIMSK |= (SET<<TOIE2);
}

void timerDisable(void)
{
	TCCR0 = 0x00;
	TIMSK = 0x00;
}

void timer2Stop(void)
{
	TCCR2 = (CLR<<CS02) | (CLR<<CS01) | (CLR<<CS00);
}

ISR(TIMER0_OVF_vect)
{
	gOverflowCount++;
	
	if((gIncPressed>5) || (gDecPressed>5))
	debounceNumber=DEBOUNCE_FAST;
	
	if(gOverflowCount > debounceNumber)
	{
		gDontUpdate=0;	
		gOverflowCount=0;
	}
}

ISR(TIMER2_OVF_vect)
{
	gSeconds++;
	
	if(gSeconds>59)
	{
			gOneMinuteOver=1;
			gSeconds=0;
			gMinutes++;
	}	
	
	if(gMinutes>99)
	{
		gMinutes=0;
		//gHours++;
	}
	
}


ISR(TIMER1_OVF_vect)
{
	gTimer1Overflow++;
	
    gTimer1OverflowMin++;
/*
	if(gTimer1OverflowMin > TIM1_UPDATE_ONE)
	{
		if(gUpdateHourData==1)
		timeToBeAdded = timeToBeAdded + 0.01667;
		gTimer1OverflowMin=0;
	}
*/		
	
	if(gTimer1Overflow > TIM1_UPDATE)
	{
		gTimer1Overflow=0;
		if(oldTimeToBeAdded != timeToBeAdded)
		{
			oldTimeToBeAdded = timeToBeAdded;
			
			updateData=1;
			
		}
	}	
	
	
	
}