#include "libLCD/hd44780.h"
#include "libLCD/hd44780.c"
#include "libTouch/touch.h"


int main{

	int x = x_pin;
	int y = y_pin;

	int set_temp = 70;
	int set_humi = 50;
	int temp;
	int humi;
	short humidifier;
	short heater;


	while(1){
		
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
		temp = PA0;
		temp = (double)temp/1024;
		temp = ((temp * 5) = 0.5) * 100;
		humi = PD8;
		if (humi < set_humi - 1) {
			humidifier = 1;
		}
		if (temp < set_temp - 1) {
			heater = 1;
		}
		if (humi =< set_humi) {
			humidifier = 0;
		}
		if (temp =< set_temp) {
			heater = 0;
		}
		if (humidifier) {
        		PORTD |= _BV(PORTD6);
		}
		else {
        		PORTD &= ~_BV(PORTD6);
		}

		if (heater) {
        		PORTD |= _BV(PORTD8);
		}
		else {
        		PORTD &= ~_BV(PORTD8);
		}
	}

}