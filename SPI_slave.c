/*
 * SPI_Slave(328p).c
 *
 * Created: 23/08/2020 10:40:22
 * Author : mituki
 */ 

#include <avr/io.h>
#include <util/delay.h>


#define F_CPU 16000000UL

uint8_t data;
int duty_cycle;

void slave_init(){
	
	//set MISO output and the rest inputs
	PORTB |= (1<<PORTB4);
	
	//enable SPI
	SPCR |= (1<<SPE);	
}

void slave_read(){
	//wait for transfer to complete
	while(!(SPSR & (1<<SPIF)));
	
	//read the data register
	data = SPDR;
}


int main(void)
{
    /* Replace with your application code */
	
	//configure LED pin output
	DDRD |= (1<<PIND6);
	
	//select Fast PWM mode for the timer0
	TCCR0A |= ((1<<WGM02)|(1<<WGM01) |(1<<WGM00));
		
	//select clear set value for compare match
	TCCR0A |= (1<<COM0A1);
		
	//select no pre_scaler i.e pre_scaler = 1
	//NOTE: this also starts the timer
	TCCR0B |= (1<<CS00);
	
	slave_init();
	
    while (1) 
    {
		slave_read();
		duty_cycle = data;
		OCR0A = duty_cycle;
		_delay_ms(2000);
		
    }
}

