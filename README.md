# Arduino Weather Station

This project is a home brew weather station. It uses SwitchDoc Labs WeatherPiArduino Board and weather sensors. The SDL WeatherPiArduino Board is connected to an Arduino Uno which sends serial data to a connected Raspberry Pi running a Python script to collect and parse the sensor data from the Arduino. This script creates files which can be served on a web page. 

Also included in the repository are scripts to use an ESP8266 board connected to a Nextion screen to display the weather data. There is also an Arduino sketch to display data on a Heltec Wifi Kit 8 display screen integrated with an ESP8266 board.

### Hardware:
* SwitchDoc Labs WeatherPiArduino: http://www.switchdoc.com/weatherpiarduino-version-2/
* SwitchDoc Labs WeatherRack Weather Sensors: https://www.switchdoc.com/weatherrack-weather-sensors/
* Arduino Uno
* Raspberry Pi 3B+
* ESP8266 NodeMCU LUA CP2102 ESP-12E Internet WiFi Development Board
* ESP8266 WiFi Development Board with 0.91 Inch ESP8266 OLED Display CP2012 NodeMCU LUA
