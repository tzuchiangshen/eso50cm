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

    print "stop alpha motor"
    mgr.setDeviceMemory(0xA2, 7, 0)
    mgr.setDeviceMemory(0xA2, 6, 0)
    print "stop delta motor"
    mgr.setDeviceMemory(0xA4, 7, 0)
    mgr.setDeviceMemory(0xA4, 7, 0)

    mgr.disconnect()
    
