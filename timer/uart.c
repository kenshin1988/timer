#include "main.h"

volatile uint8_t gRx0Start,gRx0Ctr,gRx0Over,gGlobalRx0[32];			//Global Variables for UART0
volatile uint8_t gTx0Buffer[64],gTx0Count=0;

void uartInit(int baudrate)
{
	
	int uartUBBR=0;
	
	//Only transmit enable
	UCSRB = (CLR<<RXCIE) | (CLR<<TXCIE) | (CLR<<UDRIE) | (CLR<<RXEN) | (SET<<TXEN) | (CLR<<UCSZ2) | (CLR<<RXB8) | (CLR<<TXB8);	
	//8-bit mode, 1-top bit, no parity											
	UCSRC = (CLR<<URSEL) | (CLR<<UMSEL) | (CLR<<UPM1) | (CLR<<UPM0) | (CLR<<USBS) | (SET<<UCSZ1) | (SET<<UCSZ0) | (CLR<<UCPOL);												
	//Setting the baudrate
	
	uartUBBR = ((F_CPU)/((baudrate)*16l)-1);
	
	UBRRL=uartUBBR;
	UBRRH=((int)uartUBBR)>>7;
}


ISR(USART_RXC_vect)					//Receive Interrupt Handler for UART0
{ 
 	uint8_t tempRx0;
  	
	tempRx0=UDR;	
   
	if(tempRx0==SSOF)
	{
		gRx0Start=1;
		gRx0Ctr=0;
	}

	if(gRx0Start)
    {
      	gGlobalRx0[gRx0Ctr++]=tempRx0;
    }
   
	if(tempRx0==EEOF)
    {
      	gRx0Over=1;
   	  	gRx0Start=0;
	  	gRx0Ctr=0;
    }
}


ISR(USART_UDRE_vect)
{
	if(gTx0Buffer[gTx0Count] != NULL) 
		UDR = gTx0Buffer[gTx0Count++];
	else
	{
		gTx0Count=0;
		UDRE_DS;
	}	
}

void printString(char *str)
{
	strcpy((char *)gTx0Buffer, str);
	UDRE_EN;
}	

void printThis( char* format, ...) 
{
	va_list args;
	va_start( args, format );
	vsprintf(gTx0Buffer,format,args); 
	va_end( args );
	UDRE_EN;
}