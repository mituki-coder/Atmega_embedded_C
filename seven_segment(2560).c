/*
 * seven_segment(2560).c
 *
 * Created: 03/08/2020 11:44:37
 * Author : mituki
 */ 

#include <avr/io.h>
#include <util/delay.h>

char numbers[] ={0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111,0b01101111};
char decimal_numbers[] ={0,1,2,3,4,5,6,7,8,9};

int main(void)
{
	//set PORTF into output mode
	DDRF  = 0xff;
	
    /* Replace with your application code */
    while (1) 
    {
		for(int i =0;i<10;i++){
			PORTF = decimal_numbers[i];
			_delay_ms(10000);
		}
    }
}

