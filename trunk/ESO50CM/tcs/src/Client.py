#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
from AstroUtil import *

#global variables
global status 
global ic 
global lcuImpl 

def hexStrEndianSwap(theString):
    """Rearranges character-couples in a little endian hex string to
    convert it into a big endian hex string and vice-versa. i.e. 'A3F2'
    is converted to 'F2A3'

    @param theString: The string to swap character-couples in
    @return: A hex string with swapped character-couples. -1 on error."""

    # We can't swap character couples in a string that has an odd number
    # of characters.
    if len(theString)%2 != 0:
        return -1

    # Swap the couples
    swapList = []
    for i in range(0, len(theString), 2):
        swapList.insert(0, theString[i:i+2])

    # Combine everything into one string. Don't use a delimeter.
    return ''.join(swapList)


def connect():
	global status
        global ic
        global lcuImpl

	print "Connecting.."
	status = 0
        try:
           ic = Ice.initialize(sys.argv)
           obj = ic.stringToProxy("LCU:tcp -h 192.168.0.10 -p 10000");
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
	   print datetime.utcfromtimestamp(rawEncData.lcuTime)
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
	   status = 1

def getEncoderPosition():
	encData = OUC.EncoderData()
	try:
           encData =  lcuImpl.getEncodersPosition();
	   print "LT = [%lf]\n" % encData.localTime
           print datetime.utcfromtimestamp(encData.localTime)
	   print "AW Position  = [%+10.0lf]\n" % encData.alphaWormE
	   print "AA Position  = [%+10.0lf]\n" % encData.alphaAxisE 
	   print "DW Position  = [%+10.0lf]\n" % encData.deltaWormE
	   print "AA Position  = [%+10.0lf]\n" % encData.deltaAxisE
	   print "Generated at = [%lf]\n" % encData.lcuTime
	   print datetime.utcfromtimestamp(encData.lcuTime)
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
           traceback.print_exc()
	   status = 1

def getPosition():
	telData = OUC.TelescopeData()
	try:
           telData = lcuImpl.getPosition()
	   print "LT = [%ld]\n" % telData.localTime
	   print datetime.utcfromtimestamp(telData.localTime)
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
	   print datetime.utcfromtimestamp(telData.lcuTime)
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
	   traceback.print_exc()
	   status = 1

def getConfiguration():
	telConfigData = OUC.TelescopeConfigData()
        print "antes \n %s " % telConfigData
	try:
           telConfigData = lcuImpl.getConfiguration()	
	   print telConfigData
	   print "LT = [%ld]\n" % telConfigData.localTime
           print datetime.utcfromtimestamp(telConfigData.localTime)
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
	   print "Generated at = [%ld]\n" % telConfigData.lcuTime
           print datetime.utcfromtimestamp(telConfigData.lcuTime)
	except OUC.TelescopeNotConfiguredEx():
           print "Telescope Not Configured !!!"
	   traceback.print_exc()
	   status = 1  

def isConfigured():
	print"Telescope Configuration state %d" % lcuImpl.isConfigured() 
	return lcuImpl.isConfigured()

def setConfiguration():
	try:
           lcuImpl.setConfiguration("./ESO50cm.conf")
	   print "Configuration send to LCUControl"
	except:
           print "Problems trying to configure telescope!!"
	   traceback.print_exc()
	   status = 1


if __name__ == "__main__":
      	connect()
	setConfiguration()
	if isConfigured():
		getConfiguration()
        sayHello()
	getEncoderPosition()
	getRawEncoderPosition()
	getPosition()
	disconnect()
	
