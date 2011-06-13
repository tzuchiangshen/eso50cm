#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
from AstroUtil import *
import os

#global variables
global status 
global ic 
global obsImpl 


def hexStrEndianSwap(theString):
    """Rearranges character-couples in a little endian hex string to
    convert it into a big endian hex string and vice-versa. i.e. 'A3F2'
    is converted to 'F2A3'

    @param theString: The string to swap character-couples in
    @return: A hex string with swapped character-couples. -1 on error."""

    # We can't swap character couples in a string that has an odd number
    # of characters.
    if len(theString)%2 != 0:
        return -1

    # Swap the couples
    swapList = []
    for i in range(0, len(theString), 2):
        swapList.insert(0, theString[i:i+2])

    # Combine everything into one string. Don't use a delimeter.
    return ''.join(swapList)


def connect():
	global status
        global ic
        global obsImpl

	print "Connecting.."
	status = 0
        try:
           ic = Ice.initialize(sys.argv)
           obj = ic.stringToProxy("Observing:tcp -h 192.168.1.101 -p 10000");
           obsImpl = OUC.ObservingPrx.checkedCast(obj)
	   print "Connected to ObsControl"
           if not obsImpl: 
                   raise RuntimeError("Invalid proxy")
	except:
           traceback.print_exc()
           status = 1
	   sys.exit(status)

def disconnect():
	global status
	print "Desconnecting.."
	if ic:
             try: 
                ic.destroy()
             except:
                traceback.print_exc()
                status = 1
	sys.exit(status)


def sayHello():
	try:
	   obsImpl.sayHello(3) 
	   print "I said Hello!!"
        except:
           traceback.print_exc()
	   status = 1

def sayHelloTelescope():
	try:
	   telescope = obsImpl.getTelescope()
           telescope.sayHelloTelescope(2) 
	   print "I said Hello Telescope!!"
        except:
           traceback.print_exc()
	   status = 1


if __name__ == "__main__":
	connect()
        sayHello()
        sayHelloTelescope()
	disconnect()
	
