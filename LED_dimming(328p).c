/*
 * LED_dimming(328p).c
 *
 * Created: 14/08/2020 10:57:27
 * Author : mituki
 */ 

/*This simple program covers the basis of the concept of Pulse Width Modulation in micro controllers
Pulse Width Modulation is a technique of controlling analog circuits using micro controllers digital
output. it can be defined as a way of digitally encoding analog signal level using high resolution 
counters
PWM employs the concept that the current or voltage supplied to a component is directly proportional 
to the ratio of the on off time delays also known as the duty cycle
For an analog signal of 9V, using a PMW of 10%, 50%, 90% duty cycle the supplied voltage is as below:-
	duty cycle			Supplied Voltage
	0%						0
	10%						0.9
	50%						4.5V
	90%						8.1
	100%					9V
	
These voltages represent analog signals generated from a digital signal of 0V to 9V.

	IMPORTANT DEFINITIONS
Time period = on time + off time
Frequency = 1/time period
Duty cycle = (on time of the signal / total timer period of signal)
Amplitude = Vmax - Vmin
Vout = D x Vmax

	TYPES OF PWM
1.Fast PWM
2.Phase Correct PWM
****further reading on the above is highly advised

*/

/*
*****DESIGN TASK****
Dimming an LED continuously using PWM by varying the duty cycle
Start at duty cycle of 0% and increment it by 10% up to 100% with
a delay of 2000ms in between.
Using 8 bit timer0. 
*/
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	int duty_cycle;
	
	//duty_cycle = 255;
	//set PD6 as output 
	DDRD |= (1<<PIND6);
	
	//select Fast PWM mode for the timer0
	TCCR0A |= ((1<<WGM02)|(1<<WGM01) |(1<<WGM00));
	
	//select clear set value for compare match
	TCCR0A |= (1<<COM0A1);
	
	//select no pre_scaler i.e pre_scaler = 1
	//NOTE: this also starts the timer
	TCCR0B |= (1<<CS00);
	
    while (1) 
    {
		for(duty_cycle = 0; duty_cycle <=255; duty_cycle+=10){
				OCR0A = duty_cycle;
				_delay_ms(200);
		}
    }
}

