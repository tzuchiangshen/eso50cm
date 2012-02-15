#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
import math
from AstroUtil import *
import os
import optparse

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

def offset_telescope(ra, dec):

        offsetPos = OUC.TelescopePosition()
        offsetPos.RA = sexagesimal2degs(ra, True)
        offsetPos.Dec = sexagesimal2degs(dec, False)
        print "setOffset ra=%f, dec=%f" % (offsetPos.RA, offsetPos.Dec)
        try:
            lcuImpl.setOffset(offsetPos)
        except OUC.TargetOutOfLimitsEx():
            print "Target Out of Limits. Try a new one\n";
            traceback.print_exc()
            status =1
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status =1
        except ValueError:
            print "wrong format"
 
        
if __name__ == "__main__":

    parser = optparse.OptionParser(usage= "%prog rad=00:00:00 dec=00:00:00")
    (options, args) = parser.parse_args()

    ra = "00:00:00"
    dec = "00:00:00"
    if len(args) == 0:
        print "wrong format"
    elif len(args) > 2:
        print "wrong format"
    else:
        for a in args:
            tmp = a.split("=")
            if(tmp[0].upper() == "RA"):
                ra = tmp[1]
            elif(tmp[0].upper() == "DEC"):
                dec = tmp[1]
        
    ra = sexagesimal2degs(ra, True)
    dec = sexagesimal2degs(dec, False)
    print "Degrees in the sky ra=%.5lf, dec=%.5lf"  % (ra, dec)

    # convert into encoders values
    ra = int(ra / 360.0 * 17280.0 * 6000.0)
    dec = int(dec / 360.0 * 13882.0 * 6000.0)
    print "Motor Encoders alpha=%d, delta=%d"  % (ra,dec)

    connect()
    quit = False
    quitAlpha = False
    quitDelta = False 

    #offset_telescope(ra, dec)
    setDeviceMemory(0xA2, 7, ra)
    setDeviceMemory(0xA4, 7, dec)
    time.sleep(1)
    posAlpha = readDeviceMemory(0xA2, 7, 0)
    posDelta = readDeviceMemory(0xA4, 7, 0)
    print "alpha = %d" % posAlpha
    print "delta = %d" % posDelta
    print ""

    while(not quit):
        #Alpha
        posAlpha = readDeviceMemory(0xA2, 7, 0)
        print "alpha = %d" % posAlpha
        if (math.fabs(posAlpha) < 50.0):
            #print "listo en alpha"
            quitAlpha = True

        #Delta
        posDelta = readDeviceMemory(0xA4, 7, 0)
        print "delta = %d" % posDelta
        if (math.fabs(posDelta) < 50.0): 
            #print "listo en delta"
            quitDelta = True

        if(quitAlpha and quitDelta): 
           quit = True
           #print " nos fuimos "

        time.sleep(3)
        print("")

    disconnect()
    
