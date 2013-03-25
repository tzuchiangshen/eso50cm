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
    print args

    mgr = DeviceManager()

    mgr.connect()

    if(len(args) == 0):
        print "usage: set_tracking [on/off]"
        sys.exit()

    val = args[0]
    val = val.upper()

    if(val == 'ON'):
        print "start tracking"
        mgr.setDeviceMemory(0xA2, 3, 1200)
    elif(val == 'OFF'):
        print "stop tracking"
        mgr.setDeviceMemory(0xA2, 3, 0)
    else: 
        print "usage: set_tracking [on/off]"

    mgr.disconnect()
    
