/*
 * I2C_LCD.c
 *
 * Created: 12/09/2020 08:49:09
 * Author : mituki
 * Title  : PCF8574 20*4 LCD and DS3231
 */ 

/*
I2c to 8 bit chip is PCF8574
operates for a frequency range of 100khz to 400khz
addressing is-:0 1 0 0 A2 A1 A0 R/W for my case 0 1 0 0 0 0 0 0 
since ill only be executing write only
R/W is accessed as below:-
	R/W		Logic
	R         1
	W         0
*/

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000                                             //micro controller clock frequency 1MHz

//I2C command set
#define PCF8574_address_write     0b01000000


//LCD command set
#define LCD_4bit             0x02                                   //4bit set
#define LCD_5_7              0x28                                   //5 by 7 setting
#define LCD_disp_off_cu_on   0x0E                                   //display off cursor on
#define LCD_clear            0x01                                   //clear display
#define LCD_row_1            0x80                                   //move cursor to beginning
#define LCD_row_2            0xC0
#define LCD_row_3            0x94
#define LCD_row_4            0xD4


//DS3231 command set
#define DS3231_address_write  0xD0                                   //DS3231 address is 0x68 and setting R/W bit to write results to 0xD0
#define DS3231_address_read  0xD1									//DS3231 address is 0x68 and setting R/W bit to read results to 0xD1
#define DS3231_sec_reg       0x00
#define DS3231_min_reg       0x01
#define DS3231_hour_reg      0x02
#define DS3231_day_reg       0x03
#define DS3231_date_reg      0x04
#define DS3231_month_reg     0x05	
#define DS3231_year_reg      0x06

uint8_t BCD_value,DEC_value;								


//time and date variables
int second;
int minutes;
int hours;
int days;
int dates;
int months;
int years;


char str_buffer [2] ;
/*****************************************************************I2C functions******************************************************************************/
void I2C_init(){
	//initialize I2C at 400khz
	TWSR  = 0x00;  //sets pre-scaler to 1
	TWBR  = 0x0C;
}

void I2C_start(){
	//sends start condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
}

void I2C_stop(){
	//sends stop condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

void I2C_write(uint8_t data){
	
	I2C_start();
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	I2C_stop();
}


/********************************************************PCF8574     functions******************************************************************************/

void I2C_write_PCF8574(uint8_t address,uint8_t data){
	I2C_start();
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	I2C_stop();
	
	I2C_start();
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	
	I2C_stop();
}


/********************************************************LCD I2C functions******************************************************************************/

lcd_i2c_init(){
	//initialize the LCD using the I2C interface
	
	//delay for a bit before initializing the LCD
	_delay_ms(15);    
	
	//send command for 4 bit control
	LCD_command(LCD_4bit);
	LCD_command(LCD_5_7);
	LCD_command(LCD_disp_off_cu_on);
	LCD_command(LCD_clear);
	LCD_command(LCD_row_1);
	LCD_command(LCD_clear);
}

void LCD_command(uint8_t command){
	//write command to LCD
	
	uint8_t higher_nibble, lower_nibble;
	
	//send the higher nibble first
	higher_nibble = (command & 0xf0);
	
	//trigger command write cycle,,,,pull both RS and RW low and pulse enable with a delay in between
	I2C_write_PCF8574(PCF8574_address_write,(0b00000100 | higher_nibble));
	_delay_ms(10);
	I2C_write_PCF8574(PCF8574_address_write,(0b00000000 | higher_nibble));
	_delay_ms(10);
	
	//send lower nibble last
	lower_nibble = ((command<<4) & 0xf0);
	
	//trigger command write cycle,,,,pull both RS and RW low and pulse enable with a delay in between
	I2C_write_PCF8574(PCF8574_address_write,(0b00000100 | lower_nibble));
	_delay_ms(10);
	I2C_write_PCF8574(PCF8574_address_write,(0b00000000 | lower_nibble));
	_delay_ms(10);
}

void LCD_data(uint8_t data){
	//write character data to LCD
	
	uint8_t higher_nibble, lower_nibble;
	
	//send the higher nibble first
	higher_nibble = (data & 0xf0);
	
	//trigger command write cycle,,,RS high and RW pull low ,pulse enable with a delay in between
	I2C_write_PCF8574(PCF8574_address_write,(0b00000101 | higher_nibble));
	_delay_ms(10);
	I2C_write_PCF8574(PCF8574_address_write,(0b00000001 | higher_nibble));
	_delay_ms(10);
	
	//send lower nibble last
	lower_nibble = ((data<<4) & 0xf0);
	
	//trigger command write cycle,,,RS high and RW pull low ,pulse enable with a delay in between
	I2C_write_PCF8574(PCF8574_address_write,(0b00000101 | lower_nibble));
	_delay_ms(10);
	I2C_write_PCF8574(PCF8574_address_write,(0b00000001 | lower_nibble));
	_delay_ms(10);
}

void LCD_print(char *str){
	//prints a string to LCD using the I2C interface
	int i;
	for(i = 0; str[i] !=0; i++){
		LCD_data(str[i]);
	}
}



/********************************************************DS3231 functions******************************************************************************/
void DS3231_write(uint8_t DS3231_A_W,uint8_t reg_address){
	//establishes communication with DS3231 with functionality defined as write	
	
	//convert reg_address to BCD
	DEC_BCD(reg_address);
	
	I2C_start();
	TWDR = DS3231_A_W;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
}


int DS3231_read(uint8_t write_address, uint8_t register_address, uint8_t read_address){

    uint8_t data;
	//convert reg_address to BCD
	DEC_BCD(register_address);
		
	I2C_start();
	TWDR = write_address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	I2C_start();
	TWDR = read_address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	
	
	TWCR = (1<<TWINT) | (1<<TWEN);                           // | (1<<TWEA)
	while(!(TWCR &(1<<TWINT)));
 
	I2C_stop();
	return TWDR;
}

void DS3231_read_time(){
	//reads time and date from DS3231
	DS3231_read(DS3231_address_write,DS3231_sec_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	second = DEC_value;                                                                  
	
	DS3231_read(DS3231_address_write,DS3231_min_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	minutes = DEC_value;                                                                
	
	DS3231_read(DS3231_address_write,DS3231_hour_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	hours = DEC_value;
	
	DS3231_read(DS3231_address_write,DS3231_day_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	days = DEC_value;
	
	DS3231_read(DS3231_address_write,DS3231_date_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	dates = DEC_value;
	
	DS3231_read(DS3231_address_write,DS3231_month_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	months = DEC_value;
	
	DS3231_read(DS3231_address_write,DS3231_year_reg,DS3231_address_read);
	BCD_DEC(TWDR);
	years = DEC_value;
}

void DEC_BCD(uint8_t value){
	BCD_value = (value/10*16) + (value%10);
}

void BCD_DEC(uint8_t value){
	DEC_value = (value/16*10) + (value%16);
}

void DS3231_set(uint8_t seconds, uint8_t minute, uint8_t hour, uint8_t day_week,uint8_t date, uint8_t month, uint8_t year){
	//writes to DS3231 time and date registers starting from register 00h to register 0x06
	DS3231_write(DS3231_address_write, DS3231_sec_reg);
	
	DEC_BCD(seconds);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(minute);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(hour);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(day_week);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(date);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(month);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	DEC_BCD(year);
	TWDR = BCD_value;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));
	
	I2C_stop();
}

int main(void)
{
    /* Replace with your application code */
	I2C_init();
	
	lcd_i2c_init();
  
	DS3231_set(59,59,23,3,31,12,20);
	_delay_ms(100);
	LCD_print("..I2C LCD & DS3231..");
	
    while (1) {
	//read time and date
	DS3231_read_time();
	_delay_ms(2000);
	
	LCD_command(LCD_clear);
	LCD_command(LCD_row_2);
	LCD_print("TIME:");
	LCD_print(itoa(hours,str_buffer,10));
	LCD_data(':');
	LCD_print(itoa(minutes,str_buffer,10));
	LCD_data(':');
	LCD_print(itoa(second,str_buffer,10));
	
	LCD_command(LCD_row_3);
	LCD_print("DATE:");
	LCD_print(itoa(dates,str_buffer,10));
	LCD_data(':');
	LCD_print(itoa(months,str_buffer,10));
	LCD_data(':');
	LCD_print(itoa(years,str_buffer,10));
	
	switch (days){
		case 1:
		LCD_command(LCD_row_4);
		LCD_print("Sunday");
		break;

		case 2:
		LCD_command(LCD_row_4);
		LCD_print("Monday");
		break;
		
		case 3:
		LCD_command(LCD_row_4);
		LCD_print("Tuesday");
		break;
		
		case 4:
		LCD_command(LCD_row_4);
		LCD_print("Wednesday");
		break;
		
		case 5:
		LCD_command(LCD_row_4);
		LCD_print("Thursday");
		break;
		
		case 6:
		LCD_command(LCD_row_4);
		LCD_print("Friday");
		break;
		
		case 7:
		LCD_command(LCD_row_4);
		LCD_print("Saturday");
		break;
		
		default:
		LCD_command(LCD_row_4);
		LCD_print("NULL");
		break;
		}
	}
}

