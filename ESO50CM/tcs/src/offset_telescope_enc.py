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

    parser = optparse.OptionParser(usage= "%prog ra=120 dec=60")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()

    ra = 0
    dec = 0
    if len(args) == 0:
        print "wrong format"
    elif len(args) > 2:
        print "wrong format"
    else:
        for a in args:
            tmp = a.split("=")
            if(tmp[0].upper() == "RA"):
                ra = tmp[1]
            elif(tmp[0].upper() == "DEC"):
                dec = tmp[1]
        
    # convert into encoders values
    ra = int(ra)
    dec = int(dec)
    print "Motor Encoders alpha=%d, delta=%d"  % (ra,dec)

    mgr.connect()
    quit = False
    quitAlpha = False
    quitDelta = False 

    #by convention N and E are positive
    ra = -1 * ra
    dec = -1 * dec

    #print current position before movement
    posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
    lectAlphaAxisE = mgr.readDeviceMemory(0xA8,2,0)
    lectAlphaWormE = mgr.readDeviceMemory(0xA6,2,0)
    lectAlphaMotorE = mgr.readDeviceMemory(0xA2,2,0)
    posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
    lectDeltaAxisE = mgr.readDeviceMemory(0xAC,2,0) 
    lectDeltaWormE = mgr.readDeviceMemory(0xAA,2,0)
    lectDeltaMotorE = mgr.readDeviceMemory(0xA4,2,0) 
    print "%8d, %8d, %8d, %8d, %8d, %8d, %8d, %8d" % (lectAlphaAxisE, lectAlphaWormE, lectAlphaMotorE, posAlpha, 
                                       lectDeltaAxisE, lectDeltaWormE, lectDeltaMotorE, posDelta)


    #offset_telescope(ra, dec)
    mgr.setDeviceMemory(0xA2, 7, ra)
    mgr.setDeviceMemory(0xA4, 7, dec)
    time.sleep(1)
    #posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
    #posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
    #print "alpha = %d" % posAlpha
    #print "delta = %d" % posDelta
    #print ""
  

    while(not quit):
        #Alpha
        posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
        lectAlphaAxisE = mgr.readDeviceMemory(0xA8,2,0)
        lectAlphaWormE = mgr.readDeviceMemory(0xA6,2,0)
        lectAlphaMotorE = mgr.readDeviceMemory(0xA2,2,0)

        #print "alpha = %d" % posAlpha
        if (math.fabs(posAlpha) < 20.0):
            #print "listo en alpha"
            quitAlpha = True

        #Delta
        posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
        lectDeltaAxisE = mgr.readDeviceMemory(0xAC,2,0) 
        lectDeltaWormE = mgr.readDeviceMemory(0xAA,2,0)
        lectDeltaMotorE = mgr.readDeviceMemory(0xA4,2,0) 
        #print "delta = %d" % posDelta
        if (math.fabs(posDelta) < 20.0): 
            #print "listo en delta"
            quitDelta = True

        if(quitAlpha and quitDelta): 
           quit = True
           #print " nos fuimos "

        time.sleep(1)
        print "%8d, %8d, %8d, %8d, %8d, %8d, %8d, %8d" % (lectAlphaAxisE, lectAlphaWormE, lectAlphaMotorE, posAlpha, 
                                       lectDeltaAxisE, lectDeltaWormE, lectDeltaMotorE, posDelta)

    mgr.disconnect()
    
