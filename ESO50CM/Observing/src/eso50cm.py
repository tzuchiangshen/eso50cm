#!/usr/bin/python 
import sys, traceback, Ice 
import OUC
from datetime import datetime
import time
from AstroUtil import *
import os
import optparse 


#global variables
global status 
global ic 
global obsImpl 



def connect():
    global status
    global ic
    global obsImpl

    #print "Connecting.."
    status = 0
    try:
        # Reading configuration info 
        configPath = os.environ.get("SWROOT")
        configPath = configPath + "/config/Obs-config" 
        initData = Ice.InitializationData()
        initData.properties = Ice.createProperties()
        initData.properties.load(configPath)
        ic = Ice.initialize(sys.argv, initData)

        # Create proxy
        properties = ic.getProperties();
        proxyProperty = "ObsAdapter.Proxy"
        proxy = properties.getProperty(proxyProperty);
        #print proxy
        obj = ic.stringToProxy(proxy);
        obsImpl = OUC.ObservingPrx.checkedCast(obj)
        #print "Connected to ObsControl"
        if not obsImpl: 
            raise RuntimeError("Invalid proxy")
    except:
        traceback.print_exc()
        status = 1
        sys.exit(status)

def disconnect():
    global status
    #print "Disconnecting.."
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

def setConfiguration():
    try:
        telescope = obsImpl.getTelescope()
        introot_path = ""
        introot_path = os.getenv("SWROOT")
        if(introot_path == ""):
            introot_path = "/eso50cm/SWROOT"        

        conf_path = "%s/%s" % (introot_path, "/config/ESO50cm.conf")
        telescope.setConfiguration(conf_path)
        print "Default configuration send to LCUControl"
    except:
        print chr(27)+"[0;31m"+"Problems trying to configure telescope.Cannot find the specific path!!"+ chr(27) + "[0m"
        print chr(27)+"[0;31m"+"Please be sure that you are entering the right information"+ chr(27) + "[0m"
        traceback.print_exc()
        status = 1

def setConfiguration2(conf_path):
    try:
        telescope = obsImpl.getTelescope()
        telescope.setConfiguration(conf_path)
        print "Configuration send to LCUControl"
    except:
        print chr(27)+"[0;31m"+"Problems trying to configure telescope.Cannot find the specific path!!"+ chr(27) + "[0m"
        print chr(27)+"[0;31m"+"Please be sure that you are entering the right information (abs file path = %s)" % conf_path + chr(27) + "[0m"
        #traceback.print_exc()
        status = 1

def getConfiguration():
    telConfigData = OUC.TelescopeConfigData()
    try:
        telescope = obsImpl.getTelescope()
        telConfigData = telescope.getConfiguration()   
        print "LT = [%lf]" % telConfigData.localTime
        print datetime.utcfromtimestamp(telConfigData.localTime)
        print chr(27)+"[0;32m"+"Latitude  = %+11.4lf " % telConfigData.latitude 
        print "Longitude = %+11.4lf " % telConfigData.longitude
        print "Altitude  = %+11.4lf " % telConfigData.altitude + chr(27) + "[0m"
        print chr(27)+"[0;33m"+"AMT       = %+11.4lf " % telConfigData.AMT
        print "AMH       = %+11.4lf " % telConfigData.AMH
        print "AMR       = %+11.4lf " % telConfigData.AMR
        print "AWT       = %+11.4lf " % telConfigData.AWT
        print "AWH       = %+11.4lf " % telConfigData.AWH
        print "AWR       = %+11.4lf " % telConfigData.AWR
        print "AAT       = %+11.4lf " % telConfigData.AAT
        print "AAH       = %+11.4lf " % telConfigData.AAH
        print "AAR       = %+11.4lf " % telConfigData.AAR + chr(27) + "[0m"
        print chr(27)+"[0;34m"+"DMT       = %+11.4lf " % telConfigData.DMT
        print "DMH       = %+11.4lf " % telConfigData.DMH
        print "DMR       = %+11.4lf " % telConfigData.DMR
        print "DWT       = %+11.4lf " % telConfigData.DWT
        print "DWH       = %+11.4lf " % telConfigData.DWH
        print "DWR       = %+11.4lf " % telConfigData.DWR
        print "DAT       = %+11.4lf " % telConfigData.DAT
        print "DAH       = %+11.4lf " % telConfigData.DAH
        print "DAR       = %+11.4lf " % telConfigData.DAR + chr(27) + "[0m"
        print "Generated at = [%lf]" % telConfigData.lcuTime
        print datetime.utcfromtimestamp(telConfigData.lcuTime)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1  

def getEncoderPosition():
    encData = OUC.EncoderData()
    try:
        telescope = obsImpl.getTelescope()
        encData =  telescope.getEncodersPosition();
        print "LT = [%lf]" % encData.localTime
        print datetime.utcfromtimestamp(encData.localTime)
        print chr(27)+"[0;32m"+"AW Position  = [%+10.0lf]" % encData.alphaWormE
        print "AA Position  = [%+10.0lf]" % encData.alphaAxisE 
        print "DW Position  = [%+10.0lf]" % encData.deltaWormE
        print "AA Position  = [%+10.0lf]" % encData.deltaAxisE + chr(27) + "[0m"
        print "Generated at = [%lf]" % encData.lcuTime
        print datetime.utcfromtimestamp(encData.lcuTime)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def isConfigured():
    
    try:
        telescope = obsImpl.getTelescope()
        a =  telescope.isConfigured();
        #print a
        print "isConfigured  =[%s]" % a
    except: 
        print chr(27)+"[0;31m"+"error" + chr(27) + "[0m" 

def isTracking():
    
    try:
        telescope = obsImpl.getTelescope()
        n =  telescope.isTracking();
        #print n
        print "isTracking  =[%s]" % n
    except: 
        print chr(27)+"[0;31m"+"error" + chr(27) + "[0m" 
def getRawEncoderPosition():
    encData = OUC.RawEncoderData()
    try:
        telescope = obsImpl.getTelescope()
        encData =  telescope.getRawEncodersPosition();
        #print encData
        #print "LT = [%lf]" % encData.lcuTime
        #print datetime.utcfromtimestamp(encData.lcuTime)
        print chr(27)+"[0;33m"+"                   Lecture     Position " + chr(27) + "[0m"
        print chr(27)+"[0;33m"+"Alpha AxisE   = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaAxisE, encData.posAlphaAxisE)
        print                  "Alpha WormE   = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaWormE, encData.posAlphaWormE) 
        print                  "Alpha MotorE  = [%+8.0lf     %+8.0lf ]" % (encData.lectAlphaMotor, encData.posAlphaMotor)
        print chr(27)+"[0;32m"+"Delta AxisE   = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaAxisE, encData.posDeltaAxisE)
        print                  "Delta WormE   = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaWormE, encData.posDeltaWormE) 
        print                  "Delta MotorE  = [%+8.0lf     %+8.0lf ]" % (encData.lectDeltaMotor, encData.posDeltaMotor)
        print chr(27)+"[0m" 
        #print "Generated at = [%lf]" % encData.lcuTime
        #print datetime.utcfromtimestamp(encData.lcuTime)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1

def getPosition():
    telData = OUC.TelescopeData()
    try:
        telescope = obsImpl.getTelescope()
        telData = telescope.getPosition()
        print "LT              = [%lf]" % telData.localTime
        print datetime.utcfromtimestamp(telData.localTime)
        print "Time elapsed since last access: %lf" % telData.deltaT  
        print "JD              =  %lf" % telData.julianDate + chr(27) + "[0m"
        print chr(27)+"[0;32m"+"Latitude        =   %+11.4lf" % telData.latitude + chr(27) + "[0m"
        print chr(27)+"[0;32m"+"Longitude       =   %+11.4lf" % telData.longitude + chr(27) + "[0m"
        print chr(27)+"[0;32m"+"Altitude        =   %+11.4lf" % telData.altitude + chr(27) + "[0m"
        print "High Elevation  =   %+11.4lf" % telData.highElevation
        print "Low Elevation   =   %+11.4lf" % telData.lowElevation
        format =  degs2HHMMSS(telData.currentPos.localSideralTime / 15.0)
        print "LST             =       %02d:%02d:%02.0lf" % (format[0],format[1],format[2])  
        format =  degs2HHMMSS(telData.currentPos.RA / 15.0)
        print chr(27)+"[0;34m"+"Current RA      =       %02d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.currentPos.Dec)
        print chr(27)+"[0;34m"+"Current Dec     =      %+03d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.currentPos.HA / 15.0)
        print chr(27)+"[0;34m"+"Current HA      =      %+03d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        print chr(27)+"[0;34m"+"Current Alt     =       %lf" % telData.currentPos.Alt + chr(27) + "[0m"
        print chr(27)+"[0;34m"+"Current Az      =      %lf" % telData.currentPos.Az + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.targetPos.RA / 15.0)
        print chr(27)+"[0;31m"+"Target RA       =       %02d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.targetPos.Dec)
        print chr(27)+"[0;31m"+"Target Dec      =      %+03d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.targetPos.HA / 15.0)
        print chr(27)+"[0;31m"+"Target HA       =      %+03d:%02d:%02.0lf" % (format[0],format[1],format[2]) + chr(27) + "[0m"
        print chr(27)+"[0;31m"+"Target Alt      =        %lf" % telData.targetPos.Alt + chr(27) + "[0m"
        print chr(27)+"[0;31m"+"Target Az       =        %lf" % telData.targetPos.Az + chr(27) + "[0m"
        format =  degs2HHMMSS(telData.differencePos.RA / 15.0)
        print "Difference RA   =      %02d:%02d:%02.0lf" % (format[0],format[1],format[2])
        format =  degs2HHMMSS(telData.differencePos.Dec)
        print "Difference Dec  =     %+03d:%02d:%02.0lf" % (format[0],format[1],format[2])
        print "Generated at    = [%lf]" % telData.lcuTime
        print datetime.utcfromtimestamp(telData.lcuTime)
    except OUC.TelescopeNotConfiguredEx():
        print "Telescope Not Configured !!!"
        traceback.print_exc()
        status = 1



if __name__ == "__main__":
    parser = optparse.OptionParser(usage= "%prog -c command")
    parser.add_option("-c", "--command", dest="command", default = None, 
                        type = "string", help=chr(27)+"[0;31m"+"send command to the telescope ,avaible commands: isConfigured get_position \n get_config \n read_encoders \n read_rawencoders \n istracking \n offset_telescope"+ chr(27) + "[0m")

    (options, args) = parser.parse_args()

    print chr(27)+"[0;32m"+"command received: " + chr(27) + "[0m" 
    print options
    print "arguments are: "
    print args

    connect()
    command = options.command
    if (command == "set_config"):
        if len(args) != 0:
            setConfiguration2(args[0])
        else:
            setConfiguration()
    elif (command == "set_tracking"):
        trkInfo = OUC.TrackingInfo()
        try:
            if len(args) != 0:
                trkInfo.trackState = True
                trkInfo.ticVel = int(eval(args[0]))
            else:
                trkInfo.trackState = True
                trkInfo.ticVel = 1200
            telescope = obsImpl.getTelescope()
            telescope.setTracking(trkInfo)
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1
        except ValueError: 
            print "The argument must be a number, invalid argument %s " % args[0]
        except SyntaxError:
            print "The argument must be a number, invalid argument %s " % args[0]
    elif (command == "stop_tracking"):
        trkInfo = OUC.TrackingInfo()
        try:
            trkInfo.trackState = False
            trkInfo.ticVel = 0 
            telescope = obsImpl.getTelescope()
            telescope.setTracking(trkInfo)
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1
        except ValueError: 
            print "The argument must be a number, invalid argument %s " % args[0]
        except SyntaxError:
            print "The argument must be a number, invalid argument %s " % args[0]
    elif (command == "offset_telescope"):
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
        offsetPos = OUC.TelescopePosition()
        offsetPos.RA = sexagesimal2degs(ra, True)
        offsetPos.Dec = sexagesimal2degs(dec, False)
        print "setOffset ra=%f, dec=%f" % (offsetPos.RA, offsetPos.Dec)
        try:
            telescope = obsImpl.getTelescope()
            telescope.setOffset(offsetPos)
        except OUC.TargetOutOfLimitsEx():
            print "Target Out of Limits. Try a new one\n";
            traceback.print_exc()
            status =1
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status =1
        except ValueError:
            print "wrong format"
    elif (command == "set_target"):
        ra = "00:00:00"
        dec = "00:00:00"
        if len(args) != 2:
            print "wrong format" 
        else: 
            for a in args: 
                tmp = a.split("=")
                if(tmp[0].upper() == "RA"):
                    ra = tmp[1]
                elif(tmp[0].upper() == "DEC"):
                    dec = tmp[1]
        targetPos = OUC.TelescopePosition()
        targetPos.RA = sexagesimal2degs(ra, True)
        targetPos.Dec = sexagesimal2degs(dec, False)
        #print "setOffset ra=%f, dec=%f" % (offsetPos.RA, offsetPos.Dec)
        try:
            telescope = obsImpl.getTelescope()
            telescope.setTarget(targetPos)
        except OUC.TargetOutOfLimitsEx():
            print "Target Out of Limits. Try a new one\n";
            traceback.print_exc()
            status =1
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status =1
        except ValueError:
            print "wrong format"
    elif (command == "goto_target"):
        try:
            telescope = obsImpl.getTelescope()
            telescope.moveToTarget()
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1
    elif (command == "park_telescope"):
        try:
            telescope = obsImpl.getTelescope()
            telescope.parkTelescope()
        except OUC.TelescopeNotConfiguredEx():
            print "Telescope Not Configured !!!"
            traceback.print_exc()
            status = 1
    elif (command == "stop_telescope"):
        try:
            telescope = obsImpl.getTelescope()
            telescope.stopTelescope(OUC.TelescopeDirection.North)
        except:
            traceback.print_exc()
            status = 1
    elif (command == "get_config"):
        getConfiguration()
    elif (command == "get_position"):
        getPosition()
    elif (command == "read_encoders"):
        getEncoderPosition()
    elif (command == "read_rawencoders"):
        getRawEncoderPosition()
    elif (command == "isconfigured"):
        isConfigured()
    elif (command == "istracking"):
        isTracking()
    else:
        print "unknown command" 

    disconnect()

