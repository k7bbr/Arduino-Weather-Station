#!/usr/bin/python
# -*- coding: utf-8 -*-
import serial
import time
import requests
from datetime import datetime,date
import csvinit
import datetime
import sys

'''Initialize highTemp and lowTemp variables with extreme values so they are set to current temp on first program pass'''
global highTemp 
highTemp = -800
global lowTemp
lowTemp = 400

def processTime():
	global currentHour
	global dayOfMonth
	currentTime = datetime.datetime.now()
	currentMonth = currentTime.timetuple().tm_mon
	dayOfMonth=currentTime.timetuple().tm_mday
	dayOfYear = currentTime.timetuple().tm_yday
	currentHour = currentTime.timetuple().tm_hour
	
processTime()
hourCheck = currentHour
dayCheck = dayOfMonth
today = datetime.date.today()
tomorrow = today + datetime.timedelta(1)


def processYesterday():
	currentTime = datetime.datetime.now()
	#print currentTime
	global prevHour
	yesterday = currentTime - datetime.timedelta(1)
	yesterhour = currentTime - datetime.timedelta(0,0,0,0,0,1,0)
	prevMonth = currentTime.timetuple().tm_mon
	if (prevMonth == 1): #Deal with case when current month is January
		prevMonth = 12
	else:
		prevMonth = int(prevMonth) - 1
	
	prevDayOfMonth=yesterday.timetuple().tm_mday
	prevDayOfYear = yesterday.timetuple().tm_yday
	prevHour = yesterhour.timetuple().tm_hour
	
processYesterday()

def hourChange(prainHourly):
	global rainHourlyFile, currentHour, prevRain, prevHour
	global dataArray
	'''write, append rainHourly to hourlyrain.txt'''
	rainLog = open(rainHourlyFile, 'a')
	rainLogEntry = (str(prevHour) + "00" + " " + "{:.2f}".format(prainHourly) + "\n")
	rainLog.write(rainLogEntry)
	prevRain = dataArray[0]

def dayChange():
	global today
	global tomorrow
	global rainTotal
	global lowTemp
	global highTemp
	''' Append new date stamp to rain hourly file'''
	rainLog = open(rainHourlyFile, 'a')
	rainLogEntry = (str(tomorrow) + "\n")
	rainLog.write(rainLogEntry)
	
	'''Add daily entry to daily rain total file'''
	dailylogEntry = (str(today) + " " + "{:.2f}".format(rainTotal) + "\n")
	rainDailyLog = open(rainDailyFile, 'a')
	rainDailyLog.write(dailylogEntry)
	
	'''Add entry to daily high/low temps file'''
	dailyTempsEntry = (str(today) + " " + str(highTemp) + "/" + str(lowTemp) + "\n")
	tempsDailyLog = open(highLowDailyFile, 'a')
	tempsDailyLog.write(dailyTempsEntry)
	
	'''Reset daily high and low temps'''
	lowTemp = float(outTempFahr)
	highTemp = float(outTempFahr)
	
def tempHi(poutTempFahr):
	global highTemp
	if (poutTempFahr > highTemp):
		highTemp = poutTempFahr
		
def tempLow(poutTempFahr):	
	global lowTemp
	if (poutTempFahr < lowTemp):
		lowTemp = poutTempFahr
	

'''--------Request WeatherBoard Data---------'''
port = "/dev/ttyACM0"
#fullDataRequest = "http://192.168.1.130/FullDataString"
#logFile = "/home/pi/python/weather/current.txt"			#enter name of logfile
logFile = "/media/pi/24F0-C445/python/weather/current.txt"
rawFile = "/media/pi/24F0-C445/python/weather/raw.json"
rainHourlyFile = "/media/pi/24F0-C445/python/weather/hourlyrain.txt"
rainDailyFile = "/media/pi/24F0-C445/python/weather/dailyrain.txt"
highLowDailyFile = "/media/pi/24F0-C445/python/weather/dailytemps.txt"
MBTOINCH = 0.00029530
ELEV = 0.0
ELEVMB = 146.555
prevRain = 0.0

'''Inititalize rainHourlyArray'''
rainHourlyArray =[0.0]*24

'''Initialize serial connection'''
ser = serial.Serial(port, 57600, timeout=.2)
ser.flushInput()
ser.flushOutput()
serBuffer = ''
nextChar = ''

'''Get serial data if it's available'''
while True:
	#processTime()
	
	if (ser.inWaiting() >= 0):
		serBuffer = ser.readline()
		bytesToRead = ser.inWaiting()
	
	dataArray = serBuffer.split(",")
	length = len(dataArray)
	
	if (length > 10):
		header = "-----------------------------------------"
		print (header)
		print ""
		title = "K7BBR Weather Station, Woods Cross, Utah"
		print (title)
		print ""
		timestamp = time.asctime(time.gmtime())
		currentTime =  ("Time: " + (timestamp))
		print (currentTime)
		processTime()
		global outTempFahr
		outTempCel = str(dataArray[5])
		outTempFahr = float (dataArray[5])
		outTempFahr = (outTempFahr * 9 / 5 ) + 32
		outTempFahr = ("{:.2f}".format(outTempFahr))
		outTemp = ("Outdoor Temperature: " + dataArray[5] + " (" +str(outTempFahr) + " deg F)")
		print (outTemp)
		#print ("{:.2f}".format(outTempFahr))
		humid = ("RH: " +dataArray[4] + "%")      
		print (humid)
		global inTempCel
		inTempCel = str(dataArray[6])
		inTempFahr = float(dataArray[6])
		inTempFahr = (inTempFahr * 9 / 5) +32
		inTempStr = ("{:.2f}".format(inTempFahr))
		inTemp = ("Indoor Temperature: " + dataArray[6] + " (" +(inTempStr) + " deg F)")
		print (inTemp)
		baroInch = dataArray[7]
		baroInchRaw = str(dataArray[7])
		#baroInch = (baroFloat * MBTOINCH)
		#baroInchStr = "{:.2f}".format(baroInch)
		
		baro = ("Barometric Pressure: " + dataArray[7])
		
		print (baro)
		windDir = ("Wind Dir: " + dataArray[3])
		windDirRaw = str(dataArray[3])
		print (windDir)
		windSpeed = ("Wind Speed: " + dataArray[1])
		windSpeedRaw=str(dataArray[1])
		print (windSpeed)
		windGust = ("Wind Gust: " + dataArray[2])
		windGustRaw=str(dataArray[2])
		print (windGust)
		rainHourly = (float(dataArray[0]) - float(prevRain))
		rainHourlyArray[currentHour] = rainHourly
		rainHourlyRaw= str("{:.2f}".format(rainHourly))
		rain = ("Hourly Rain Total: " + ("{:.2f}".format(rainHourly)))
		print (rain)
		rainTotal = sum(rainHourlyArray)
		dayRainRaw = str(rainTotal)
		dayrain = ("24 Hour Rain Total " + ("{:.2f}".format(rainTotal)))
		print (dayrain)
		tempHi(float(outTempFahr))
		#tempHiRaw = str(tempHi)
		tempLow(float(outTempFahr))
		#tempLowRaw = str(tempLow)
		highLow = ("Today's High/Low Temperatures(in deg F): " + ("{:.2f}".format(highTemp)) + "/" + ("{:.2f}".format(lowTemp)))
		highLowRaw = ("High/Low (in deg F): " + ("{:.2f}".format(highTemp)) + "/" + ("{:.2f}".format(lowTemp)))
		tempHighRaw = ("{:.2f}".format(highTemp))
		tempLowRaw = ("{:.2f}".format(lowTemp))
		print (highLow)
		print ""
		print (header)
		
		log = open(logFile, 'w')
		logentry = (header + "\n" + "\n" +
					title + "\n" + "\n" +
					currentTime + "\n" +
					outTemp + "\n" +
					humid + "\n" +
					inTemp + "\n" +
					baro + "\n" +
					windDir + "\n" +
					windSpeed + "\n" +
					windGust + "\n" +
					rain + "\n" +
					dayrain + "\n" +
					highLow + "\n" + "\n" +
					header)				    
		log.write(logentry)
		'''------------------------------------Create JSON file-----------------------'''
		raw = open(rawFile, 'w')
		rawentry = 	("{" + "\"time\"" + ":" + "\"" + currentTime + "\"" + "," +
					"\"dir\"" + ":" + "\"" + windDirRaw + "\"" + "," +
					"\"speed\"" + ":" + "\"" + windSpeedRaw + "\"" + "," +
					"\"gust\"" + ":" + "\"" +windGustRaw + "\"" + "," +
					"\"otempc\"" + ":" + "\"" +outTempCel + "\"" + "," +
					"\"otempf\"" + ":" + "\"" + str(outTempFahr) + "\"" + "," +
					"\"humid\"" + ":" + "\"" +humid + "\"" + "," +
					"\"itempc\"" + ":" + "\"" +inTempCel + "\"" + "," +
					"\"itempf\"" + ":" + "\"" +inTempStr + "\"" + "," +
					"\"pressure\"" + ":" + "\"" +baroInchRaw + "\"" + "," +
					"\"hrain\"" + ":" + "\"" +rainHourlyRaw+ "\"" + "," +
					"\"dayrain\"" + ":" + "\"" +dayRainRaw + "\"" + "," +
					"\"high\"" + ":" + "\"" +tempHighRaw + "\"" + "," +
					"\"low\"" + ":" + "\"" +tempLowRaw + "\"" "}")
		raw.write(rawentry)
		print (rawentry)
		
		'''-------------------------------------Check Time Passage---------------------'''		
		#hourCheck = 0 # for testing
		if ((hourCheck) != (currentHour)):
			processYesterday()
			hourChange(rainHourly)
			hourCheck = currentHour
		#dayCheck = 1 #for testing
		if ((dayCheck) != (dayOfMonth)):
			dayChange()
			dayCheck = dayOfMonth
			
		
		#nextChar = ''
		#serBuffer = ''
		
		time.sleep(2)
