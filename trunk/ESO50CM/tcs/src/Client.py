#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
import time

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
	except:
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
	   print "LST = %lf \n" %  telData.localSideralTime
	   print "Current RA = %lf \n" % telData.currentPos.RA
	   print "Current Dec = %lf \n" % telData.currentPos.Dec
	   print "Current HA = %lf \n" % telData.currentPos.Dec
	   print "Current Alt = %lf \n" % telData.currentPos.Alt
	   print "Current Az = %lf \n" % telData.currentPos.Az
	   print "Target RA = %lf \n" % telData.targetPos.RA
	   print "Target Dec = %lf \n" % telData.targetPos.Dec
	   print "Target HA = %lf \n" % telData.targetPos.Dec
	   print "Target Alt = %lf \n" % telData.targetPos.Alt
	   print "Target Az = %lf \n" % telData.targetPos.Az
	   print "Generated at = [%ld]\n" % telData.lcuTime
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

bool isConfigured():
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
	#getPosition()
	setConfiguration()
	if isConfigured():
		getConfiguration()
	disconnect()
	
