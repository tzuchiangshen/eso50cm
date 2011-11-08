#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
from AstroUtil import *
import os
import optparse 


#global variables
global status 
global ic 
global obsImpl 



def connect():
    global status
    global ic
    global obsImpl

    #print "Connecting.."
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
        #print proxy
        obj = ic.stringToProxy(proxy);
        obsImpl = OUC.ObservingPrx.checkedCast(obj)
        #print "Connected to ObsControl"
        if not obsImpl: 
            raise RuntimeError("Invalid proxy")
    except:
        traceback.print_exc()
        status = 1
        sys.exit(status)

def disconnect():
    global status
    #print "Desconnecting.."
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

def setConfiguration():
    try:
        telescope = obsImpl.getTelescope()
        introot_path = ""
        introot_path = os.getenv("SWROOT")
        if(introot_path == ""):
            introot_path = "/eso50cm/SWROOT"        

        conf_path = "%s/%s" % (introot_path, "/config/ESO50cm.conf")
        telescope.setConfiguration(conf_path)
        print "Configuration send to LCUControl"
    except:
        print "Problems trying to configure telescope!!"
        traceback.print_exc()
        status = 1

def getConfiguration():
    telConfigData = OUC.TelescopeConfigData()
    try:
        telescope = obsImpl.getTelescope()
        telConfigData = telescope.getConfiguration()   
        print "LT = [%lf]" % telConfigData.localTime
        print datetime.utcfromtimestamp(telConfigData.localTime)
        print "Latitude = %+11.4lf " % telConfigData.latitude
        print "Longitude = %+11.4lf " % telConfigData.longitude
        print "Altitude = %+11.4lf " % telConfigData.altitude
        print "AMT = %+11.4lf " % telConfigData.AMT
        print "AMH = %+11.4lf " % telConfigData.AMH
        print "AMR = %+11.4lf " % telConfigData.AMR
        print "AWT = %+11.4lf " % telConfigData.AWT
        print "AWH = %+11.4lf " % telConfigData.AWH
        print "AWR = %+11.4lf " % telConfigData.AWR
        print "AAT = %+11.4lf " % telConfigData.AAT
        print "AAH = %+11.4lf " % telConfigData.AAH
        print "AAR = %+11.4lf " % telConfigData.AAR
        print "DMT = %+11.4lf " % telConfigData.DMT
        print "DMH = %+11.4lf " % telConfigData.DMH
        print "DMR = %+11.4lf " % telConfigData.DMR
        print "DWT = %+11.4lf " % telConfigData.DWT
        print "DWH = %+11.4lf " % telConfigData.DWH
        print "DWR = %+11.4lf " % telConfigData.DWR
        print "DAT = %+11.4lf " % telConfigData.DAT
        print "DAH = %+11.4lf " % telConfigData.DAH
        print "DAR = %+11.4lf " % telConfigData.DAR
        print "Generated at = [%lf]" % telConfigData.lcuTime
        print datetime.utcfromtimestamp(telConfigData.lcuTime)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1  

def getEncoderPosition():
    encData = OUC.EncoderData()
    try:
        telescope = obsImpl.getTelescope()
        encData =  telescope.getEncodersPosition();
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
        telescope = obsImpl.getTelescope()
        telData = telescope.getPosition()
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



if __name__ == "__main__":
    parser = optparse.OptionParser(usage= "%prog -c command")
    parser.add_option("-c", "--command", dest="command", default = None, 
                        type = "string", help="send command to the telescope")

    (options, args) = parser.parse_args()

    print options
    print args

    connect()
    command = options.command
    if (command == "set_config"):
        setConfiguration()
    elif (command == "get_config"):
        getConfiguration()
    elif (command == "get_position"):
        getPosition()
    elif (command == "read_encoders"):
        getEncoderPosition()

    disconnect()

