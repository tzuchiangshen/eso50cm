#!/usr/bin/python 
import sys, traceback, Ice
import OUC
from datetime import datetime
import time
from AstroUtil import *
import os

#global variables
global status
global ic
global lcuImpl


def connect():
        global status
        global ic
        global lcuImpl

        print "Connecting.."
        status = 0
        try:
           # Reading configuration info 
           configPath = os.environ.get("SWROOT")
           configPath = configPath + "/config/LCU-config"
           initData = Ice.InitializationData()
           initData.properties = Ice.createProperties()
           initData.properties.load(configPath)
           ic = Ice.initialize(sys.argv, initData)

           # Create proxy
           properties = ic.getProperties();
           proxyProperty = "LCUAdapter.Proxy"
           proxy = properties.getProperty(proxyProperty);
           print proxy
           obj = ic.stringToProxy(proxy);
           lcuImpl = OUC.LCUPrx.checkedCast(obj)
           print "Connected to LCUControl"
           if not lcuImpl:
                   raise RuntimeError("Invalid proxy")
        except:
           traceback.print_exc()
           status = 1
           sys.exit(status)

def disconnect():
        global status
        print "Desconnecting.."
        if ic:
             try:
                ic.destroy()
             except:
                traceback.print_exc()
                status = 1
        sys.exit(status)


def getEncoderPosition():
        encData = OUC.EncoderData()
        try:
           encData =  lcuImpl.getEncodersPosition();
           print "LT = [%lf]" % encData.localTime
           print datetime.utcfromtimestamp(encData.localTime)
           print "AW Position  = [%+10.0lf]" % encData.alphaWormE
           print "AA Position  = [%+10.0lf]" % encData.alphaAxisE
           print "DW Position  = [%+10.0lf]" % encData.deltaWormE
           print "AA Position  = [%+10.0lf]" % encData.deltaAxisE
           print "Generated at = [%lf]" % encData.lcuTime
           print datetime.utcfromtimestamp(encData.lcuTime)
        except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
           status = 1

def getPosition():
        telData = OUC.TelescopeData()
        try:
                telData = lcuImpl.getPosition()
                print "LT = [%lf]" % telData.localTime
                print datetime.utcfromtimestamp(telData.localTime)
                print "Time elapsed since last access: %lf" % telData.deltaT
                print "JD  = %lf" % telData.julianDate
                print "Latitude = %+11.4lf" % telData.latitude
                print "Longitude = %+11.4lf" % telData.longitude
                print "Altitude = %+11.4lf" % telData.altitude
                print "High Elevation = %+11.4lf" % telData.highElevation
                print "Low Elevation = %+11.4lf" % telData.lowElevation
                format =  degs2HHMMSS(telData.currentPos.localSideralTime / 15.0)
                print "LST = %02d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.currentPos.RA / 15.0)
                print "Current RA = %02d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.currentPos.Dec)
                print "Current Dec = %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.currentPos.HA / 15.0)
                print "Current HA = %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
                print "Current Alt = %lf" % telData.currentPos.Alt
                print "Current Az = %lf" % telData.currentPos.Az
                format =  degs2HHMMSS(telData.targetPos.RA / 15.0)
                print "Target RA =  %02d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.targetPos.Dec)
                print "Target Dec = %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.targetPos.HA / 15.0)
                print "Target HA = %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
                print "Target Alt = %lf" % telData.targetPos.Alt
                print "Target Az = %lf" % telData.targetPos.Az
                format =  degs2HHMMSS(telData.differencePos.RA / 15.0)
                print "Difference RA = %02d:%02d:%02.0lf" % (format[0],format[1],format[2])
                format =  degs2HHMMSS(telData.differencePos.Dec)
                print "Difference Dec = %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
                print "Generated at = [%lf]" % telData.lcuTime
                print datetime.utcfromtimestamp(telData.lcuTime)
        except OUC.TelescopeNotConfiguredEx():
                print "Telescope Not Configured !!!"
                traceback.print_exc()
                status = 1

def getRawEncoderPosition():
        rawEncData = OUC.RawEncoderData()
        try:
           rawEncData =  lcuImpl.getRawEncodersPosition();
           print "Time elapsed since last access: %lf" % rawEncData.deltaT
           print "Lecture Alpha AxisE: %6d" % rawEncData.lectAlphaAxisE
           print "Position Alpha AxisE: %11d" % rawEncData.posAlphaAxisE
           print "Lecture Alpha WormE: %6d" % rawEncData.lectAlphaWormE
           print "Position Alpha WormE: %11d" % rawEncData.posAlphaWormE
           print "Lecture Alpha Motor: %6d" % rawEncData.lectAlphaMotor
           print "Position Alpha Motor: %11d" % rawEncData.posAlphaMotor
           print "Lecture Delta AxisE: %6d" % rawEncData.lectDeltaAxisE
           print "Position Delta AxisE: %11d" % rawEncData.posDeltaAxisE
           print "Lecture Delta WormE: %6d" % rawEncData.lectDeltaWormE
           print "Position Delta WormE: %11d" % rawEncData.posDeltaWormE
           print "Lecture Delta Motor: %6d" % rawEncData.lectDeltaMotor
           print "Position Delta Motor: %11d" % rawEncData.posDeltaMotor
           print "Generated at = [%lf]" % rawEncData.lcuTime
           print datetime.utcfromtimestamp(rawEncData.lcuTime)
        except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
           status = 1



if __name__ == "__main__":
	connect()
	try:
           while(True):  
		   print "#######################################"
		   #getRawEncoderPosition()
		   print "#######################################"
                   getEncoderPosition()
		   print "#######################################"
                   getPosition()
	           time.sleep(1)                
        except KeyboardInterrupt:
                print "^C pressed!"
		print "Trying to disconnect..."
                disconnect()
