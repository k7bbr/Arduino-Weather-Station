/*
  SDL_Weather_80422.cpp - Library for SwitchDoc Labs WeatherRack.
  SparkFun Weather Station Meters
  Argent Data Systems
  Created by SwitchDoc Labs July 27, 2014.
  Released into the public domain.
  Version 1.1 - updated constants to suppport 3.3V
  
  Supports WeatherPiArduino Board www.switchdoc.com
*/
#include <Wire.h>
#include <Time.h>
#include <Adafruit_AM2315.h>
#include <Adafruit_BMP280.h>
#include "SDL_Weather_80422.h"



#define pinLED     13   // LED connected to digital pin 13
#define pinAnem    2  // Anenometer connected to pin 18 - Int 5 - Mega   Int 0 Uno / Uno pin 2
#define pinRain    3  // Rain Gauge connected to pin 2 - Int 0 - Mega   / Uno Pin 3 
#define intAnem    0 // int 0 (check for Uno) Int 5 for mega
#define intRain    1  // int 1
#define ELEV       4.31377 //Elevation factor to add to pressure to get equivalent sea level pressure in inches 4.25377
#define ELEVMB     146.555 //Elevation factor to add to pressure in mb 129.665

// for mega, have to use Port B - only Port B works.
/*
 Arduino Pins         PORT
 ------------         ----
 Digital 0-7          D
 Digital 8-13         B
 Analog  0-5          C
*/


//Initialize the AM2315 Temperature Probe library
Adafruit_AM2315 am2315;

//Initialize the BMP280 Barometer and temperature and alitmeter library
Adafruit_BMP280 bmp; // I2C

// initialize SDL_Weather_80422 library
SDL_Weather_80422 weatherStation(pinAnem, pinRain, intAnem, intRain, A0, SDL_MODE_INTERNAL_AD);


uint8_t i;

float currentWindSpeed;
float currentWindGust;
float totalRain;
float currentPressurePa;
float currentPressureMb;
float currentPressureIn;
float pressureIn;
float pressureMb;

void setup()
{ 
  Serial.begin(57600);  

//Check to see if the AM2315 Temperature/Humidity probe is working  
if (! am2315.begin()) 
    {
     Serial.println("Sensor not found, check wiring & pullups!");
    }

//Check to see if BMp280 senseor is working
if (!bmp.begin()) 
    {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    //while (1);
    }    
 
 Serial.println("-----------");
 Serial.println("WeatherArduino SDL_Weather_80422 Class Test");
 Serial.println("Version 1.1");
 Serial.println("-----------");
      
      
      weatherStation.setWindMode(SDL_MODE_SAMPLE, 5.0);
      //weatherStation.setWindMode(SDL_MODE_DELAY, 5.0);
      totalRain = 0.0;
}


void loop()
{/*
  Serial.println("----------------");

  currentWindSpeed = weatherStation.current_wind_speed()/1.6;
  currentWindGust = weatherStation.get_wind_gust()/1.6;
  totalRain = totalRain + weatherStation.get_current_rain_total()/25.4;
  Serial.print("rain_total=");
  Serial.print(totalRain);
  Serial.print(""" wind_speed=");
  Serial.print(currentWindSpeed);
  Serial.print("MPH wind_gust=");
  Serial.print(currentWindGust);
  Serial.print("MPH wind_direction=");
  Serial.print(weatherStation.current_wind_direction());
  Serial.print(" HUM:");
  Serial.print(am2315.readHumidity());
  delay(50); //AM2315 requires a pause in the query or else you'll get 'nan' for the value
  Serial.print(" Temp:");
  Serial.print(am2315.readTemperature());
  Serial.print(F(" BMP280 Temperature="));
  Serial.print(bmp.readTemperature());
  Serial.print(F(" Pressure="));

  /*Convert air pressure from Pascals to inches and mb */
/*
  currentPressurePa = (bmp.readPressure());
  currentPressureIn = currentPressurePa*0.00029530;
  currentPressureMb =  currentPressurePa/100;
  pressureIn = currentPressureIn + ELEV;
  pressureMb = currentPressureMb +ELEVMB;
  
  Serial.print(pressureIn, 2);
  Serial.print(" in");
  Serial.print(" (");
  Serial.print(pressureMb, 2);
  Serial.print(" mb)");
 
  Serial.print(F(" Approx altitude="));
  Serial.print(bmp.readAltitude(pressureMb));//this should be adjusted for your local altimeter setting
  Serial.println(" m");
  */
  
  currentWindSpeed = weatherStation.current_wind_speed()/1.6;
  currentWindGust = weatherStation.get_wind_gust()/1.6;
  totalRain = totalRain + weatherStation.get_current_rain_total()/25.4;
  //Serial.print("rain_total=");
  Serial.print(totalRain);//0
  Serial.print(",");
  Serial.print(currentWindSpeed);//1
  Serial.print(",");
  Serial.print(currentWindGust);//2
  Serial.print(",");
  Serial.print(weatherStation.current_wind_direction());//3
  Serial.print(",");
  Serial.print(am2315.readHumidity());//4
  delay(50); //AM2315 requires a pause in the query or else you'll get 'nan' for the value
  Serial.print(",");
  Serial.print(am2315.readTemperature());//5
  Serial.print(F(","));
  Serial.print(bmp.readTemperature());//6
  Serial.print(F(","));

  /*Convert air pressure from Pascals to inches and mb */
  
  currentPressurePa = (bmp.readPressure());
  currentPressureIn = currentPressurePa*0.00029530;
  currentPressureMb =  currentPressurePa/100;
  pressureIn = currentPressureIn + ELEV;
  pressureMb = currentPressureMb +ELEVMB;
  
  Serial.print(pressureIn, 2);//7
  Serial.print(",");
  //Serial.print(" (");
  Serial.print(pressureMb, 2);//8
  Serial.print(",");
 
  //Serial.print(F(" Approx altitude="));
  Serial.print(bmp.readAltitude(pressureMb));//this should be adjusted for your local altimeter setting 9
  Serial.println(",");

  
  delay(3000);
  
  
}

