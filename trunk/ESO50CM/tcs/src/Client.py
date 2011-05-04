#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
import time
from AstroUtil import *

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
           ic = Ice.initialize(sys.argv)
           obj = ic.stringToProxy("LCU:tcp -p 10000");
           lcuImpl = OUC.LCUPrx.checkedCast(obj)
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

def sayHello():
	try:
	   lcuImpl.sayHello(3)
	   print "I said Hello!!"
        except:
           traceback.print_exc()
	   status = 1


def getRawEncoderPosition():
	rawEncData = OUC.RawEncoderData()
	try:
	   rawEncData =  lcuImpl.getRawEncodersPosition();
	   print "Time elapsed since last access: %lf\n" % rawEncData.deltaT
	   print "Lecture Alpha AxisE: %6d\n" % rawEncData.lectAlphaAxisE
	   print "Position Alpha AxisE: %11d\n" % rawEncData.posAlphaAxisE
	   print "Lecture Alpha WormE: %6d\n" % rawEncData.lectAlphaWormE
	   print "Position Alpha WormE: %11d\n" % rawEncData.posAlphaWormE
	   print "Lecture Alpha Motor: %6d\n" % rawEncData.lectAlphaMotor
	   print "Position Alpha Motor: %11d\n" % rawEncData.posAlphaMotor
	   print "Lecture Delta AxisE: %6d\n" % rawEncData.lectDeltaAxisE
	   print "Position Delta AxisE: %11d\n" % rawEncData.posDeltaAxisE
	   print "Lecture Delta WormE: %6d\n" % rawEncData.lectDeltaWormE
	   print "Position Delta WormE: %11d\n" % rawEncData.posDeltaWormE
	   print "Lecture Delta Motor: %6d\n" % rawEncData.lectDeltaMotor
	   print "Position Delta Motor: %11d\n" % rawEncData.posDeltaMotor 
	   print "Generated at = [%ld]\n" % rawEncData.lcuTime
	   print time.strftime("Generated at = %b %d %Y %H:%M:%S \n", time.gmtime(rawEncData.lcuTime))
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
	   status = 1

def getEncoderPosition():
	encData = OUC.EncoderData()
	try:
           encData =  lcuImpl.getEncodersPosition();
	   print "LT = [%ld]\n" % encData.localTime
           print time.strftime("LT = %b %d %Y %H:%M:%S \n", time.gmtime(encData.localTime))
	   print "AW Position  = [%+10.0lf]\n" % encData.alphaWormE
	   print "AA Position  = [%+10.0lf]\n" % encData.alphaAxisE 
	   print "DW Position  = [%+10.0lf]\n" % encData.deltaWormE
	   print "AA Position  = [%+10.0lf]\n" % encData.deltaAxisE
	   print "Generated at = [%ld]\n" % encData.lcuTime
	   print time.strftime("Generated = %b %d %Y %H:%M:%S \n", time.gmtime(encData.lcuTime))
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
	   status = 1

def getPosition():
	telData = OUC.TelescopeData()
	try:
           telData = lcuImpl.getPosition()
	   print "LT = [%ld]\n" % telData.localTime
	   print time.strftime("LT = %b %d %Y %H:%M:%S \n", time.gmtime(telData.localTime))
	   print "Time elapsed since last access: %lf\n" % telData.deltaT  
	   print "JD  = %lf\n" % telData.julianDate
	   print "Latitude = %+11.4lf \n" % telData.latitude
	   print "Longitude = %+11.4lf \n" % telData.longitude
	   print "Altitude = %+11.4lf \n" % telData.altitude
	   print "High Elevation = %+11.4lf \n" % telData.highElevation
	   print "Low Elevation = %+11.4lf \n" % telData.lowElevation
	   format =  degs2HHMMSS(telData.localSideralTime / 15.0)
	   print "LST: %02d:%02d:%02.0lf\n" % (format[0],format[1],format[2])  
	   format =  degs2HHMMSS(telData.currentPos.RA / 15.0)
	   print "Current RA = %02d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   format =  degs2HHMMSS(telData.currentPos.Dec)
	   print "Current Dec = %+03d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   format =  degs2HHMMSS(telData.currentPos.HA / 15.0)
	   print "Current HA = %+03d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   print "Current Alt = %lf \n" % telData.currentPos.Alt
	   print "Current Az = %lf \n" % telData.currentPos.Az
	   format =  degs2HHMMSS(telData.targetPos.RA / 15.0)
	   print "Target RA =  %02d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   format =  degs2HHMMSS(telData.targetPos.Dec)
	   print "Target Dec = %+03d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   format =  degs2HHMMSS(telData.targetPos.HA / 15.0)
	   print "Target HA = %+03d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   print "Target Alt = %lf \n" % telData.targetPos.Alt
	   print "Target Az = %lf \n" % telData.targetPos.Az
	   format =  degs2HHMMSS(telData.differencePos.RA / 15.0)
	   print "Difference RA = %02d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   format =  degs2HHMMSS(telData.differencePos.Dec)
	   print "Difference Dec = %+03d:%02d:%02.0lf\n" % (format[0],format[1],format[2])
	   print "Generated at = [%ld]\n" % telData.lcuTime
	   print time.strftime("Generated at = %b %d %Y %H:%M:%S \n", time.gmtime(telData.lcuTime))
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
	   traceback.print_exc()
	   status = 1

def getConfiguration():
	telConfigData = OUC.TelescopeConfigData()
	try:
           telConfigData = lcuImpl.getConfiguration()	
	   print "LT = [%ld]\n" % telConfigData.localTime
	   print "Latitude = %+11.4lf \n" % telConfigData.latitude
	   print "Longitude = %+11.4lf \n" % telConfigData.longitude
	   print "Altitude = %+11.4lf \n" % telConfigData.altitude
	   print "AMT = %+11.4lf \n" % telConfigData.AMT
	   print "AMH = %+11.4lf \n" % telConfigData.AMH
	   print "AMR = %+11.4lf \n" % telConfigData.AMR
	   print "AWT = %+11.4lf \n" % telConfigData.AWT
	   print "AWH = %+11.4lf \n" % telConfigData.AWH
	   print "AWR = %+11.4lf \n" % telConfigData.AWR
	   print "AAT = %+11.4lf \n" % telConfigData.AAT
	   print "AAH = %+11.4lf \n" % telConfigData.AAH
	   print "AAR = %+11.4lf \n" % telConfigData.AAR
	   print "DMT = %+11.4lf \n" % telConfigData.DMT
	   print "DMH = %+11.4lf \n" % telConfigData.DMH
	   print "DMR = %+11.4lf \n" % telConfigData.DMR
	   print "DWT = %+11.4lf \n" % telConfigData.DWT
	   print "DWH = %+11.4lf \n" % telConfigData.DWH
	   print "DWR = %+11.4lf \n" % telConfigData.DWR
	   print "DAT = %+11.4lf \n" % telConfigData.DAT
	   print "DAH = %+11.4lf \n" % telConfigData.DAH
	   print "DAR = %+11.4lf \n" % telConfigData.DAR
	   print "Generated at = [%ld]\n" % telData.lcuTime
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
	   traceback.print_exc()
	   status = 1  

def isConfigured():
	print"Telescope Configuration state %d" % lcuImpl.isConfigured() 
	return lcuImpl.isConfigured()

def setConfiguration():
	try:
           lcuImpl.setConfiguration("ESO50cm.conf")
	except:
           print "Problems trying to configure telescope!!"
	   traceback.print_exc()
	   status = 1


if __name__ == "__main__":
      	connect()
        #sayHello()
	#getEncoderPosition()
	#getRawEncoderPosition()
	#getPosition()
	setConfiguration()
	if isConfigured():
		getConfiguration()
	disconnect()
	
