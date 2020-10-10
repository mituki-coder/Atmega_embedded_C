/*
 * Timers_1(328p).c
 *
 * Created: 13/08/2020 10:39:59
 * Author : mituki
 */ 

/*
Timers/counters are hardware components that are used to count time or external events
and can also be used to generate precise delays and generate PWM(Pulse Width Modulation) signals.

Atmega328p has 3 timers/counters:-
	timer0 (8bit)
	timer1 (16bit)
	timer2 (8bit)
	
To achieve desired time delays or timing tasks, the timers require a clock signal source which can 
be the micro controller 16Mhz clock signal. However, for better precision the signal can scaled down
 using pre_scalers which are available in the atmega328p:-
	1,8,64,256,1024
	
Modes of operation for the timer are:-
	Normal mode
	CTC (Clear Timer on Compare Match)
	Phase Correct PWM
	Fast PWM
	***Further details on the same from the datasheeet***
Bottom- 0x00 for 8bit and 0x0000 for 16bit
Max- 0xFF for 8bit and 0xFFFF for 16bit
Top-highest value in the count sequence which can be fixed to max or stored in OCR0A register

There are a couple of registers for handling operations of timers and are clearly described in the data 
sheet		
*/

/*
****DESIGN TASK****
toggle an LED at 0.5Hz i.e on/off delay of 2000ms or 2s
***Solution***
f = 0.5Hz, t = 1/f ,t = 2s(2000ms)
F = atmega328p clock frequency = 16Mhz
         ******
Using 16bit timer1
timer max = 65535

for 16Mhz,, timer resolution is 1/16Mhz = 0.0040959375 == 4ms
i.e if we use a frequency of 16Mhz,,the timer is only able to run from bottom to max in roughly 4ms and 
then is cleared,,, this is too fast since we need the counter to have a delay of 2000ms before completing 
the count from bottom to max.

Pre_scalers now come into play
The formula for establishing the pre_scaler is:-
Target Timer Count =  ((input frequency /pre_scaler)/Target Frequency -1)
Input Frequency = 16Mh
Target Frequency = 0.5Hz

pre_sclaer value				Target Timer Count
1									31999999
8									 3999999
64									  499999
256									  124999
1024								   31249

Note: the target timer count is the number of counts the timer is required to make at the given frequency
to achieve the desired frequency or "delay"
This value has to be below our counter max which is 65535
From the table the best pre_scaler is seen to be 1024 since it gives a value below timer max
*/

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	//set led pin as output
	DDRD |=(1<<PORTD0);
	
	//configure time pre_scaler to 1024 (for more details refer to data sheet)
	TCCR1B |= ((1<<CS12) | (1<<CS10));
	
    while (1) 
    {
		//read timer value and toggle led
		if(TCNT1 >=31249){
			PORTD ^=(1<<PORTD0);
			
			//clear timer
			TCNT1 = 0;
		}
    }
}

