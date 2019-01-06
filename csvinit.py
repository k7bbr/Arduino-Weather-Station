#!/usr/bin/python
#Filename: csvinit.py
#A function to get values from a CSV file and put them into a python dictionary:
import csv

def initCSV(pcsvFile):
    try:
        csvFileHandle = csv.reader(open(pcsvFile, 'r',))	#open csv file to get data for dictionary
        csvDict = {}					#initialize dictionary
        for row in csvFileHandle:			#read values into dict, key in first column, value in second column
            k, v = row
            csvDict[k] = v
        return csvDict
    except:						#if file not found set up blank dictionary for error handling
        csvDict ={}
        return csvDict
