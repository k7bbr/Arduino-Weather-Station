#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>

const char* ssid = "mySSID";
const char* password = "hackme";
const char* httpRequest = "http://k7bbr.net/weather/raw.json";
void getWxWifi();
void parseJSON(String&);
void setDisplay(String *, String *, String *, String *);
void setDisplay(String&, String&);
void setDisplay(String&, String&, String&, String&);
void setDisplay(String&, String&, String&, String&, String&);

//U8g2 Contructor---------------------------------------------------------------------------------
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

void setup ()
{
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print("Connecting..");
  } 

  u8g2.begin();
}
 
void loop() 
{
  Serial.println();
  getWxWifi(); 
}

void getWxWifi()
{
String payload;
String& payloadRef = payload;
if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
  { 
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(httpRequest);  //Specify request destination
    int httpCode = http.GET();  //Send the request
    if (httpCode > 0) //Check the returning code
    { 
      payload = http.getString();   //Get the request response payload
      Serial.println(payloadRef);                     //Print the response payload ****** for DEBUG
    }
    http.end();   //Close connection
  }
  parseJSON(payloadRef);
}

void parseJSON(String& rpayloadRef)
{
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
 const size_t bufferSize = JSON_OBJECT_SIZE(13) + 530;
DynamicJsonBuffer jsonBuffer(bufferSize);

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(rpayloadRef);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    //return;
  }

String time = root["time"]; // "Time: Sat Dec 15 03:50:55 2018"
String timeAbb = time.substring(6,25);
String& timeRef = timeAbb;
String dir = root["dir"]; // "Wind Direction: 270.00"
String& dirRef = dir;
String speed = root["speed"]; // "Wind Speed: 5.35"
String& speedRef = speed;
String gust = root["gust"]; // "Wind Gust: 3.90"
String& gustRef = gust;
String otempc = root["otempc"]; // "-1.40"
String otempf = root["otempf"]; // "29.48"
String& otempfRef = otempf;
String humid = root["humid"]; // "RH: 76.90%"
String& humidRef = humid;
String itempc = root["itempc"]; // "3.08"
String itempf = root["itempf"]; // "37.54"
String& itempfRef = itempf;
String pressure = root["pressure"]; // "Barometric Pressure: 30.16"
String& pressureRef = pressure;
String hrain = root["hrain"]; // "Hourly Rain Total: 0.00"
String& hrainRef = hrain;
String dayrain = root["dayrain"]; // "24 Hour Rain Total 0.00"
String& dayrainRef = dayrain;
String high = root["high"]; // "33.98"
String& highRef = high;
String low = root["low"]; // "32.90"
String& lowRef = low;

//root.printTo(Serial);
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

//Labels for data fields:-----------------------------------------------
String timeLabel = "K7BBR WX Station";
String& timeLabelRef = timeLabel;
String otempfLabel = "Outside Temp: ";
String& otempfLabelRef = otempfLabel;
String highLowLabel = "H/L Temp: ";
String& highLowLabelRef = highLowLabel;
String dirLabel = "Wind Dir: ";
String& dirLabelRef = dirLabel;
String speedLabel = "Speed: ";
String& speedLabelRef = speedLabel;
String humidLabel = "Humidity: ";
String& humidLabelRef = humidLabel;
String pressureLabel = "Pressure: ";
String& pressureLabelRef = pressureLabel;
String hrainLabel = "Hr Rain: ";
String& hrainLabelRef = hrainLabel;
String dayrainLabel = "24 hr Rain: ";
String& dayrainLabelRef = dayrainLabel;

for (int i = 0; i<2; i++)
  {
  setDisplay(timeLabelRef, timeRef);
  delay(3000);
  
  setDisplay(dirLabelRef, dirRef, speedLabelRef, speedRef, gustRef);
  delay(3000);
  
  setDisplay(otempfLabelRef, otempfRef, highLowLabelRef, highRef, lowRef);
  delay(3000);
  
  setDisplay(humidLabelRef, humidRef, pressureLabelRef, pressureRef);
  delay(3000);
  
  setDisplay(hrainLabelRef, hrainRef, dayrainLabelRef, dayrainRef);
  delay(2000);
  
  setDisplay(otempfLabelRef, otempfRef, highLowLabelRef, highRef, lowRef);
  delay(4000);
  }
}

void setDisplay(String *plabel1, String *pdata1, String *plabel2, String *pdata2)
{
  u8g2.setFont(u8g2_font_t0_12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(*plabel1 + *pdata1);
  u8g2.setCursor(0, 30);
  u8g2.print(*plabel2 + *pdata2);
  Serial.println(*plabel2 + *pdata2);
  u8g2.sendBuffer(); 
}

void setDisplay(String& rlabel1, String& rdata2)
{
  u8g2.setFont(u8g2_font_t0_12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(rlabel1);
  u8g2.setCursor(0, 30);
  u8g2.print(rdata2);
  Serial.println(rdata2);
  u8g2.sendBuffer();
}
void setDisplay(String& rlabel1, String& rdata1, String& rlabel2, String& rdata2)
{
  u8g2.setFont(u8g2_font_t0_12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(rlabel1 + rdata1);
  u8g2.setCursor(0, 30);
  u8g2.print(rlabel2 + rdata2);
  Serial.println(rlabel2 + rdata2);
  u8g2.sendBuffer();
}

void setDisplay(String& rlabel1, String& rdata1, String& rlabel2, String& rdata2a, String& rdata2b) //Overloaded method for line 1 single data, line 2 double data
{
  u8g2.setFont(u8g2_font_t0_12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(rlabel1 + rdata1);
  u8g2.setCursor(0, 30);
  u8g2.print(rlabel2 + rdata2a + "/" + rdata2b);
  Serial.println(rlabel2 + rdata2a+ "/" + rdata2b);
  u8g2.sendBuffer();
}
