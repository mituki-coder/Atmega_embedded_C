/*
 * DC_motor_control(Phase_Correct_PWM)_328p.c
 *
 * Created: 19/08/2020 19:38:34
 * Author : mituki
 */ 
/*
This program executes phase correct PWM in its simplest form to run a DC motor in variable speeds.
To completely understand phase correct PWM.refer to notes on the same and Atmega328P data sheet.
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Replace with your application code */
	int duty_cycle;
	
	DDRD |= (1<<PIND6);
	
	//select Phase Correct PWM mode for the timer0
	TCCR0A |= ((1<<WGM02) |(1<<WGM00));

	//select clear set value for compare match	
	TCCR0A |= (1<<COM0A1);
	
	//select no pre_scaler i.e pre_scaler = 1
	//NOTE: this also starts the timer
	TCCR0B |= (1<<CS00);
    while (1) 
    {
		for(duty_cycle = 0; duty_cycle <=255; duty_cycle+=25){
			OCR0A = duty_cycle;
			_delay_ms(200);
		}
    }
}

