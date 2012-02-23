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
        self.telConfigData = None
         

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


    def getPosition(self):
        telData = OUC.TelescopeData()
        try:
            telData = self.lcuImpl.getPosition()
            return telData
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1


    def getConfiguration(self):
        self.telConfigData = OUC.TelescopeConfigData()
        try:
            self.telConfigData = self.lcuImpl.getConfiguration()   
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1  
    

    def convert_axis_to_motor_enc(self, axis, axis_enc): 
        if (self.telConfigData == None):
            self.getConfiguration()

        if (axis.upper() == "ALPHA"):
            AMR = float(self.telConfigData.AMR)
            AMT = float(self.telConfigData.AMT)
            AAR = float(self.telConfigData.AAR)
            AAT = float(self.telConfigData.AAT)
            return (axis_enc * AMR * AMT / AAT / AAR)
        elif (axis.upper == "DELTA"): 
            DMR = float(self.telConfigData.DMR)
            DMT = float(self.telConfigData.DMT)
            DAR = float(self.telConfigData.DAR)
            DAT = float(self.telConfigData.DAT)
            return (axis_enc * DMR * DMT / DAT / DAR)
        else: 
            raise "axis=%s doen't exist" % axis
    
    def convert_worm_to_motor_enc(self, axis, worm_enc): 
        if (self.telConfigData == None):
            self.getConfiguration()

        if (axis.upper() == "ALPHA"):
            AMR = float(self.telConfigData.AMR)
            AMT = float(self.telConfigData.AMT)
            AWR = float(self.telConfigData.AWR)
            AWT = float(self.telConfigData.AWT)
            return (worm_enc * AMR * AMT / AWT / AWR)
        elif (axis.upper == "DELTA"): 
            DMR = float(self.telConfigData.DMR)
            DMT = float(self.telConfigData.DMT)
            DWR = float(self.telConfigData.DWR)
            DWT = float(self.telConfigData.DWT)
            return (axis_enc * DMR * DMT / DWT / DWR)
        else: 
            raise "axis=%s doen't exist" % axis
 



if __name__ == "__main__":

    parser = optparse.OptionParser(usage= "%prog")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()
    mgr.connect()
    print "0xAC, addr=4 val=%d" % mgr.readDeviceMemory(0xAC, 4, 0)
    mgr.disconnect()
    
