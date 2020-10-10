/*
 * UART_basics(328P).c
 *
 * Created: 26/08/2020 19:19:27
 * Author : mituki
 */ 

/*
UART-Universal Asynchronous Receiver Transmitter
Asynchronous in such that the master and receiver do not have a synchronized clock system.Unlike SPI and I2C which 
are implemented in software form, UART is a standalone chip or hardware configuration or a physical unit in a micro
controller or a circuit used to transmit and receive data serially.
UART uses two "wires" to achieve communication:-
RX - receiver
TX - transmitter

UART converts parallel data from a CPU or other peripheral component to a serial form and adds more bits to convert it 
to a data packet to send to the receiver. These additions are discussed later. The receiver then converts the data back 
to parallel form and removes the additions. 

**********Components of an UART data packet****************
--1 Start bit
--5 to 9 data bits(the forms the data frame)  
--0 to 9 parity bits
--1 to 2 Stop bits

***Start bit*****
To start transmission ,UART pulls transmission line from HIGH to LOW for one clock cycle ..This,for the receiving UART signifies 
the start of data transmission

***Data frame*****
Actual data being transmitted 
5 - 8 bits if parity bit is used
9 bits if no parity bit is used

***Parity bit****
Describes the oddness or evenness of data being transmitted. This is used to check if the data has changed during transmission.
...Read more if necessary...

***Stop bit****
UART drives the transmission line from LOW to HIGH voltage for at leasts TWO CLOCK CYCLES
*/

/*
*************************The BaudRate*****************
Specifies how fast data is sent over a serial line.It is expressed in terms of bits-per-second(bps). For UART communication, the 
two communicating devices have to be set to same baud rate to ensure that there are no errors during "decoding" of the data on the 
receiving device.Common standard baud rates include:- 9600,1200,2400,4800,19200,38400,57600,115200. The value stored in the baud 
registers is not the baud rate but rather a value calculated depending on the mode of operation of the UART.Formulae for the same 
clearly defined in the data sheet. 
*/
/*
*************ATMEGA 328P SPECIFIC APPROACH************
The register description is clear in the data sheet.
*/

/*
Below is a simple program for sending "HELLO" via UART asynchronous mode to PUTTY .
*/
#include <avr/io.h>
#include <util/delay.h>

#define FOSC 16000000 // Clock Speed is 16000000Hz
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1
void UART_init(unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bits */
	UCSR0C = (1<<USBS0)| (1<<UCSZ01)| (1<<UCSZ00);          //(3<<UCSZ00);
}


void USART_Transmit( unsigned char data )
{
	/*note data transmitted can be in both binary rep for ASCII value or in char form 
	i.e '4' or 0b00110100 reference to ASCII table is advised for the binary format*/
	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0))) 
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
int main(void)
{
    /* Replace with your application code */
	UART_init(MYUBRR);
	int i;
	char my_data[5] = "HELLO";
	
    while (1) 
    {			
	for(i = 0 ; i < 5; i++){
		USART_Transmit(my_data[i]);
		}
		USART_Transmit('\n'); //new line feed
		USART_Transmit('\r'); //carriage return
    }
}

