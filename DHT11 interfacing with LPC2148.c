
/*
  DHT11 interfacing with LPC2148(ARM7)
*/

#include <lpc214x.h>
#include <stdio.h>
#include <string.h>

void delay_ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    
	}
}

void delay_us(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<7; x++);   
	}
}

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	
	U0LCR = 0x83;	
	U0DLM = 0x00;	
	U0DLL = 0x61;	
	U0LCR = 0x03; 
}

void UART0_TxChar(char ch) 
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	
}

void UART0_SendString(char* str) 
{
	uint8_t i = 0;
	while( str[i] != '\0' )
	{
		UART0_TxChar(str[i]);
		i++;
	}
}

unsigned char UART0_RxChar(void) 
{
	while( (U0LSR & 0x01) == 0);	
		return U0RBR;
}

void dht11_request(void)
{
	IO0DIR = IO0DIR | 0x00000010;	
	IO0PIN = IO0PIN & 0xFFFFFFEF; 
	delay_ms(20);
	IO0PIN = IO0PIN | 0x00000010; 
}

void dht11_response(void)
{
	IO0DIR = IO0DIR & 0xFFFFFFEF;	
	while( IO0PIN & 0x00000010 );	
	while( (IO0PIN & 0x00000010) == 0 );	
	while( IO0PIN & 0x00000010 );	
}

uint8_t dht11_data(void)
{
	int8_t count;
	uint8_t data = 0;
	for(count = 0; count<8 ; count++)	
	{
		while( (IO0PIN & 0x00000010) == 0 );	
		delay_us(30);	
		if ( IO0PIN & 0x00000010 ) 
			data = ( (data<<1) | 0x01 );
		else	
			data = (data<<1);
		while( IO0PIN & 0x00000010 );	
	}
	return data;
}

int main (void)
{
	uint8_t humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum; 
	char data[7]; 
	UART0_init();
	while(1)
	{
		dht11_request();
		dht11_response();
		humidity_integer = dht11_data();
		humidity_decimal = dht11_data();
		temp_integer = dht11_data();
		temp_decimal = dht11_data();
		checksum = dht11_data();
		if( (humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum )
			UART0_SendString("Checksum Error\r\n");
		else
		{			
			UART0_SendString("Relative Humidity : ");
			memset(data, 0, 7);
			sprintf(data, "%d.", humidity_integer);
			UART0_SendString(data);
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", humidity_decimal);
			UART0_SendString(data);
			UART0_SendString("Temperature : ");
			memset(data, 0, 7);
			sprintf(data, "%d.", temp_integer);
			UART0_SendString(data);
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", temp_decimal);
			UART0_SendString(data);
			UART0_SendString("Checksum : ");
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", checksum);
			UART0_SendString(data);			
			delay_ms(1000);
		}
	}
}
