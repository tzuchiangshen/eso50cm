#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
import math
from AstroUtil import *
import os
import optparse

class DeviceManager:
   
    def __init__(self):
        self.status = 0
        self.ic = None
        self.lcuImpl = None

    def connect(self):
        print "Connecting.."
        self.status = 0
        try:
            # Reading configuration info 
            configPath = os.environ.get("SWROOT")
            configPath = configPath + "/config/LCU-config"
            initData = Ice.InitializationData()
            initData.properties = Ice.createProperties()
            initData.properties.load(configPath)
            self.ic = Ice.initialize(sys.argv, initData)
    
            # Create proxy
            properties = self.ic.getProperties();
            proxyProperty = "LCUAdapter.Proxy"
            proxy = properties.getProperty(proxyProperty);
            print proxy
            obj = self.ic.stringToProxy(proxy);
            self.lcuImpl = OUC.LCUPrx.checkedCast(obj)
            print "Connected to LCUControl"
            if not self.lcuImpl:
                raise RuntimeError("Invalid proxy")
        except:
           traceback.print_exc()
           self.status = 1
           sys.exit(status)
    
    def disconnect(self):
        global status
        print "Desconnecting.."
        if self.ic:
            try: 
                self.ic.destroy()
            except:
                traceback.print_exc()
                self.status = 1
                sys.exit(status)
    
    def readDeviceMemory(self, deviceId, address, value):
        try:
            return self.lcuImpl.readDeviceMemory(deviceId, address, value)
        except:
            traceback.print_exc()
            self.status = 1
    
    def setDeviceMemory(self, deviceId, address, value):
        try:
            return self.lcuImpl.setDeviceMemory(deviceId, address, value)
        except:
            traceback.print_exc()
            self.status = 1

if __name__ == "__main__":

    parser = optparse.OptionParser(usage= "%prog")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()
    mgr.connect()
    print "0xAC, addr=4 val=%d" % mgr.readDeviceMemory(0xAC, 4, 0)
    mgr.disconnect()
    
