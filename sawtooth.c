#include "libLCD/hd44780.h"
#include "libLCD/hd44780.c"
// #include "libTouch/touch_settings.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#define x_pin 19
#define y_pin 18
#define DHT11_PIN 14
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;


int Request (void)				/* Microcontroller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
}

int Response (void)				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data ()			/* receive data */
{	
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}





int main (void) {

	int x = x_pin;
	int y = y_pin;
	char data[5];
	int set_temp = 70;
	int set_humi = 50;
	short humidifier;
	short heater;


	while(1){
		


		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */

		if (x < 2000){
		set_temp = set_temp - 1;
		}
		if (x > 2000){
		set_temp = set_temp + 1;
		}
		if (y < 2000){
		set_humi = set_humi - 1;
		}
		if (y > 2000){
		set_humi = set_humi + 1;
		}

		if (I_RH < set_humi - 1) {
			humidifier = 1;
		}
		if (D_RH < set_temp - 1) {
			heater = 1;
		}
		if (I_RH >= set_humi) {
			humidifier = 0;
		}
		if (D_RH >= set_temp) {
			heater = 0;
		}
		if (humidifier) {
        		PORTD |= _BV(PORTD6);
		}
		else {
        		PORTD &= ~_BV(PORTD6);
		}

		if (heater) {
        		PORTD |= _BV(PORTB0);
		}
		else {
        		PORTD &= ~_BV(PORTB0);
		}
		LCD_GotoXY(0, 0);
		LCD_PrintString("Temp: ");
		LCD_PrintInteger(set_temp);
		LCD_GotoXY(0, 1);
		LCD_PrintString("Hum: ");
		LCD_PrintInteger(set_humi);


			if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			LCD_Clear();
			LCD_GotoXY(0,0);
			LCD_PrintString("Error");
		}
		
		else
		{	
			itoa(I_RH,data,10);
			LCD_GotoXY(11,0);
			LCD_PrintString(data);
			LCD_PrintString(".");
			
			itoa(D_RH,data,10);
			LCD_PrintString(data);
			LCD_PrintString("%");

			itoa(I_Temp,data,10);
			LCD_GotoXY(6,1);
			LCD_PrintString(data);
			LCD_PrintString(".");
			
			itoa(D_Temp,data,10);
			LCD_PrintString(data);
			LCD_SendData(0xDF);
			LCD_PrintString("C ");
			
			itoa(CheckSum,data,10);
			LCD_PrintString(data);
			LCD_PrintString(" ");
		}
				
	_delay_ms(10);
		
	}

}