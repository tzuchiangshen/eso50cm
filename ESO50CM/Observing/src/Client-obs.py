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


if __name__ == "__main__":
    connect()
    sayHello()
    sayHelloTelescope()
    disconnect()

