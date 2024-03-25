/*
 * LCD_H File
 * Author: Binuli 
 */ 

#ifndef LCD_H_									//header guard
#define LCD_H_

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DPRT PORTB
#define LCD_DDDR DDRB
#define LCD_RS	 0
#define LCD_EN   1

void LCD_Command(unsigned char cmnd);
void LCD_Init();
void LCD_Clear();
void LCD_Char(unsigned char data);
void LCD_String_xy(unsigned char, unsigned char, char *);


#endif 





