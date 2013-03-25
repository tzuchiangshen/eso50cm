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

    telData = OUC.TelescopeData()
    telData = mgr.getPosition()
    #print telData

    format = degs2HHMMSS(telData.currentPos.localSideralTime / 15.0)
    sLST = "LST = |%02d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    lt = datetime.utcfromtimestamp(telData.localTime)
    sLT  = "LT  = | %s|" % lt.strftime("%H:%M:%S")
    sLD  = "LD  = |%s|" % lt.strftime("%Y-%m-%d")

    sJD  = "LJD = |%lf|" % telData.julianDate

    format =  degs2HHMMSS(telData.currentPos.RA / 15.0)
    sRA  = "RA  = |%02d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    format =  degs2HHMMSS(telData.currentPos.Dec)
    sDEC = "Dec = |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2]) 

    format =  degs2HHMMSS(telData.currentPos.HA / 15.0)
    sHA  = "HA  = |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    format =  degs2HHMMSS(telData.currentPos.Dec - telData.latitude)
    sDL = " D-L = |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2]) 

    format =  degs2HHMMSS(telData.targetPos.RA / 15.0)
    stRA = "tRA = |%02d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    format =  degs2HHMMSS(telData.targetPos.Dec)
    stDEC= "tDec= |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2]) 

    format =  degs2HHMMSS(telData.targetPos.HA / 15.0)
    stHA = "tHA = |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    format =  degs2HHMMSS(telData.targetPos.Dec - telData.latitude)
    stDL = " D-L = |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2]) 

    format =  degs2HHMMSS(telData.differencePos.RA / 15.0)
    sdRA = "dRA = |%02d:%02d:%02.0lf|" % (format[0],format[1],format[2])

    format =  degs2HHMMSS(telData.differencePos.Dec)
    sdDEC= "dDec= |%+03d:%02d:%02.0lf|" % (format[0],format[1],format[2]) 

    sALT = "Alt = |%lf|" % telData.currentPos.Alt
    sAZ  = "Az  = |%lf|" % telData.currentPos.Az

    print sLST, sLT, sLD, sJD
    print sRA, sDEC, sHA, sDL
    print stRA, stDEC, stHA, stDL
    print sdRA, sdDEC
    print ""
    print sALT
    print sAZ
    print "--------------------------------------------"

    mgr.disconnect()
    
