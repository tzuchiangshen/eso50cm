#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
import math
from AstroUtil import *
import os
import optparse
from DeviceManager import DeviceManager

if __name__ == "__main__":

    parser = optparse.OptionParser(usage= "%prog")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()

    mgr.connect()

    encData = OUC.RawEncoderData()
    encData.lectAlphaAxisE = mgr.readDeviceMemory(0xA8,2,0)
    encData.posAlphaAxisE  = mgr.readDeviceMemory(0xA8,4,0)
    encData.lectAlphaWormE = mgr.readDeviceMemory(0xA6,2,0)
    encData.posAlphaWormE  = mgr.readDeviceMemory(0xA6,4,0)
    encData.lectAlphaMotor = mgr.readDeviceMemory(0xA2,2,0)
    encData.posAlphaMotor  = mgr.readDeviceMemory(0xA2,4,0)
    encData.lectDeltaAxisE = mgr.readDeviceMemory(0xAC,2,0) 
    encData.posDeltaAxisE  = mgr.readDeviceMemory(0xAC,4,0) 
    encData.lectDeltaWormE = mgr.readDeviceMemory(0xAA,2,0)
    encData.posDeltaWormE  = mgr.readDeviceMemory(0xAA,4,0) 
    encData.lectDeltaMotor = mgr.readDeviceMemory(0xA4,2,0) 
    encData.posDeltaMotor  = mgr.readDeviceMemory(0xA4,4,0)

    print "                   Lecture     Position "
    print "Alpha AxisE   = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaAxisE, encData.posAlphaAxisE)
    print                  "Alpha WormE   = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaWormE, encData.posAlphaWormE) 
    print                  "Alpha MotorE  = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaMotor, encData.posAlphaMotor)
    print "Delta AxisE   = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaAxisE, encData.posDeltaAxisE)
    print                  "Delta WormE   = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaWormE, encData.posDeltaWormE) 
    print                  "Delta MotorE  = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaMotor, encData.posDeltaMotor)

    mgr.disconnect()
    
