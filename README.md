# weather-station

This project is a home brew weather station. It uses Switch Doc Labs WeatherBoard and weather sensors. The SDL WeatherBoard is connected to an Arduino Uno which sends serial data to a connected Raspberry Pi running a Python script to collect and parse the sensor data from the Arduino. This script creates files which can be served on a web page. 

Also included in the repository are scripts to use an ESP8266 board connected to a Nextion screen to display the weather data. There is also an Arduino sketch to display data on a Heltec Wifi Kit 8 display screen integrated with an ESP8266 board.
