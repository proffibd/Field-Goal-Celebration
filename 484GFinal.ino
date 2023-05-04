/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-temperature-humidity-sensor-lcd
 */

#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11

// Header for TouchScreen
#include "Adafruit_TSC2007.h"
Adafruit_TSC2007 touch;

// DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// PIDArduino
#include <PID_v1.h>

const int
	LCD_PIN_RS = 11,
	LCD_PIN_EN = 12,
	LCD_PIN_D4 = 2,
	LCD_PIN_D5 = 3,
	LCD_PIN_D6 = 4,
	LCD_PIN_D7 = 5;

LiquidCrystal lcd(
	LCD_PIN_RS,
	LCD_PIN_EN,
	LCD_PIN_D4,
	LCD_PIN_D5,
	LCD_PIN_D6,
	LCD_PIN_D7);

const int RELAY_PIN = 3;  // the Arduino pin, which connects to the IN pin of relay

// PID Variables
double Setpoint, Input, Output;

// PID object and Tuning Parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1,P_ON_M, DIRECT); //P_ON_M specifies that Proportional on Measurement be used
                                                            //P_ON_E (Proportional on Error) is the default behavior

void setup()
{
  dht.begin();     // initialize the sensor
  
  // Introduce the touchscreen
  lcd.clear();
  if (!touch.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Touch Screen Lost");
    while (1) {
      delay(10);
    } 
  }
  
  lcd.print("Touch Controller Found!");

  // Set up the Relay Module
  // initialize digital pin as an output.
  pinMode(RELAY_PIN, OUTPUT);

  // Setup Code for PID
  Input = analogRead(DHTPIN);
  Setpoint = 700;

  // turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  delay(2000); // wait a few seconds between measurements

  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature

  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    // Printing the Actual Temperature and Humidity
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");

    lcd.setCursor(0, 1);  // start to print at the second row
    lcd.print("Humi: ");
    lcd.print(humi);      // print the humidity
    lcd.print("%");
  }

  // Reading the Scroll of the Touchscreen 
  // To Control the Relay Module
  uint16_t x, y, z1, z2;
  int currX;
  int currY;
  int desiredTemp;
  int desiredHumi;

  if (touch.read_touch(&x, &y, &z1, &z2)) {
    
    while (z1 != 0) { // Printing when the touch screen is pressed
      // Move the x to change the temperature
      currX = x;
     
      // Move the y to change the humidity
      currY = y;

      // Calculating changes
      desiredTemp = tempC + (currX % 100);
      desiredHumi = humi + (currY % 100);

      // Printing the Desired Temperature and Humidity
      lcd.clear();
      lcd.setCursor(0, 0);  // start to print at the first row
      lcd.print("D.Temp: ");
      lcd.print(desiredTemp);     // print the temperature
      lcd.print((char)223); // print ° character
      lcd.print("C");

      lcd.setCursor(0, 1);  // start to print at the second row
      lcd.print("D.Humi: ");
      lcd.print(desiredHumi);      // print the humidity
      lcd.print("%");  
      delay(1000); 
      lcd.clear();             
    }
  }
  
  delay(1000);

  // After reading the desired touch
  // Decide when to turn on the relay module
  // To activate the heater
  // Quick PID Check
  Input = analogRead(DHTPIN);
  myPID.Compute();
  analogWrite(3, Output);
  
  if (desiredTemp > tempC + 2) { // Time to turn the heater on!
      digitalWrite(RELAY_PIN, HIGH);
      delay(500);
  }

  if (desiredTemp == tempC || desiredTemp < tempC) { // Stop the heater!
      digitalWrite(RELAY_PIN, LOW);
      delay(500);
  }
  
}
/*
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C address 0x3F, 16 column and 2 rows
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  dht.begin();     // initialize the sensor
  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight 
}
*/