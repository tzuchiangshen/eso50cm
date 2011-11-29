#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""

"""
# basic imports
import sys
import os
# for threading and semaphores
from threading import Thread, Semaphore
from time import sleep
# for UI
from Ui_StartupPanelBase import Ui_StartupPanelBase
from PyQt4 import  QtGui, QtCore
#for bash process management
import shlex, pexpect,subprocess
#for read configuration file
from xml.etree.ElementTree import ElementTree

UPDATE_TIME=5


class Service(Thread):
    def __init__(self,name, host,username, command, grep,dependencies,timeToStart,isProcessRunning):
        Thread.__init__(self)
        self.isProcessRunning=isProcessRunning  # this is a method, will go up to ask for a process status information
        self.name=name
        self.username=username
        self.host=host
        self.command=command
        self.grep=grep
        self.dependencies=dependencies
        self.instances=0
        self.timeToStart=timeToStart
        self.button= QtGui.QPushButton("Start")
        QtCore.QObject.connect(self.button, QtCore.SIGNAL("clicked()"), self.buttonPressed)
        self.status=QtGui.QTableWidgetItem()
        self.running=False # we will change this calling stopService method
        self.semChecking=Semaphore()
        self.starting=False
        self.stopping=False
        self.keepChecking=False
        self.markAsStopped()
            
    def markAsStopped(self,message="Stopped"):
        if (self.running):
            self.running=False
            self.status.setTextColor(QtGui.QColor('red'))
            self.status.setText(message)
            self.status.toolTip(message)
            self.button.setText("Start")

    def markAsStarted(self,message="Running"):
        if (not self.running):
            self.running=True
            self.status.setTextColor(QtGui.QColor('green'))
            self.status.setText(message)
            self.status.toolTip(message)
            self.button.setText("Stop")
                           
    def checkStatus(self):
        try:
            self.semChecking.acquire()      
            self.status.setTextColor(QtGui.QColor('blue'))
            self.status.setText("Checking....")
            #you process should be detected by > ps -fea | egrep "<your regular expression>" |grep -v grep| awk  '{print $2}'
            commandLine='ssh -X '+self.username+'@'+self.host+' ps -fea | egrep "'+self.grep+'" '+'| grep -v grep'
            print self.name+"-commandLine: "+commandLine
            ssh=subprocess.Popen(shlex.split(commandLine),stdout=subprocess.PIPE)
            awk=subprocess.Popen(['awk','{print $2}'],stdin=ssh.stdout ,stdout=subprocess.PIPE)
            output=awk.communicate()[0].strip()
            pids=output.split('\n')
            ssh.stdout.close()
            try: 
                pids.remove('')
            except:
                pass
            self.instances=pids.__len__()
            if (self.instances==0):
                self.markAsStopped()
            else:
                sleep(self.timeToStart)
                self.markAsStarted()
        except Exception,ex:
            self.status.setText("Error updating process status: "+str(ex))
            self.status.setToolTip("Error updating process status: "+str(ex))
        finally:
            self.semChecking.release()
    def startService(self):
        if (self.starting):
            return
        self.starting=True
        message=self.checkHostCommunication()     
        if message != "OK":
            self.markAsStopped(message)
            self.starting=False
            return
            
        try:
            self.status.setTextColor(QtGui.QColor('black'))
            self.status.setText("Starting service")
            self.keepChecking=True  # we will use this variable to stop checking if we press the stop button
            for dep in self.dependencies:
                if self.isProcessRunning(dep):
                    continue
                else:  # if not running, we will keep asking every 2 seconds
                    self.status("waiting for "+dep)
                    while(self.keepChecking):
                        if self.isProcessRunning():
                            break
                        sleep(2)
                    if not self.keepChecking:  # someone press a button, we will not continue checking neither starting
                        self.keepChecking=False 
                        self.starting=False
                        return
                    self.keepChecking=False          
            subprocess.call(shlex.split('ssh -X  '+self.username+'@'+self.host+" "+self.command+" &"))
            self.checkStatus()
            self.starting=False
        except:
            self.starting=False
                
    def stopService(self):
        if (self.stopping):
            return
        self.stopping=True
        try:
            #you process should be detected by > ps -fea | egrep "<your regular expression>" |grep -v grep| awk  '{print $2}'
            commandLine='ssh -X '+self.username+'@'+self.host+' ps -fea | egrep "'+self.grep+'" '+'| grep -v grep'
            print self.name+"-commandLine:"+commandLine
            ssh=subprocess.Popen(shlex.split(commandLine),stdout=subprocess.PIPE)
            awk=subprocess.Popen(['awk','{print $2}'],stdin=ssh.stdout ,stdout=subprocess.PIPE)
            output=awk.communicate()[0].strip()
            pids=output.split('\n')
            ssh.stdout.close() 
            for pid in pids:
                subprocess.call(shlex.split('ssh '+self.username+'@'+self.host+" kill -9 "+str(pid)))
                sleep(1)
            self.stopping=False
            
        except:
            self.stopping=False
    def buttonPressed(self):
        if not self.running:
            self.startService()
            self.checkStatus()
        else:
            self.stopService()
            self.checkStatus()
        if self.keepChecking:  # this is usefull to stop checking for dependencies
            self.keepChecking=False
       
    def printService(self):
        print "Service: "+self.name
        print "\t"+self.host
        print "\t"+self.name
        print "\t"+self.username
        print "\t"+self.grep
        print "\t"+self.command
        print "\t"+str(self.timeToStart)
        print "\tDependencies"
        for dep in self.dependencies:
            print "\t\t"+dep
    def checkHostCommunication(self):    
        ssh_newkey = 'Are you sure you want to continue connecting'
        # my ssh command line
        p=pexpect.spawn("ssh "+self.username+"@"+self.host+" echo 'Hello!'")
        
        i=p.expect([ssh_newkey,'password:',pexpect.EOF,pexpect.TIMEOUT])
        if i==0:
            # first conection, asking for continue
            p.sendline('yes')
            i=p.expect([ssh_newkey,'password:',pexpect.EOF,pexpect.TIMEOUT])
        if i==1:
            # it's asking for a password! we're not expecting this!!
            # this is not useful for us, so we just kill the process
            p.kill(pexpect.signal.SIGTERM)
            return "A password was asked. Please make sure to export the proper authorization keys in the host machine (run 'ssh-copy "+self.username+"@"+self.host+"')"
        elif i==2:
            # timeout or the server is not responding, check manually!
            if  p.before.strip() == "Hello!": # print out the result
                return "OK"
            else: 
                return "I cannot communicate with the server. Please check manually running `ssh "+self.username+"@"+self.host+" echo 'Hello!' ` at console. You should get a 'Hello!' message (no pass)"
    def run(self):
        comStablished=self.checkHostCommunication()     
        if comStablished != "OK":
            self.markAsStopped(comStablished)
            
        while(True):
            try:
                if comStablished == "OK":
                    self.checkStatus()
                else:
                    comStablished=self.checkHostCommunication()     
            except:
                pass
            finally:
                sleep(5)
                
class Panel():
    def __init__(self,name, host,username, command):
        self.name=name
        self.username=username
        self.host=host
        self.command=command
    def launchPanel(self):
        commandLine='ssh -X '+self.username+'@'+self.host+' "'+ self.command+'" &'
        subprocess.call(shlex.split(commandLine))
    def printPanel(self):
        print "Panel: "+self.name
        print "\t"+self.host
        print "\t"+self.name
        print "\t"+self.username
        print "\t"+self.command

                 
class StartupPanel(QtGui.QMainWindow):
    def __init__(self, confFile,parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_StartupPanelBase()
        self.ui.setupUi(self) # inherited version
        self.configurationFile=confFile # configuration file
        self.servicesList={}
        self.panelList={}
        self.readConfigurationFile()
        self.threadList=[]
        self.setupUI() # local version
    def isProcessRunning(self,processName):
        try:
            service=self.servicesList[processName]
        except:
            return False
        if service == None:
            return False
        else:
            return service.running
    def setupUI(self):
        ui=self.ui # to make shorter calls
        for panel in self.panelList:
            ui.pbutton = QtGui.QPushButton(self.panelList[panel].name, ui.launchPanelGB)   
            QtCore.QObject.connect(ui.pbutton, QtCore.SIGNAL("clicked()"),self.panelList[panel].launchPanel)
            ui.verticalLayout_4.addWidget(ui.pbutton)
        # Here we fill the process table
        ui.servicesListTable.setSortingEnabled(False)
        rowCounter=0
        ui.servicesListTable.setRowCount(self.servicesList.__len__())
        for service in self.servicesList:
            # process name
            item = QtGui.QTableWidgetItem()
            item.setText(self.servicesList[service].name)
            ui.servicesListTable.setItem(rowCounter,0,item)
            # host name
            item = QtGui.QTableWidgetItem()
            item.setText(self.servicesList[service].host)
            ui.servicesListTable.setItem(rowCounter,1,item)
            # Action button
            ui.servicesListTable.setCellWidget(rowCounter,2,self.servicesList[service].button)
            # now create the status widget     
            ui.servicesListTable.setItem(rowCounter,3,self.servicesList[service].status)
            # and finally we create the thread that will manage the status of the proc 
            self.servicesList[service].start()
            rowCounter+=1     
        QtCore.QObject.connect(ui.quitButton, QtCore.SIGNAL("clicked()"), QtCore.QCoreApplication.instance().quit)
        ui.servicesListTable.setColumnWidth(0,300) # we start column 0 (process name) a little wider
        ui.servicesListTable.setColumnWidth(1,150) # host
        ui.servicesListTable.setColumnWidth(3,200) # status message

    def readConfigurationFile(self):
        et=ElementTree()
        tree=et.parse(self.configurationFile)
        # first, we read the services and create a list with them
        for service in tree.findall('service'):
            name=service.find('name').text
            host=service.find('host').text
            username=service.find('username').text
            grep=service.find('grep').text
            command=service.find('command').text
            timeToStart=service.find('timeToStart')
            if (timeToStart==None):
                
                timeToStart=0
            else:
                timeToStart=int(timeToStart.text)
            deplist=[]
            for dep in service.findall('dependency'):
                deplist.append(dep.text)
            self.servicesList[name]=Service(name, host,username, command, grep,deplist,timeToStart,self.isProcessRunning)  
        for panel in tree.findall('panel'):
            name=panel.find('name').text
            host=panel.find('host').text
            username=panel.find('username').text
            command=panel.find('command').text
            self.panelList[name]=Panel(name, host,username, command)
    
if __name__ == "__main__":
    defaultConfFile=os.environ['SWROOT']+'/config/startupPanelConf.xml'
    if sys.argv.__len__() > 1:
        confFile=sys.argv[1]
    else:
        confFile=defaultConfFile
        
    if (not os.path.exists(confFile)):
        print "Error, cannot read "+confFile
        sys.exit(1)   
    app = QtGui.QApplication(sys.argv)
    myapp = StartupPanel(confFile)
    myapp.show()
    sys.exit(app.exec_())
    
