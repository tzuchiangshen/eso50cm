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

    parser = optparse.OptionParser(usage= "%prog ra=00:00:00 dec=00:00:00")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()

    ra = "00:00:00"
    dec = "00:00:00"
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
        
    ra = sexagesimal2degs(ra, True)
    dec = sexagesimal2degs(dec, False)
    print "Degrees in the sky ra=%.5lf, dec=%.5lf"  % (ra, dec)

    # convert into encoders values
    ra = int(ra / 360.0 * 17280.0 * 6000.0)
    dec = int(dec / 360.0 * 13882.0 * 6000.0)
    print "Motor Encoders alpha=%d, delta=%d"  % (ra,dec)

    mgr.connect()
    quit = False
    quitAlpha = False
    quitDelta = False 

    #by convention N and E are positive
    ra = -1 * ra
    dec = -1 * dec

    #offset_telescope(ra, dec)
    mgr.setDeviceMemory(0xA2, 7, ra)
    mgr.setDeviceMemory(0xA4, 7, dec)
    time.sleep(1)
    posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
    posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
    print "alpha = %d" % posAlpha
    print "delta = %d" % posDelta
    print ""

    while(not quit):
        #Alpha
        posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
        print "alpha = %d" % posAlpha
        if (math.fabs(posAlpha) < 50.0):
            #print "listo en alpha"
            quitAlpha = True

        #Delta
        posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
        print "delta = %d" % posDelta
        if (math.fabs(posDelta) < 50.0): 
            #print "listo en delta"
            quitDelta = True

        if(quitAlpha and quitDelta): 
           quit = True
           #print " nos fuimos "

        time.sleep(3)
        print("")

    mgr.disconnect()
    
