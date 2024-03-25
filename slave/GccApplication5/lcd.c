/*
 *lcd.c file
 *Author: Binuli 
 */
#include "lcd.h"

void LCD_Command(unsigned char cmd)					//LCD_Command is used to send commands
{
	LCD_DPRT = (LCD_DPRT & 0x0f)|(cmd & 0xf0);		// Sending command to data port - upper nibble
	LCD_DPRT &= ~ (1<<LCD_RS);						// RS = 0 for command
	LCD_DPRT |= (1<<LCD_EN);						// EN = 1 
	_delay_us(1);									// wait for make enable wide
	LCD_DPRT &= ~(1<<LCD_EN);						// EN = 0 
	_delay_us(100);									// wait for make enable wide 
	
	LCD_DPRT = (LCD_DPRT & 0x0f)|(cmd << 4);		// Sending command to data port - lower nibble
	LCD_DPRT |= (1<<LCD_EN);						// EN = 1
	_delay_us(1);									// wait for make enable wide
	LCD_DPRT &= ~(1<<LCD_EN);						// EN = 0
	_delay_us(2000);								// wait for make enable wide
}

void LCD_Init()
{
	LCD_DDDR = 0xFF;								//Making DDRD pins as output pins
	_delay_ms(20);									//wait for some time
	LCD_Command(0x02);								// send for 4 bit initialization of LCD
	LCD_Command(0x28);								// INIT. LCD 2 line, 5 X 8 matrix 
	LCD_Command(0x0C);								// Display cursor
	LCD_Command(0x01);								// clears LCD
	LCD_Command(0x82);								// Shift cursor to write
}


void LCD_Clear()									//This function is used to clear the Display of the LCD
{
	LCD_Command(0x01);								//Clears display 
	_delay_ms(3);
	LCD_Command(0x80);								// Cursor at home position (left edge of the display)
}

void LCD_Char(unsigned char data)					//This function is used to display a single character on the LCD
{
	LCD_DPRT = (LCD_DPRT & 0x0f)|(data & 0xf0);		// Send data to data port-  sending upper nibble
	LCD_DPRT |= (1<<LCD_RS);						// make RS = 1 for data
	LCD_DPRT |= (1<<LCD_EN);						// EN=1 
	_delay_us(1);									// wait for make enable wide
	LCD_DPRT &= ~(1<<LCD_EN);						// EN = 0 
	_delay_us(100);									// wait for make enable wide
	
	LCD_DPRT = (LCD_DPRT & 0x0f)|(data << 4);		// Send data to data port-  sending lower nibble
	LCD_DPRT |= (1<<LCD_EN);						// EN=1 
	_delay_us(1);									// wait for make enable wide
	LCD_DPRT &= ~(1<<LCD_EN);						// EN = 0 
	_delay_us(2000);								// wait for make enable wide
}

void LCD_String_xy(unsigned char row, unsigned char pos, char *str) //This function is used to display a character array on the LCD with x,y position

{
	if (row == 1)
	LCD_Command((pos & 0x0F)|0x80);									// Command of first row and pos should be less than 16
	else if (row == 2)
	LCD_Command((pos & 0x0F)|0xC0);									// Command of Second row 
	
	int i;
	for(i=0;str[i]!=0;i++)											// Send each char of string till the NULL
	{
		LCD_Char (str[i]);											// passes each char to the LCD_char function
	}
}







