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

def sayHello():
    try:
        lcuImpl.sayHello(200)
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
         print "Generated at = [%lf]\n" % rawEncData.lcuTime
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

def getConfiguration():
    telConfigData = OUC.TelescopeConfigData()
    print "antes \n %s " % telConfigData
    try:
        telConfigData = lcuImpl.getConfiguration()   
        print telConfigData
        print "LT = [%lf]\n" % telConfigData.localTime
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
        print "Generated at = [%lf]\n" % telConfigData.lcuTime
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
        introot_path = ""
        introot_path = os.getenv("SWROOT")
        if(introot_path == ""):
            introot_path = "/eso50cm/SWROOT"        

        conf_path = "%s/%s" % (introot_path, "/config/ESO50cm.conf")
        lcuImpl.setConfiguration(conf_path)
        print "Configuration send to LCUControl"
    except:
        print "Problems trying to configure telescope!!"
        traceback.print_exc()
        status = 1

def setTracking(trkState=True, velocity=602):
    trkInfo = OUC.TrackingInfo()
    try:
        trkInfo.trackState = trkState
        trkInfo.ticVel = velocity
        lcuImpl.setTracking(trkInfo)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def setTarget(Ra, Dec):
    targetPos = OUC.TelescopePosition()
    targetPos.RA = HHMMSS2degs(Ra[0], Ra[1], Ra[2])
    targetPos.Dec = DDMMSS2degs(Dec[0], Dec[1], Dec[2])
    try:
        lcuImpl.setTarget(targetPos)
    except OUC.TargetOutOfLimitsEx():
        print "Target Out of Limits. Try a new one\n";
        traceback.print_exc()
        status =1  
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status =1

def moveToTarget():
    try:
        lcuImpl.moveToTarget()
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def stopTelescope():
    try:
        lcuImpl.stopTelescope(OUC.TelescopeDirection.North)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def parkTelescope():
    try:
        lcuImpl.parkTelescope()
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def readDeviceMemory(deviceId, address, value):
    try:
        return lcuImpl.readDeviceMemory(deviceId, address, value)
    except:
        traceback.print_exc()
        status = 1

def setDeviceMemory(deviceId, address, value):
    try:
        return lcuImpl.setDeviceMemory(deviceId, address, value)
    except:
        traceback.print_exc()
        statur = 1
        
if __name__ == "__main__":
    ra = [12,0,0]
    dec = [40,0,0]
    connect()
    #setConfiguration()
    print "#######################################"
    #if isConfigured():
    #   getConfiguration()
    #sayHello()
    #getEncoderPosition()
    #parkTelescope()
    #moveToTarget()
    #setTarget(ra, dec)
    #moveToTarget()
    #stopTelescope()
    #setTracking()
    #print "#######################################"    
    getRawEncoderPosition()
    #print "#######################################"
    #getPosition()
    #readDeviceMemory(0xA8, 4, 0)
    disconnect()
    
