# -*- coding: utf-8 -*-

import re
import queue  
import time

from PyQt4.QtGui import QFileDialog
from PyQt4.QtCore import pyqtSignature
from PyQt4.QtGui import QMainWindow
from Ui_htmlParser import Ui_MainWindow
from htmlDataAcquired import htmlDataAcquired
from RootWordGenerated import RootWordGenerated


class MainWindow(QMainWindow, Ui_MainWindow):
    """
    Class documentation goes here.
    """
    def __init__(self, queueBigData, queueCtls, parent=None):
        """
        Constructor
        
        @param parent reference to the parent widget
        @type QWidget
        """
        print (self)
        QMainWindow.__init__(self, parent)
        self.setupUi(self);
        self.data = "";
        self.queueBigData = queueBigData;
        self.queueCtls = queueCtls;
        self.Windows = self;
        #GlobalVariable.init()
        #GlobalVariable.eventHappen.valueChanged.connect(self.TextView)
        
        
        
    @pyqtSignature("")
    def on_RootWord_clicked(self):
        
        
        start_time = time.time()
        print("the Execution Time11:%s seconds " % (time.time() - start_time))
        rootWordThread = RootWordThread()
        rootWordThread.start()
        print("the Execution Time22:%s seconds " % (time.time() - start_time))
        
        
        
        """
        rootWordGenerated = RootWordGenerated()
        
            
        start_time = time.time()
        print("the Execution Time11:%s seconds " % (time.time() - start_time))
        quitRootWordThread = rootWordGenerated.RootWordGenerated()
            
        print("the Execution Time22:%s seconds " % (time.time() - start_time))
        
        """
        
        
        
    
    @pyqtSignature("")
    def on_Save_As_clicked(self):
        
        start_time = time.time()
        html = htmlDataAcquired()
        
        
        SaveDataToFile = QFileDialog.getSaveFileName(None,self.trUtf8 ("Save the data as a file"),\
       None, self.trUtf8("All Types(*.csv *.txt *doc *docx *xml *xmlx)"), None)
        self.data =html.htmlDataAcquired()
        self.data = re.sub(r'\\','',self.data)
        """
        print "**********************************************************************************"
        print(isinstance(self.data,str))
        print "**********************************************************************************"
        """
        
        
        self.View_Text.setText(self.data)
        
        print("the Execution Time1: %s seconds " % (time.time() - start_time))
        if SaveDataToFile != '':
            
            with open(SaveDataToFile,'w') as txt_file:
            #print "error:"
                txt_file.write(self.data.encode("UTF-8"))
                
                
        b = re.sub(r'\\','',self.data)
        print (b)
        print("the Execution Time2: %s seconds " % (time.time() - start_time))
        
    

    
    @pyqtSignature("")
    def on_OpenFile_triggered(self):
        """
        Slot documentation goes here.
        """
        
        
        saveAsThread = SaveAsThread(self.queueBigData)
        #saveAsThread = Thread(target =SaveAsThread,  args=[] ) 
        saveAsThread.start()
        
        displayInfoThread = DisplayInfoThread(self.queueCtls)
        displayInfoThread.start()

    def TextView(self, queueBigData):
        
        self.View_Text.setText(queueBigData.get());












        
#import time
from FileParser import FileParser
from threading import Thread
    
    




class SaveAsThread(Thread):
    def __init__(self, _queue):
        self._queue = _queue
        Thread.daemon = False
        self.quitSaveTheFile = False
        Thread.__init__(self)   
        #since we are overriding the Thread class, we need to first call its __init__() method.
        #self.Window = Window
       

    def run(self):
       while (not self.quitSaveTheFile):
            fileParser = FileParser(self._queue)
            start_time = time.time()
            print("the Execution Time11:%s seconds " % (time.time() - start_time))
        
        
       
            self.quitSaveTheFile = fileParser.FileParser()
            print("the Execution Time22:%s seconds " % (time.time() - start_time))            




class RootWordThread(Thread):
    def __init__(self):
        Thread.daemon = False
        Thread.__init__(self) 
        self.quitRootWordThread = False
        
    def run(self):
        #ImportError: cannot import name 'RootWordGenerated'
        rootWordGenerated = RootWordGenerated()
        while(not self.quitRootWordThread):
            
            start_time = time.time()
            print("the Execution Time11:%s seconds " % (time.time() - start_time))
            self.quitRootWordThread = rootWordGenerated.RootWordGenerated()
            
            print("the Execution Time22:%s seconds " % (time.time() - start_time))


class DisplayInfoThread(Thread):
    def __init__(self, _queue):
        self._queue = _queue
        Thread.daemon = False
        Thread.__init__(self)
    def run(self):
       
        while True:
            try:  
                string=self._queue.get(block=True, timeout=20)#接收消息  
                #self.Window.View_Text.setText(string)
            except queue.Empty: 
                print("Nothing to do!i will go home!")  
            
            time.sleep(0.5)





#!/usr/bin/python
from PyQt4.QtGui import QApplication

#No module named ui.MainWindow is an error--only the folder has a __init__.py can be called package
#A py file is a module

def main(queueBigData, queueCtls):
    import sys
    app = QApplication(sys.argv)
    #initializes and starts the Qt library so that it handles mouse movements, mouse button presses, and keyboard key presses
    wnd = MainWindow(queueBigData, queueCtls)
    wnd.show()
    status = app.exec_()   # run app, show window, wait for input
    sys.exit(status)
    #sys.exit(app.exec_())
    
 

if __name__ == '__main__':
    queueBigData=queue.Queue(10000)
    queueCtls = queue.Queue()
    main(queueBigData, queueCtls)
   
    
