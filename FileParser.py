import re
import time

#import GlobalVariable
import os
import psutil
import sqlite3

from htmlDataAcquired import htmlDataAcquired
#from PyQt4.QtCore import QString
from PyQt4.QtGui import QFileDialog, QWidget

class FileParser(QWidget):
    def __init__(self, queue):
        super(FileParser, self).__init__()
        self.queue = queue
        
        #self.MainWindow = MainWindow
        self.meaning = ''
        self.word = ''
        self.wordNum = 0
        
        self.phonetic = ''
        self.phoneticNum = 0
        
        self.levelNum = 0
        # get the object of the class htmlDataAcquired
        self.html = htmlDataAcquired()
        
        self.htmlData = ''
        self.englishMean = 0
        self.englishMeaning = ''
        self.data = ''
        self.font = 0
        
        self.ID = 0
        
        # use the array data structures for indexing in the other places
        self.writeToFiles = []
        self.writeToDatabase = []
        for i in range(0, 14):
            self.writeToFiles.append('')
            self.writeToDatabase.append('')
        self.writingNow = 0
        self.wordForWeb = ''
        
    def FileParser(self):
        
        # CONNECTS A DATABASE FILE
        conn = sqlite3.connect('Eythmology.db')
        conn.execute('DROP TABLE SOURCEWORD')
        conn.execute('''create table if not exists  SOURCEWORD
            (ID                             INT PRIMARY KEY     NOT NULL,
            WORD                          TEXT,
            PARTOFSPEECH              TEXT,
            LEVEL                          TEXT,
            PHONETIC                     TEXT,
            ENGLISHMEANING           TEXT ,            
            PLUSSOURCE                 TEXT,
            WORDROOT                   TEXT,
            SIMPLESOURCE              TEXT,
            ALLSOURCEMEANNING     TEXT,
            WORDFREQUENT            TEXT,
            EXAMPLESENTENCE        TEXT,
            CHINESEMEANNING         TEXT,
            WORDWEBSITE               TEXT,
            VOTES                          TEXT
            );''')
        tableName = 'SOURCEWORD'
        OpenOriginalFile = QFileDialog.getOpenFileName(None, "Select and Open a file to parse", \
                                        None, "*.txt *.xml *.doc *.docx *.xmlx", QFileDialog.ShowDirsOnly)
        OpenSaveFile = QFileDialog.getOpenFileName(None, "Select and Open a file to save the data", \
                                        None, "*.txt *.xml *.doc *.docx *.xmlx", QFileDialog.ShowDirsOnly)
        if  OpenOriginalFile and OpenSaveFile != '':
            #with open(OpenOriginalFile,"r") as f:
                #text = f.readlines()
            
            #with open(OpenOriginalFile,"r") as f:
                #Linetext = f.readlines()
            

            with open(OpenOriginalFile, "r",encoding="utf8") as fin:
               with open(OpenSaveFile, "w",encoding="utf8") as fout:
                    #datadict = pickle.load(fin, encoding='latin1')
                    for line in fin:
                        
                        self.LineTextParser(line)
                        #print "with open:" + self.writeToFiles
                        #self.data = re.sub(r'\\','',self.data)
                        time.sleep(0.5)
                        if self.writingNow == 1:                            
                            
                            conn.execute("INSERT INTO "+tableName+" VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)",\
                                        (self.ID,  self.writeToDatabase[0], self.writeToDatabase[1], self.writeToDatabase[2],self.writeToDatabase[3],self.writeToDatabase[4] ,\
                                        self.writeToDatabase[5],  self.writeToDatabase[6], self.writeToDatabase[7], self.writeToDatabase[8],self.writeToDatabase[9],self.writeToDatabase[10], \
                                        self.writeToDatabase[11], self.writeToDatabase[12], self.writeToDatabase[13]));
                            self.ID += 1
                            if self.ID%10 == 0:
                                conn.commit()
                            print("+++++++++++++++++++++++++++self.ID:"+str(self.ID)+"++++++++++++++++++++++++++++++++++")
                            for i in range(0, 5):
                                time.sleep(2)
                                print ("self.writeToFiles[%s]:"  %i)
                                #print(type(self.writeToFiles[i]))
                                print (self.writeToFiles[i])
                                
                                self.queue.put(self.writeToFiles[i], block = True, timeout = None )
                                #self.ViewBrower.setText(self.writeToFiles[i])
                                #GlobalVariable.eventHappen = ~ GlobalVariable.eventHappen
                                fout.write(str(self.writeToFiles[i]))
                                
                            # initial the data again   using this method can decrease a lot of usage of memory, beacuse we won't definite many arrays.
                            self.writingNow = 0
                            for i in range(0, 14):
                                self.writeToDatabase[i] = ''
                                self.writeToFiles[i] = ''
                        else:
                            pass
                    conn.close()
            return True
        else:
            return True

    
                
    def LineTextParser(self, LineText):
                
        if re.match(r'\+', LineText):
            
            self.wordNum = 1
            self.word = re.sub(r'\+', '', LineText) 
            time.sleep(0.5)
            self.wordForWeb = self.word.rstrip('\n')
            
            #url = "https://www.reddit.com/r/{}.json?limit=1".format(self.wordForWeb)
            wordWebSite = ''.join([r'http://www.etymonline.com/index.php?term=', self.wordForWeb, r'&allowed_in_frame=0'])
            
            # display the level of the word
            self.levelNum = 3
            time.sleep(0.3)
            levelWebSite = ''.join([r'http://www.iciba.com/', self.wordForWeb])
            level = self.html.htmlDataAcquired(levelWebSite, self.levelNum)
            # Problem:    TypeError: Can't convert 'bytes' object to str implicitly
            #print(level)
            #level = str(level.encode('utf8'))
            
            level = re.sub(r'\\','',level)
            level = re.sub(r'\ ', '', level)
            level = re.sub(r'\n', '', level)
            self.writeToDatabase[1] = level
            self.writeToDatabase[0] = self.word.rstrip('\n')
            
            self.word = ''.join([self.word.rstrip('\n'),'    ', str(level),'\n'])
            self.word = ''.join([self.word, '*******************************************'])
            self.word = ''.join(['\n','              ', self.word]) 
            self.writeToFiles[0] = self.writeToFiles[0] + self.word
           
            print ("***************************||   "+ self.word.strip('\n') + "   ||*********************************")
            self.htmlData = ''
            #print "-------------------------  put the webSite into beautifulsoup and get the datas...  ----------------------------------"
            
            # executed time 
            start_time = time.time()
            print("The Html Datas Acquired Program Execution Time1: %s seconds " % (time.time() - start_time))
            self.htmlData = self.html.htmlDataAcquired(wordWebSite, self.wordNum)
            print("The Html Datas Acquired Program  Execution Time2: %s seconds " % (time.time() - start_time))
            
            # get the usage of the project ( memory information )
            process = psutil.Process(os.getpid())
            print ("The Usage of the Python Project :" +  str(process.memory_info().rss))
            
            
            #print "+++++++++++++++++++++++  Get The Datas Successfully  +++++++++++++++++++++++++++++"
            i = 0
            while self.htmlData == '':
                # the following codes do not have any chance to be executed
                i = i + 1
                time.sleep(2)
                print ("----------------------" + str(i) + "------------------------")
                try:
                    self.htmlData = self.html.htmlDataAcquired(wordWebSite)
                except IOError:
                    time.sleep(2)
                    #self.htmlData = self.html.htmlDataAcquired(webSite)
                
            #print (self.word)
            #return self.word
            
        elif re.match(r'\#',LineText):
            
            self.meaning = re.sub(r'\#','',LineText)
            #self.meaning = ''.join([self.meaning, '\n'])
            self.writeToDatabase[10] = self.writeToDatabase[10] + self.meaning
            
            self.writeToFiles[4] = self.writeToFiles[4] + self.meaning
            
            #print (self.writeToFiles[4])
            #print ("**********************************************")
            #return self.meaning
            
        elif re.match(r'\&', LineText):
            phoneticWebSite = ''.join([r'http://dict.cn/', self.wordForWeb])
            self.phoneticNum = 2
            if phoneticWebSite is not None:
                
                (self.phonetic,  self.englishMeaning) = self.html.htmlDataAcquired(phoneticWebSite, self.phoneticNum)
                self.phonetic = self.phonetic
                self.englishMeaning = self.englishMeaning
                
                self.phonetic = re.sub(r'\\','',self.phonetic)
                print("+++++++++++++\n"+self.phonetic)
                print("self.phonetic is :"+str(type(self.phonetic)))
                self.englishMeaning =  re.sub(r'\\','',self.englishMeaning)
                
                self.writeToDatabase[2] = self.writeToDatabase[2]+self.phonetic
                self.writeToDatabase[3] = self.writeToDatabase[3]+self.englishMeaning
                
                self.writeToFiles[1] = self.writeToFiles[1]+self.phonetic
                self.writeToFiles[2] = self.writeToFiles[2]+self.englishMeaning
                
            else:
                pass
            """
            self.phonetic = re.sub(r'\&', '', LineText)
            self.phonetic = self.phonetic.rstrip('\n')
            self.phonetic = ''.join(['[', self.phonetic ])
            self.phonetic = ''.join([ self.phonetic,']' ])
            self.phonetic = ''.join([ self.phonetic,'\n' ])
            self.phonetic = ''.join([ '\n' , self.phonetic])
            """
            
            #return self.phonetic
        elif re.match(r'\@', LineText):
            #while(self.htmlData == self.upDate):
                #time.sleep(2000)
            self.htmlData = self.htmlData.rstrip('\n')
            self.htmlData = re.sub(r'\\','',self.htmlData)
            
            self.writeToDatabase[4] = self.writeToDatabase[4] + self.htmlData
            
            self.htmlData = ''.join([ self.htmlData,'\n' ])
            self.htmlData = ''.join(['\n' ,  self.htmlData])
            #self.htmlData = str(self.htmlData.encode('utf8'))
            
            self.writeToFiles[3] = self.writeToFiles[3]+self.htmlData
            
            #return self.htmlData.encode('utf8')
        elif re.match(r'\$', LineText) :
            
            self.writingNow = 1
            #return ''
        else:
            pass
