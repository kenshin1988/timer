void timerInitialise(void);
void timer1Initialise(void);
void timer1Start(void);
void timer1Stop(void);
void timerDisable(void);
void timer2Initialise(void);
void timerStart(void);
void timerStop(void);
void timer2Stop(void);

#include "main.h"

//#if defined(__AVR_ATmega128__)

#define ENABLE_TIMER1		TCCR1B=0x05;TIMSK=0x04;TCNT1=0x00
#define DISABLE_TIMER1		TCCR1B=0x00;TIMSK=0x00;TCNT1=0x00

#define OVERFLOW_COUNT 10
#define HOUR_UPDATE_COUNT	3333UL
#define TIM1_UPDATE		36
#define TIM1_UPDATE_ONE	7
//#elif defined(__AVR_AT90CAN128__)

//#define ENABLE_TIMER1		TCCR1B=0x05;TIMSK1=0x01;TCNT1=0x00
//#define DISABLE_TIMER1		TCCR1B=0x00;TIMSK1=0x00;TCNT1=0x00

//#endif
