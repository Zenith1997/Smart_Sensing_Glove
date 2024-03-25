#define F_CPU 8000000UL
#include <avr/io.h>
#include "usart.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "lcd.h"
#include "ADC_H.h"
							
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */

float Acc_x,Acc_y,Acc_z,Gyro_x,Gyro_y,Gyro_z;



int main(void)
{
	USART_Init(9600);
	ADC_Init();
	LCD_Init();
	sei();
	uint8_t value;
	char buffer[12];

	while(1)
	{	
		// LCD_String_xy(1,0,"hi");
	//	value= ReadData();
		 AtoD();
		LCD_String_xy(2, 10, buffer);

	}
}




void AtoD()
{
	
	
	
	char buffer[20];
	int ADC_Value1;
	int ADC_Value2;
	int ADC_Value3;
	int ADC_Value4;
	int ADC_Value5;
	

	

	//LCD_Clear();
	// sprintf(buffer, "%d -->> ", signL);
//	 LCD_String_xy(2,0,buffer);
	 
	ADC_Value1 = ADC_Read(0);		/* Read the status on X-OUT pin using channel 0 */
	sprintf(buffer, "A%d   ", ADC_Value1);
	LCD_String_xy(1, 2, buffer);
	
	ADC_Value2 = ADC_Read(1);		/* Read the status on Y-OUT pin using channel 0 */
	sprintf(buffer, "B%d   ", ADC_Value2);
	LCD_String_xy(1, 10, buffer);


	ADC_Value3 = ADC_Read(2);		/* Read the status on SWITCH pin using channel 0 */
	sprintf(buffer, "C%d   ", ADC_Value3);
	LCD_String_xy(1, 17, buffer);
	
	
	ADC_Value4 = ADC_Read(3);		/* Read the status on SWITCH pin using channel 0 */
	sprintf(buffer, "D%d   ", ADC_Value4);
		LCD_String_xy(1, 24, buffer);
	
	ADC_Value5 = ADC_Read(4);		/* Read the status on SWITCH pin using channel 0 */
	sprintf(buffer, "E%d   ", ADC_Value5);
	LCD_String_xy(1, 32, buffer);
	if (ADC_Value1<500&&ADC_Value2<500&&ADC_Value3<500&&ADC_Value4<500&&ADC_Value5<500)
	{
		
		
		sprintf(buffer, "Comehere  ");
		//LCD_String_xy(2, 10, buffer);
		//_delay_ms(1000);
		
		
		
	}
	else if (ADC_Value1==707&&ADC_Value2==707&&ADC_Value3<700&&ADC_Value4<700&&ADC_Value5<700)
	{
		
		
		sprintf(buffer, "ha haaa  ");
	//	LCD_String_xy(2, 10, buffer);
		//_delay_ms(1000);
		
		
		
	}
	else
	{
		 
		sprintf(buffer, "ha ertwt ");
		//LCD_String_xy(2, 10, buffer);
		//_delay_ms(1000);
		
	}
}
