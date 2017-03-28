
#!/usr/bin/env python
# coding=utf -8


import sqlite3

from PyQt4.QtGui import QFileDialog
from RootWordHtmlParser import RootWordHtmlParser
import GlobalVariable 



class RootWordGenerated:
    def __init__(self):
        self.ID = 1
        self.rootWord = []
        for i in range(0, 14):
            self.rootWord.append('')
  
    def RootWordGenerated(self):
        rootWordHtmlParser = RootWordHtmlParser()
        #conn = sqlite3.connect(":memory:")
        conn = sqlite3.connect('EythmologyRootWord.db')
        conn.execute('DROP TABLE EythmologyRootWord')
        conn.execute('''create table if not exists  EythmologyRootWord
            (ID                             INT PRIMARY KEY     NOT NULL,
            WORD                          TEXT,
            PART_OF_SPEECH              TEXT,
            LEVEL                          TEXT,
            PHONETIC                     TEXT,
            ENGLISH_MEANING           TEXT ,
            PLUS_SOURCE                 TEXT,
            WORD_ROOT                   TEXT,
            SIMPLE_SOURCE              TEXT,
            ALL_SOURCE_MEANNING     TEXT,
            WORD_FREQUENT            TEXT,
            EXAMPLE_SENTENCE        TEXT,
            CHINESE_MEANNING         TEXT,
            WORD_WEBSITE               TEXT,
            VOTES                          TEXT
            );''')
        tableName = 'EythmologyRootWord'
        OpenOriginalFile = QFileDialog.getOpenFileName(None, "Select and Open a file to parse", \
                                        None, "*.txt *.xml *.doc *.docx *.xmlx", QFileDialog.ShowDirsOnly)
        
        if  OpenOriginalFile != '':
            with open(OpenOriginalFile, "r",encoding="utf8") as fin:
                for line in fin:
                    line = line.rstrip('\n')
                    
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
                        print("------------------- self.ID:--------------------    "+str(self.ID))
                        
                        self.ID += 1
                        
                        if self.ID%10 == 0:
                            conn.commit()
                        
                        # initalize the global variable 
                        for i in range(0, 14):
                            if i == 5:
                                continue
                            
                            print("GlobalVariable.rootWord[%s] : " %str(i))
                            print( GlobalVariable.rootWord[i])
                            GlobalVariable.rootWord[i] = ''
                    else:
                        continue
            conn.commit()
            conn.close()
            return True
        else:
            return True
                    
