
void uartInit(int baudrate);
void printString(char *str);

#define SSOF 		0xF5
#define EEOF 		0xF6
#define ACK 		0xF0
#define NAK 		0xE1

#define UART_BAUD 	19200

#define UDRE_EN 	UCSRB |= (SET << UDRIE)
#define UDRE_DS 	UCSRB &= ~(SET << UDRIE)

