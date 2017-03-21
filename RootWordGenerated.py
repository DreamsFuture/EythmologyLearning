import re
import sqlite3

from PyQt4.QtGui import QFileDialog
from RootWordHtmlParser import RootWordHtmlParser
import GlobalVariable 



class RootWordGenrated:
    def __init__(self):
        self.ID = 0
        
    def RootWordGenrated(self):
        rootWordHtmlParser = RootWordHtmlParser()
        
        
        conn = sqlite3.connect('EythmologyRootWord.db')
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
        tableName = 'EythmologyRootWord'
        OpenOriginalFile = QFileDialog.getOpenFileName(None, "Select and Open a file to parse", \
                                        None, "*.txt *.xml *.doc *.docx *.xmlx", QFileDialog.ShowDirsOnly)
        
        if  OpenOriginalFile != '':
            with open(OpenOriginalFile, "r",encoding="utf8") as fin:
                for line in fin:
                    
                    if re.match('a..*', line) or re.match('n..*', line) or re.match('adj..*', line) or  re.match('adv..*', line) or  re.match('prep..*', line)\
                             or re.match('v..*', line) or  re.match('conj..*', line):
                        continue
                    
                    isAsciiString = all(ord(char) < 128 for char in str(line))
                    if isAsciiString:
                        pass
                    else:
                        continue
                    
                    
                    InsertOrNot = rootWordHtmlParser.RootWordHtmlParser(line)
                    if InsertOrNot:
                        
                        conn.execute("INSERT INTO "+tableName+" VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)",\
                                        (self.ID,  GlobalVariable.GlobalVariable.rootWord[0], GlobalVariable.rootWord[1], GlobalVariable.rootWord[2],GlobalVariable.rootWord[3],GlobalVariable.rootWord[4] ,\
                                        GlobalVariable.rootWord[5],  GlobalVariable.rootWord[6], GlobalVariable.rootWord[7], GlobalVariable.rootWord[8],GlobalVariable.rootWord[9],GlobalVariable.rootWord[10], \
                                        GlobalVariable.rootWord[11], GlobalVariable.rootWord[12], GlobalVariable.rootWord[13]));
                        self.ID += 1
                        if self.ID%10 == 0:
                            conn.commit()
                        for i in range(0, 14):
                            GlobalVariable.rootWord.append('')
                    else:
                        pass
                    
                conn.close()
                    
                    
