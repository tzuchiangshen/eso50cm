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

    AlphaMotorRemaining = mgr.readDeviceMemory(0xA2,7,0)
    AlphaMotorCurrent = mgr.readDeviceMemory(0xA2,2,0)
    DeltaMotorRemaining = mgr.readDeviceMemory(0xA4,7,0) 
    DeltaMotorCurrent  = mgr.readDeviceMemory(0xA4,2,0)

    print "                   Remaining     Current " 
    print "Alpha MotorE  = [%+8.0lf     %+8.0lf ]" % (AlphaMotorRemaining, AlphaMotorCurrent)
    print "Delta MotorE  = [%+8.0lf     %+8.0lf ]" % (DeltaMotorRemaining, DeltaMotorCurrent)

    mgr.disconnect()
    
