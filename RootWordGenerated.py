
import sqlite3

from PyQt4.QtGui import QFileDialog
from RootWordHtmlParser import RootWordHtmlParser
import GlobalVariable 



class RootWordGenerated:
    def __init__(self):
        self.ID = 0
        self.rootWord = []
        for i in range(0, 14):
            self.rootWord.append('')
  
    def RootWordGenerated(self):
        rootWordHtmlParser = RootWordHtmlParser()
        conn = sqlite3.connect(":memory:")
        #conn = sqlite3.connect('EythmologyRootWord.db')
        #conn.execute('DROP TABLE EythmologyRootWord')
        conn.execute('''create table if not exists  EythmologyRootWord
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
        tableName = 'EythmologyRootWord'
        OpenOriginalFile = QFileDialog.getOpenFileName(None, "Select and Open a file to parse", \
                                        None, "*.txt *.xml *.doc *.docx *.xmlx", QFileDialog.ShowDirsOnly)
        
        if  OpenOriginalFile != '':
            with open(OpenOriginalFile, "r",encoding="utf8") as fin:
                for line in fin:
                    line = line.rstrip('\n')
                    
                    """
                    if  re.match('n..*', line) or re.match('adj..*', line) or  re.match('adv..*', line) or  re.match('prep..*', line)\
                             or re.match('v..*', line) or  re.match('conj..*', line):
                        continue
                    """
                    
                    isAsciiString = all(ord(char) < 128 for char in str(line))
                    #print("line: ----    "+line+"   is   "+str(isAsciiString))
                    if isAsciiString:
                        pass
                    else:
                        continue
                    
                    
                    InsertOrNot = rootWordHtmlParser.RootWordHtmlParser(line)
                    if InsertOrNot:
                        conn.execute("INSERT INTO "+tableName+" VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",\
                                        (self.ID,  GlobalVariable.rootWord[0], GlobalVariable.rootWord[1], GlobalVariable.rootWord[2],GlobalVariable.rootWord[3],GlobalVariable.rootWord[4] ,\
                                        GlobalVariable.rootWord[5],  GlobalVariable.rootWord[6], GlobalVariable.rootWord[7], GlobalVariable.rootWord[8],GlobalVariable.rootWord[9],GlobalVariable.rootWord[10], \
                                        GlobalVariable.rootWord[11], GlobalVariable.rootWord[12], GlobalVariable.rootWord[13]));
                        print("self.ID:"+str(self.ID))
                        
                        self.ID += 1
                        
                        if self.ID%10 == 0:
                            conn.commit()
                        for i in range(0, 14):
                            print("GlobalVariable.rootWord : "+ GlobalVariable.rootWord[i])
                            GlobalVariable.rootWord.append('')
                    else:
                        continue
                    
            conn.close()
            return True
        else:
            return True
                    
