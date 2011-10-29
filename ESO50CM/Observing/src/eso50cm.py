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
    disconnect()

