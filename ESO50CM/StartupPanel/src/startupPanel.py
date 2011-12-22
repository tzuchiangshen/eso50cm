#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Startup Panel
"""
# basic imports
import sys
import os
# for threading and semaphores

# for UI
from Ui_StartupPanelBase import Ui_StartupPanelBase
from PyQt4 import  QtGui, QtCore
#for bash process management
import shlex, pexpect,subprocess
#for read configuration file
from xml.etree.ElementTree import ElementTree

UPDATE_TIME=7 # in seconds
DEBUG=False

class ProcessStatusChecker(QtCore.QThread):
    def __init__(self,name,username,host,grep,parent=None):
        QtCore.QThread.__init__(self,parent)
        self.username=username
        self.host=host
        self.grep=grep
        self.name=name
    def run(self):
        try:
            if DEBUG:   f=open('debug'+self.name[1:],'w+')
            self.emit(QtCore.SIGNAL("CHECKING_STARTED(  )"))
            if DEBUG:   f.write("checking %s start checking host communication\n"%(self.name))
            message=self.checkHostCommunication()     
            if DEBUG:   f.write("checking %s finished checking host communication\n"%(self.name))
            if message != "OK":
                self.emit(QtCore.SIGNAL("SERVICE_STOPPED( QString )"),message)
                return
            self.emit(QtCore.SIGNAL("SET_STATUS( QString)"),'blue:Checking')

            #you process should be detected by > ps -fea | egrep "<your regular expression>" |grep -v grep| awk  '{print $2}'
            commandLine='ssh '+self.username+'@'+self.host+' ps -fea | egrep "'+self.grep+'" '+'| grep -v grep '
            if DEBUG:   f.write("checking %s running %s\n"%(self.name,commandLine))
            p = subprocess.Popen(shlex.split(commandLine), stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if DEBUG:   f.write("checking %s finished %s\n"%(self.name,commandLine))
            pids=[]
            for line in p.stdout.readlines():
                line=line.strip()
                if line!='' and line.count("<defunct>")==0:
                    pids.append(line)
            p.stdout.close() 
            self.instances=pids.__len__()
            for l in pids:
                if DEBUG:   f.write(l+"\n")
            if (self.instances==0):
                self.emit(QtCore.SIGNAL("SERVICE_STOPPED( QString )"),"Stopped")
            else:
                #ok! the process is started, but we will not mark it inmediatly as started
                self.emit(QtCore.SIGNAL("PROC__STARTED(  )"))
            if DEBUG:   f.write("checking %s finished\n"%(self.name))
        except Exception,ex:
            self.emit(QtCore.SIGNAL("SET_STATUS( QString)"),"red:Error updating process status: "+str(ex))
            #self.status.setToolTip("Error updating process status: "+str(ex))
        finally:
            if DEBUG:   f.write("Done\n")
            if DEBUG:   f.close()
            self.emit(QtCore.SIGNAL("CHECKING_FINISHED( )"))

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

class Service(QtCore.QObject):
    def __init__(self,name, host,username, command, grep,dependencies,timeToStart,keep,isProcessRunning):
        self.name=name
        self.username=username
        self.host=host
        self.command=command
        self.grep=grep
        self.dependencies=dependencies
        self.instances=0
        self.isProcessRunning=isProcessRunning
        self.timeToStart=timeToStart
        self.button= QtGui.QPushButton("Start")
        QtCore.QObject.connect(self.button, QtCore.SIGNAL("clicked()"), self.buttonPressed)
        self.keepCB=QtGui.QCheckBox()
        if keep:
            self.keepCB.setChecked(True)
        else:
            self.keepCB.setChecked(False)
        self.status=QtGui.QTableWidgetItem()
        self.running=False # we will change this calling stopService method
        self.startResquested=False
        # timer for updating
        self.ctimer = QtCore.QTimer()
        # timer for set the process as started (single shot)
        self.stimer = QtCore.QTimer()
        self.waitingForTimer=False
        QtCore.QObject.connect(self.ctimer, QtCore.SIGNAL("timeout()"), self.updateTimer)
        self.ctimer.start(UPDATE_TIME*1000)
        self.checking=False
        self.waitingForDep="None"
        self.procChecker=ProcessStatusChecker(name,username,host,grep)
        self.connect(self.procChecker,QtCore.SIGNAL("SET_STATUS( QString ) "), self.setStatus)
        self.connect(self.procChecker,QtCore.SIGNAL("SERVICE_STOPPED( QString ) "), self.markAsStopped)
        self.connect(self.procChecker,QtCore.SIGNAL("PROC__STARTED(  ) "), self.markAsStarted)
        self.connect(self.procChecker,QtCore.SIGNAL("CHECKING_FINISHED (  ) "), self.checkingFinished)
        self.connect(self.procChecker,QtCore.SIGNAL("CHECKING_STARTED (  ) "), self.checkingStarted)
        self.updateTimer()
            
    def markAsStopped(self,message):
        self.running=False
        if (self.waitingForDep!="None"):
            message="Waiting for "+self.waitingForDep
        self.status.setTextColor(QtGui.QColor('red'))
        self.status.setText(message)
        #self.status.toolTip(message)
        self.button.setText("Start")   
    def checkingStarted(self):
        self.checking=True   
    def checkingFinished(self):
        self.checking=False
    def markAsStarted(self):
        self.running=True
        self.status.setTextColor(QtGui.QColor('green'))
        self.status.setText("Running")
        #self.status.toolTip(message)
        self.button.setText("Stop")
        if not self.keepCB.isChecked():
            self.startResquested=False
    def setStatus(self,message):
        self.status.setTextColor(QtGui.QColor(message.split(":",1)[0]))
        self.status.setText(message.split(":",1)[1])
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
    def updateTimer(self):
        self.protectedCheck()
        if (self.startResquested and not self.running and not self.waitingForTimer):
                self.startService()
    def startService(self):
        message=self.checkHostCommunication()     
        if message != "OK":
            self.markAsStopped(message)
            return           
        try:
            self.status.setTextColor(QtGui.QColor('black'))
            self.status.setText("Starting service")
            self.keepChecking=True  # we will use this variable to stop checking if we press the stop button
            self.waitingForDep="None"
            for dep in self.dependencies:
                if self.isProcessRunning(dep):
                    continue
                else:  # if not running, we will keep asking every 2 seconds
                    self.status.setTextColor(QtGui.QColor('brown'))
                    self.waitingForDep=dep
                    self.status.setText("waiting for "+dep)
                    return
            self.waitingForDep="None"
            # here we actually start the process
            subprocess.Popen(shlex.split('ssh -X '+self.username+'@'+self.host+" 'export PS1=1;source ~/.bashrc; nohup "+self.command+"' &"))
            self.waitingForTimer=True
            self.stimer.singleShot(self.timeToStart*1000, self.protectedCheckForced) 
        except Exception, e:
            self.markAsStopped(str(e))
    def protectedCheck(self):
        if not self.waitingForTimer:
            if not self.checking:
                self.procChecker.start()
    def protectedCheckForced(self):
        self.waitingForTimer=False
        self.protectedCheck()

    def stopService(self):
        try:
            #you process should be detected by > ps -fea | egrep "<your regular expression>" |grep -v grep| awk  '{print $2}'
            commandLine='ssh '+self.username+'@'+self.host+' ps -fea | egrep "'+self.grep+'" '+'| grep -v grep '
            p = subprocess.Popen(shlex.split(commandLine), stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            pids=[]
            for line in p.stdout.readlines():
                line=line.strip()
                if line!='' and line.count("<defunct>")==0:
                    line=line.split()[1]
                    pids.append(line)
            if DEBUG:   print pids
            p.stdout.close() 
            for pid in pids:
                command='ssh '+self.username+'@'+self.host+" kill -9 "+str(pid)
                if DEBUG:   print command
                subprocess.call(shlex.split(command))
        except Exception,ex:
            print ex  
        finally:
            self.protectedCheck()  
    def buttonPressed(self):
        if not self.running:
            self.startResquested=True
            self.startService()
        else:
            self.startResquested=False
            self.stopService()     
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


                    
class Panel():
    def __init__(self,name, host,username, command):
        self.name=name
        self.username=username
        self.host=host
        self.command=command
    def launchPanel(self):
        subprocess.Popen(shlex.split('ssh -X '+self.username+'@'+self.host+" 'export PS1=1;source ~/.bashrc;"+self.command+"' &"))
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
        self.setupUI() # local version
               
    def isProcessRunning(self,processName):
        try:
            service=self.servicesList[processName]
        except:
            raise Exception(processName+" not in the list of process defined")
        return self.servicesList[processName].running
    def stopAll(self):
        for service in self.servicesList:
            self.servicesList[service].startResquested=False
            self.servicesList[service].stopService() 
    def startAll(self):
        for service in self.servicesList:
            self.servicesList[service].startResquested=True     
            self.servicesList[service].protectedCheck()
    def setupUI(self):
        ui=self.ui # to make shorter calls
        QtCore.QObject.connect(ui.startAllB, QtCore.SIGNAL("clicked()"),self.startAll) # connect the start button
        QtCore.QObject.connect(ui.stopAllB, QtCore.SIGNAL("clicked()"),self.stopAll) # connect the start button
        for panel in self.panelList:
            ui.pbutton = QtGui.QPushButton(self.panelList[panel].name, ui.launchPanelGB)   
            QtCore.QObject.connect(ui.pbutton, QtCore.SIGNAL("clicked()"),self.panelList[panel].launchPanel)
            ui.verticalLayout_4.addWidget(ui.pbutton)
        # Here we fill the process table
        ui.servicesListTable.setSortingEnabled(False)
        rowCounter=0
        ui.servicesListTable.setRowCount(self.servicesList.__len__())
        for service in self.servicesList:
            # keep it running checkbox
            ui.servicesListTable.setCellWidget(rowCounter,0,self.servicesList[service].keepCB)
            # process name
            item = QtGui.QTableWidgetItem()
            item.setText(self.servicesList[service].name)
            ui.servicesListTable.setItem(rowCounter,1,item)
            # host name
            item = QtGui.QTableWidgetItem()
            item.setText(self.servicesList[service].host)
            ui.servicesListTable.setItem(rowCounter,2,item)
            # Action button
            ui.servicesListTable.setCellWidget(rowCounter,3,self.servicesList[service].button)
            # now create the status widget     
            ui.servicesListTable.setItem(rowCounter,4,self.servicesList[service].status)
            rowCounter+=1     
        QtCore.QObject.connect(ui.quitButton, QtCore.SIGNAL("clicked()"), QtCore.QCoreApplication.instance().quit)
        ui.servicesListTable.setColumnWidth(0,20) # status message       
        ui.servicesListTable.setColumnWidth(1,200) # we start column 0 (process name) a little wider
        ui.servicesListTable.setColumnWidth(2,150) # host
        ui.servicesListTable.setColumnWidth(4,300) # status message
                
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
            keepItRunning=service.find('keepItRunning')
            if keepItRunning==None:
                keep=False
            else:
                keep=bool(keepItRunning.text)
                print    keepItRunning.text
            deplist=[]
            for dep in service.findall('dependency'):
                deplist.append(dep.text)
            self.servicesList[name]=Service(name, host,username, command, grep,deplist,timeToStart,keep,self.isProcessRunning)  
        for panel in tree.findall('button'):
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
    
