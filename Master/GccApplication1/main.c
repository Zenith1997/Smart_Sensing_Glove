/*
 * usart_text_transmit.c
 *
 * Created: 25.01.2013 12:55:44
 *  Author: Zenith
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#define D4 eS_PORTD3
#define D5 eS_PORTD4
#define D6 eS_PORTD5
#define D7 eS_PORTD6
#define RS eS_PORTB0
#define EN eS_PORTB1
#define  Trigger_pin PC4
#include <avr/io.h>
#include "ADC_H.h"
#include "usart.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <string.h>
#include "MPU6050_res_define.h"	
#include "I2C_Master_H_file.h"							




#include <string.h>





/*GSM COMMANDS, Functions & variables*/
char gsm_ON[]={"ATE0\r"};                             //send ATE0 to check module is ready or not
char At[] = {"AT\n"};                                 // AT to check GSM  -- or "AT\n"
char Msg_Mode[] = {"AT+CMGF=1\n"};                    // Active message mode. “1” is for text only mode
char Msg_cmd1[] = {"AT+CMGS="};                      // message command
char emergency_msg[200] = {"I am in an emergency.please help..."};             // Message about emergency (push button)
char mobile_No_01[16]={"\"+94762356027\""};          //receiver mobile number
void Send_meg(char *receiver_no,char *notify);
#define Buffer_size  380
char buffer[Buffer_size];  //buffer to store responses and numbers
volatile int buffer_index = 0, call_status_flag=0;



int main(void)
{

   usart_init(9600);//we use 1 MHz clck and 2400 baud and our ubrr is 25

    ADC_Init();
	I2C_Init();	/* Initialize I2C */
	MPU6050_Init();	/* Initialize MPU6050 */
	
	
	char g;
	char c;
	
   uint8_t value;

    while(1)
    {
		
		DDRB |= 1<<PB0;	
		
	connectTXRX('c');

	value= LeftHand_Gesture();
    usart_data_transmit(value);
	
	if(PINB & (1<<PB7)) //push button
	{
		connectTXRX('g');
		
		Send_meg(mobile_No_01,emergency_msg);
		connectTXRX('c');
	}
	
	
    }
	
		freQuency();
}







	void MPU6050_Init(){										/* Gyro initialization function */

		_delay_ms(150);										/* Power up time >100ms */
		I2C_Start_Wait(0xD0);								/* Start with device write address */
		I2C_Write(SMPLRT_DIV);								/* Write to sample rate register */
		I2C_Write(0x07);									/* 1KHz sample rate */
		I2C_Stop();

		I2C_Start_Wait(0xD0);
		I2C_Write(PWR_MGMT_1);								/* Write to power management register */
		I2C_Write(0x01);									/* X axis gyroscope reference frequency */
		I2C_Stop();

		I2C_Start_Wait(0xD0);
		I2C_Write(CONFIG);									/* Write to Configuration register */
		I2C_Write(0x00);									/* Fs = 8KHz */
		I2C_Stop();

		I2C_Start_Wait(0xD0);
		I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
		I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C */
		I2C_Stop();

		I2C_Start_Wait(0xD0);
		I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
		I2C_Write(0x01);
		I2C_Stop();
	}


	void MPU_Start_Loc(){
		I2C_Start_Wait(0xD0);								/* I2C start with device write address */
		I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */
		I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
	}








int LeftHand_Gesture()
{
	
	char bufferm[5];
	uint8_t sign;
	char buffer[20];
	int ADC_Value1;
	int ADC_Value2;
	int ADC_Value3;
	int ADC_Value4;
	int ADC_Value5;
	
	float Xa,Ya,Za;
	float Xg=0,Yg=0,Zg=0;
	float Acc_x,Acc_y,Acc_z,Gyro_x,Gyro_y,Gyro_z;
	
	Xa = Acc_x/16384.0; 							/* Divide raw value by sensitivity scale factor to get real values */
	Ya = Acc_y/16384.0;
	Za = Acc_z/16384.0;
	
	Xg = Gyro_x/16.4;
	Yg = Gyro_y/16.4;
	Zg = Gyro_z/16.4;
	
	MPU_Start_Loc();									/* Read Gyro values */
									/* Read Gyro values */
		Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	
		Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());

	I2C_Stop();
	
	ADC_Value1 = ADC_Read(0);		/* Read the status on X-OUT pin using channel 0 */
	//sprintf(buffer, "A-%d   ", ADC_Value1);
	//LCD_String_xy(1, 2, buffer);
	
	ADC_Value2 = ADC_Read(1);		/* Read the status on Y-OUT pin using channel 0 */
	//sprintf(buffer, "B-%d   ", ADC_Value2);
	//LCD_String_xy(1, 10, buffer);

	ADC_Value3 = ADC_Read(2);		/* Read the status on SWITCH pin using channel 0 */
	//sprintf(buffer, "C-%d   ", ADC_Value3);
	//LCD_String_xy(1, 17, buffer);
	
	ADC_Value4 = ADC_Read(3);		/* Read the status on SWITCH pin using channel 0 */
	//sprintf(buffer, "D-%d   ", ADC_Value4);
	//LCD_String_xy(1, 24, buffer);
	
	ADC_Value5 = ADC_Read(4);		/* Read the status on SWITCH pin using channel 0 */
	//sprintf(buffer, "E-%d   ", ADC_Value5);
	//LCD_String_xy(1, 32, buffer);
	if (ADC_Value1<500&&ADC_Value2<500&&ADC_Value3<500&&ADC_Value4<500&&ADC_Value5<500&&Xa<0.5&&Ya<0.8&&Za<0.5)
	{
		
	
		sprintf(buffer, "Come here  ");
		
	
		sign=1;
		
	}
	
	else if (ADC_Value1<500&&ADC_Value2<500&&ADC_Value3<500&&ADC_Value4<500&&ADC_Value5<500&&Xa<0.5&&Ya<0.8&&Za<0.5)
	{
		
		
		sprintf(buffer, "Come here  ");
	
		//_delay_ms(1000);
		
		sign=1;
		
	}
		
	else if (ADC_Value1<500&&ADC_Value2<500&&ADC_Value3<500&&ADC_Value4<500&&ADC_Value5<500&&Xa>0.5&&Ya>0.8&&Za>0.5)
	{
		

		sprintf(buffer, "Come here  ");
	
		sign=1;
		
	}
	
	else
	{

		
		sprintf(buffer, "Sit down  ");
		
		sign=2;
	}
	
		sprintf(buffer, " # %d ", sign);
	
	
		return sign;
		
}


void freQuency(){
	
	DDRC |= 1<<PC4;
	unsigned int x,y,period;

	TCCR1A = 0;
	TCNT1=0;
	TIFR = (1<<ICF1);   /* Clear ICF (Input Capture flag) flag */

	TCCR1B = 0x41;   /* Rising edge, no prescaler */
	while ((TIFR&(1<<ICF1)) == 0);
	x = ICR1;   /* Take value of capture register */
	TIFR = (1<<ICF1);   /* Clear ICF flag */

	while ((TIFR&(1<<ICF1)) == 0);
	y = ICR1;   /* Take value of capture register */
	TIFR = (1<<ICF1);   /* Clear ICF flag */

	//TCCR1B = 0;   /*to Stop the timer */

	period=y-x;

	long f1= F_CPU/period;/* frequency Equation*/
	f1/=10;

	if(f1>800){
		PORTD|=(1<<PIND6);//switch on red led
	}
	_delay_ms(50);
	if(f1<=800 && f1<=300)
	{
		PORTD|=(1<<PIND7);//switch on green led
	}
	_delay_ms(50);
}

void Send_meg(char *receiver_no,char *notify){

	usart_string_transmit(At); // check AT
	_delay_ms(10);
	usart_string_transmit(Msg_Mode); // enter message mode
	_delay_ms(10);
	usart_string_transmit(Msg_cmd1); //
	_delay_ms(10);
	usart_string_transmit(receiver_no); // receivers number
	usart_data_transmit(13); //UDR=(13);    //  <CR> character
	_delay_ms(10);
	usart_string_transmit(notify);
	usart_data_transmit(',');
	usart_data_transmit(26);  // Ctrl+Z to send the massage
	_delay_ms(2000);
	memset(buffer, 0, Buffer_size);
	buffer_index = 0;
	
}

void connectTXRX(char x)// gsm >> g , chip >> c
{

	switch(x)
	{
		case 'c':
		PORTB &=~(1<<PB0); // O FOR GSM
		break;
		case 'g':
		PORTB |=1<<PB0; // 1 for chip
		break;
	}
}
