/*This Arduino sketch gets weather data from a DHT 22 sensor and a web file and displays that data on a Nextion 
screen via a serial connection.
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_BMP085_U.h>

//data for local sensors:---------------------------------------------------------------------------------------------
#define DHTPIN 4
#define DHTTYPE DHT22
#define ELEV       4.31377 //Elevation factor to add to pressure to get equivalent sea level pressure in inches 4.25377
#define ELEVMB     146.555 //Elevation factor to add to pressure in mb 129.665
#define METERSFEET 3.28084//Conversion for number of feet to 1 meter
#define nexSerial Serial1

float temperature = 0.0f;
float temperatureF = 0.0f;
float humidity = 0.0f;
float altitude = 0.0f;
float currentPressureIn = 0.0f;
String humidityEthStr = "";
void readSensor();
DHT dht(DHTPIN, DHTTYPE); //instantiate dht sensor object
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
//end data for local sensors-----------------------------------------------------------------------------------------
 
const char* ssid = "mySSID";
const char* password = "hackme";
const char* httpRequest = "http://k7bbr.net/weather/raw.json";
String payload;
void sendAltitudeToNextion();
void sendHumidityToNextion(String *);
void sendTemperatureToNextion();
void sendTemperatureFToNextion();
void sendTimeToNextion(String *);
void sendOTempCToNextion(String *);
void sendOTempFToNextion(String *);
void displaySensorDetails(void);
void sendDirToNextion(String *);
void sendSpeedToNextion(String *);
void sendGustToNextion(String *);
void sendPressureToNextion(String *);
void sendHrainToNextion(String *);
void sendHighToNextion(String *);
void sendLowToNextion(String *);
void getWxWifi();
 
void setup ()
{
  Serial.begin(115200);
  nexSerial.begin(9600);
  dht.begin();
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print("Connecting..");
  } 
}
 
void loop() 
{
  Serial.println();
  readSensor();
  endNextionCommand(); //add this end command so previous serial commands don't interfere with subsequent Nextion updates
  sendTemperatureToNextion();
  sendTemperatureFToNextion();
  sendAltitudeToNextion();
  getWxWifi();
  delay(5000);    //Send a request every 30 seconds 
}

void readSensor()
{
 humidity = dht.readHumidity();
 temperature = dht.readTemperature();
 temperatureF = dht.readTemperature(true);
}

void sendHumidityToNextion(String *phumidity)
{
  String command = "humidity.txt=\""+String(*phumidity)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendTemperatureToNextion()
{
  String command = "temperature.txt=\""+String(temperature,1)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendTemperatureFToNextion()
{
  String command = "temperatureF.txt=\""+String(temperatureF,1)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendAltitudeToNextion()
{
  String command = "altitudeFt.txt=\""+String(altitude,1)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendTimeToNextion(String *ptime)
{
  String command = "timeNx.txt=\""+String(*ptime)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendOTempCToNextion(String *potempc)
{
  String command = "otempc.txt=\""+String(*potempc)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendOTempFToNextion(String *potempf)
{
  String command = "otempf.txt=\""+String(*potempf)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendDirToNextion(String *pdir)
{
  String command = "dir.txt=\""+String(*pdir)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendSpeedToNextion(String *pspeed)
{
  String command = "speed.txt=\""+String(*pspeed)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendGustToNextion(String *pgust)
{
  String command = "gust.txt=\""+String(*pgust)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendHrainToNextion(String *phrain)
{
  String command = "hrain.txt=\""+String(*phrain)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendHighToNextion(String *phigh)
{
  String command = "high.txt=\""+String(*phigh)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendLowToNextion(String *plow)
{
  String command = "low.txt=\""+String(*plow)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void sendPressureToNextion(String *ppressure)
{
  String command = "pressureIn.txt=\""+String(*ppressure)+"\"";
  nexSerial.print(command);
  endNextionCommand();
}

void endNextionCommand()
{
  nexSerial.write(0xff);
  nexSerial.write(0xff);
  nexSerial.write(0xff);
}

void getWxWifi()
{
if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
  { 
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(httpRequest);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) //Check the returning code
    { 
 
      payload = http.getString();   //Get the request response payload
      //String& payloadRef = payload;
      //Serial.println(payload);                     //Print the response payload ****** for DEBUG
    }
 
    http.end();   //Close connection
  }

  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
 const size_t bufferSize = JSON_OBJECT_SIZE(13) + 530;
DynamicJsonBuffer jsonBuffer(bufferSize);
Serial.println(bufferSize);

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(payload);
  Serial.println(payload);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    return;
  }

String time = root["time"]; // "Time: Sat Dec 15 03:50:55 2018"
String dir = root["dir"]; // "Wind Direction: 270.00"
String speed = root["speed"]; // "Wind Speed: 5.35"
String gust = root["gust"]; // "Wind Gust: 3.90"
String otempc = root["otempc"]; // "-1.40"
String otempf = root["otempf"]; // "29.48"
String humid = root["humid"]; // "RH: 76.90%"
String itempc = root["itempc"]; // "3.08"
String itempf = root["itempf"]; // "37.54"
String pressure = root["pressure"]; // "Barometric Pressure: 30.16"
String hrain = root["hrain"]; // "Hourly Rain Total: 0.00"
String dayrain = root["dayrain"]; // "24 Hour Rain Total 0.00"
String high = root["high"]; // "33.98"
String low = root["low"]; // "32.90"
//Print data to the serial interface to a connected serial device--------------------------
Serial.println();
Serial.println(time);
Serial.println(dir);
Serial.println(speed);
Serial.println(gust);
Serial.println(otempc);
Serial.println(otempf);
Serial.println(humid);
Serial.println(itempc);
Serial.println(itempf);
Serial.println(pressure);
Serial.println(hrain);
Serial.println(dayrain);
Serial.println(high);
Serial.println(low);

//print data to the Nextion screen----------------------------------------------------------
endNextionCommand();
sendHumidityToNextion(&humid);
sendTimeToNextion(&time);
sendOTempCToNextion(&otempc);
sendOTempFToNextion(&otempf);
sendDirToNextion(&dir);
sendSpeedToNextion(&speed);
sendGustToNextion(&gust);
sendHrainToNextion(&hrain);
sendHighToNextion(&high);
sendLowToNextion(&low);
sendPressureToNextion(&pressure);
delay(10000);
}  
