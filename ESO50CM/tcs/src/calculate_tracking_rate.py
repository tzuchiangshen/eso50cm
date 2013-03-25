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

    lst = telData.currentPos.localSideralTime / 15.0
    print lst 
    while(True): 
        new_lst = telData.currentPos.localSideralTime / 15.0
        format = degs2HHMMSS(new_lst)
        print "|%02d:%02d:%02.0lf| old = %.5lf current=%.5lf delta=%.5lf" %(format[0],format[1],format[2], lst, new_lst, new_lst - lst)
        lst = new_lst
        telData = mgr.getPosition()
        time.sleep(1)




    mgr.disconnect()
    
