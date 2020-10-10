/*
 * keypad_and_LCD(2560).c
 *
 * Created: 27/07/2020 13:52:58
 * Author : mituki
 */ 

#include <avr/io.h>
#include <util/delay.h>

//constants
#define data_port PORTD
#define control_port PORTE

#define data_ddr DDRD
#define control_ddr DDRE

#define rs_pin PE0
#define rw_pin PE1
#define en_pin PE2

void lcd_init();
void send_command(unsigned char command);
void send_character(unsigned char character);

void column_checker();
void column_checker_1();
void column_checker_2();
void column_checker_3();

void key_disp();

int column,row;
char key;

//rows are assigned bits 0 to 3 PORTA and columns bits 0 to 3 PORTB
//declaring variables
unsigned char keypad[4][4] = {
 {'7','8','9','/'},
 {'4','5','6','x'},
 {'1','2','3','-'},
 {'*','0','#','+'},	
};







int main(void)
{
    /* Replace with your application code */
	control_ddr = 0xff;
	data_ddr = 0xff;
	
	DDRF  |= ((1<<PF0) | (1<<PF1) |(1<<PF2) |(1<<PF3)| (1<<PF4) | (1<<PF5) |(1<<PF6) |(1<<PF7));
	
    lcd_init();
	//rows config outputs
	DDRA  |= ((1<<PA0) | (1<<PA1) |(1<<PA2) |(1<<PA3));
	
	//columns config inputs
	DDRB  |= ((0<<PB0) | (0<<PB1) |(0<<PB2) |(0<<PB3));
	
	//columns config pull up resistors
	PORTB |= ((1<<PB0) | (1<<PB1) |(1<<PB2) |(1<<PB3));
	
	
    while (1) 
    {
		
	PORTA &= ~(1<<PA0);
	column_checker();
	PORTA |=(1<<PA0);
	
	PORTA &= ~(1<<PA1);
	column_checker_1();
	PORTA |=(1<<PA1);
	
	PORTA &= ~(1<<PA2);
	column_checker_2();
	PORTA |=(1<<PA2);

	PORTA &= ~(1<<PA3);
	column_checker_3();
	PORTA |=(1<<PA3);

    }
}


void column_checker(){
	
	//check if any of the 4 columns is pulled low
	if ((PINB & (1<<PB0)) == 0){
		//col_1_disp();
		row = 0; 
		column = 0;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB1)) == 0){
		row = 0;
		column = 1;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB2)) == 0){
		row = 0;
		column = 2;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB3)) == 0){
		row = 0;
		column = 3;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}	
}



void column_checker_1(){
	//check if any of the 4 columns is pulled low
	if ((PINB & (1<<PB0)) == 0){
		row = 1;
		column = 0;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB1)) == 0){
		row = 1;
		column = 1;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB2)) == 0){
		row = 1;
		column = 2;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB3)) == 0){
		row = 1;
		column = 3;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}	
}


void column_checker_2(){
	//check if any of the 4 columns is pulled low
	if ((PINB & (1<<PB0)) == 0){
		row = 2;
		column = 0;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB1)) == 0){
		row = 2;
		column = 1;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB2)) == 0){
		row = 2;
		column = 2;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB3)) == 0){
		row = 2;
		column = 3;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
}


void column_checker_3(){
	//check if any of the 4 columns is pulled low
	if ((PINB & (1<<PB0)) == 0){
		row = 3;
		column = 0;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB1)) == 0){
		row = 3;
		column = 1;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB2)) == 0){
		row = 3;
		column = 2;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
	
	if ((PINB & (1<<PB3)) == 0){
		row = 3;
		column = 3;
		key = keypad[row][column];
		key_disp();
	}
	else{
		//do nothing
	}
}




/*****************LCD functions***********************/

void lcd_init(){

	_delay_ms(40);
	send_command(0b00001100);
	_delay_ms(4.1);
	
	send_command(0b00001100);
	_delay_us(100);
	
	send_command(0b00001100);
	_delay_us(100);
	
	send_command(00111000);
	_delay_us(40);
	
	send_command(00001000);
	_delay_us(40);
	
	send_command(0b00000001);
	_delay_ms(1.64);
	
	send_command(0b00000110);
	_delay_us(40);
	
	send_command(0b00001100);
	_delay_us(40);	
}

void send_command(unsigned char command){
	data_port  = command;
	control_port &=~(1<<rs_pin);
	control_port &=~(1<<rw_pin);
	
	//invoke action
	control_port |=(1<<en_pin);
	_delay_ms(10);
	control_port &=~(1<<en_pin);
	_delay_ms(10);
	
}

void send_character(unsigned char character){
	data_port = character;
	control_port |=(1<<rs_pin);
	control_port &=~(1<<rw_pin);
	
	//invoke action
	control_port |=(1<<en_pin);
	_delay_ms(10);
	control_port &=~(1<<en_pin);
	_delay_ms(10);
}

void key_disp(){
	send_character(key);
	_delay_ms(150);
	send_command(0x01);
	_delay_ms(150);
}
