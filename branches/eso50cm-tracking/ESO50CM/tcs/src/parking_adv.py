#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
from time import sleep
import math
from AstroUtil import *
import os
import optparse
from DeviceManager import DeviceManager
 
        
if __name__ == "__main__":

    parser = optparse.OptionParser(usage= "%prog ra=00:00:00 dec=00:00:00")
    (options, args) = parser.parse_args()

    mgr = DeviceManager()

    #print "Motor Encoders alpha=%d, delta=%d"  % (ra,dec)

    mgr.connect()
    quit = False
    quitAlpha = False
    quitDelta = False 
    moveWest = False
    moveEast = False

    # 1.1 find Alpha AxisE = 561
    alphaAxisE_current = mgr.readDeviceMemory(0xA8, 2, 0)
    print "current axisE = %d" % alphaAxisE_current
    alpha_diff = 561 -alphaAxisE_current
    if( alpha_diff > 0 ):
        print "I should move RA axis toward West in order to find the park position" 
        moveWest = True
        moveEast = False
    else:
        print "I should move RA toward East, in order to find the park position"
        moveEast = True
        moveWest = False

    # 1.2: move RA axis. 
    counter = 0
    while(not quit):
        if(math.fabs(alpha_diff) > 50):
            step = 7200000
        elif(math.fabs(alpha_diff) > 5): 
            step = 720000
        elif(math.fabs(alpha_diff) > 1):
	    step = 120000
        else:
	    step = 12000

        if(moveWest):
	    step = step * 1
        else: 
            step = step * -1
      
        #move the alpha motor, hopefully with the correct direction 
        mgr.setDeviceMemory(0xA2, 7, step)
        print "step %d, moving %d motor encoder in alpha axis (negative is East). diff=%d" % (counter, step, alpha_diff)
        # wait until the motor reach the position
        wait = True
        while(wait):
            #need couples of seconds to apply the command
            sleep(3)
            alphaMotorE = mgr.readDeviceMemory(0xA2, 7, 0)
            if (math.fabs(alphaMotorE) < 50.0):
                print "listo en alpha"
                wait = False
            else:
                print "waiting ..."
                sleep(1)
        #one step ready, check again
        alphaAxisE_current = mgr.readDeviceMemory(0xA8, 2, 0)
        alpha_diff = 561 - alphaAxisE_current
        print "moveWest = %d, alpha_diff = %d " % (moveWest, alpha_diff)
        if(moveWest):
            if (alpha_diff <= 0):
               quit = True
        else:
            #move east
            if (alpha_diff >= 0):
               quit = True

        counter = counter + 1
        print ""

    # 2.1 find Alpha WormE = 469
    print" Doing fine ajustment: move worm to 469 ...." 
    alphaAxisE_current = mgr.readDeviceMemory(0xA6, 2, 0)
    print "current axisE = %d" % alphaAxisE_current
    alpha_diff = 561 - alphaAxisE_current
    if( alpha_diff > 0 ):
        print "I should move RA axis toward West in order to find the park position" 
        moveWest = True
        moveEast = False
    else:
        print "I should move RA toward East, in order to find the park position"
        moveEast = True
        moveWest = False

    # 1.2: move RA axis. 
    counter = 0
    quit = False
    while(not quit):
        if(math.fabs(alpha_diff) > 50):
            step = 7200
        elif(math.fabs(alpha_diff) > 5): 
            step = 1200
        elif(math.fabs(alpha_diff) > 1):
	    step = 720
        else:
	    step = 120

        #for the encoder in the worm, positive means East
        if(moveWest):
	    step = step * -1
        else: 
            step = step * 1
      
        #move the alpha motor, hopefully with the correct direction 
        mgr.setDeviceMemory(0xA2, 7, step)
        print "step %d, moving %d motor encoder in alpha axis (negative is West). diff=%d" % (counter, step, alpha_diff)
        # wait until the motor reach the position
        wait = True
        while(wait):
            #need couples of seconds to apply the command
            sleep(3)
            alphaMotorE = mgr.readDeviceMemory(0xA2, 7, 0)
            if (math.fabs(alphaMotorE) < 50.0):
                print "listo en alpha"
                wait = False
            else:
                print "waiting ..."
                sleep(1)
        #one step ready, check again
        alphaAxisE_current = mgr.readDeviceMemory(0xA6, 2, 0)
        alpha_diff = 469 - alphaAxisE_current
        print "moveWest = %d, alpha_diff = %d " % (moveWest, alpha_diff)
        if(moveWest):
            if (alpha_diff <= 0):
               "Sorry I passed ...., you should go to the opposive way, moveWest=%d" % moveWest
               quit = True
        else:
            #move east
            if (alpha_diff >= 0):
               "Sorry I passed ...., you should go to the opposive way, moveWest=%d" % moveWest
               quit = True

        counter = counter + 1
        print ""


     



    #by convention N and E are positive
    #ra = -1 * ra
    #dec = -1 * dec

    #offset_telescope(ra, dec)
#    mgr.setDeviceMemory(0xA2, 7, ra)
#    mgr.setDeviceMemory(0xA4, 7, dec)
#    time.sleep(1)
#    posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
#    posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
#    print "alpha = %d" % posAlpha
#    print "delta = %d" % posDelta
#    print ""
#
#    while(not quit):
#        #Alpha
#        posAlpha = mgr.readDeviceMemory(0xA2, 7, 0)
#        print "alpha = %d" % posAlpha
#        if (math.fabs(posAlpha) < 50.0):
#            #print "listo en alpha"
#            quitAlpha = True
#
#        #Delta
#        posDelta = mgr.readDeviceMemory(0xA4, 7, 0)
#        print "delta = %d" % posDelta
#        if (math.fabs(posDelta) < 50.0): 
#            #print "listo en delta"
#            quitDelta = True
#
#        if(quitAlpha and quitDelta): 
#           quit = True
#           #print " nos fuimos "
#
#        time.sleep(3)
#        print("")

    mgr.disconnect()
    
