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
global obsImpl 



def connect():
    global status
    global ic
    global obsImpl

    print "Connecting.."
    status = 0
    try:
        # Reading configuration info 
        configPath = os.environ.get("SWROOT")
        configPath = configPath + "/config/Obs-config" 
        initData = Ice.InitializationData()
        initData.properties = Ice.createProperties()
        initData.properties.load(configPath)
        ic = Ice.initialize(sys.argv, initData)

        # Create proxy
        properties = ic.getProperties();
        proxyProperty = "ObsAdapter.Proxy"
        proxy = properties.getProperty(proxyProperty);
        print proxy
        obj = ic.stringToProxy(proxy);
        obsImpl = OUC.ObservingPrx.checkedCast(obj)
        print "Connected to ObsControl"
        if not obsImpl: 
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
        obsImpl.sayHello(3) 
        print "I said Hello!!"
    except:
        traceback.print_exc()
        status = 1

def sayHelloTelescope():
    try:
        telescope = obsImpl.getTelescope()
        telescope.sayHelloTelescope(2) 
        print "I said Hello Telescope!!"
    except:
        traceback.print_exc()
        status = 1

def getRawEncoderPosition():
    rawEncData = OUC.RawEncoderData()
    try:
         telescope = obsImpl.getTelescope()
         rawEncData = telescope.getRawEncodersPosition() 
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
        telescope = obsImpl.getTelescope()
        encData =  telescope.getEncodersPosition();
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

def parkTelescope():
    try:
        telescope = obsImpl.getTelescope()
        telescope.parkTelescope()
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

if __name__ == "__main__":
    connect()
    #sayHello()
    sayHelloTelescope()
    getRawEncoderPosition()
    getEncoderPosition()
    parkTelescope()
    disconnect()

