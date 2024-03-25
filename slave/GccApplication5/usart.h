/*
 * IncFile1.h
 *
 * Created: 5/6/2021 2:40:41 PM
 *  Author: Zenith Anthony
 */ 

#define  F_CPU 800000UL

#ifndef usart_H_
#define usart_H_
#define  F_CPU 800000UL
#include <avr/io.h>

void USART_Init( unsigned int ubrr)//initialize usart with ubrr value
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

char ReadData( void )// this function read onyl one byte
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}


void WriteData(char data)//this function write only one byte
{
	//Wait For Transmitter to become ready
	while(!(UCSRA & (1<<UDRE)));

	//Now write
	UDR=data;
}

//this function receive long string chars
//make sure buffer size is enough
//firsly read from usart the data and write to string array
//after compare last data with '\0' if it is null teminate the reading
void ReadStringData(char *str){
	
	char c;
	do{
		c=ReadData();
		*str=c;
		str++;
	}
	while(c!='\0');
	LCD_Clear();

	return;
	
}


//this function send string daha via usart byte by byte
//firsly read from byte pointer adress
//make sure readed char is not null char
//send data and increase pointer to read next char
//if readed char is null ('\0') end the transmisson after sent terminater null char
void WriteStringData(char *strData)
{
	while(*strData!='\0')
	{
		WriteData(*strData);
		strData++;
	}
	WriteData('\0');
	return;

}



#endif /* INCFILE1_H_ */

