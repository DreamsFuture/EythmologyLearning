from bs4 import BeautifulSoup
import socket
import urllib3
import time
import re
import GlobalVariable



class RootWordHtmlParser:
    def __init__(self):
        self.http = urllib3.PoolManager()

    def RootWordHtmlParser(self, word):
        
        word = re.sub(r'\ ', '', word)
        word  = word.rstrip('\n') 
        GlobalVariable.rootWord[0] = word
        print("GlobalVariable.rootWord[1]:  "+ word)
        
        webSite = ''.join([r'http://www.etymonline.com/index.php?term=', word, r'&allowed_in_frame=0'])
        GlobalVariable.rootWord[12] = webSite
        
        phoneticwebsite = ''.join([r'http://dict.cn/', word])        
        levelwebsite = ''.join([r'http://www.iciba.com/',word])
        loopNum = 0
        
        
        
        
        print("------------------   sucessful goes to the Internet   ---------------------------")
        #-----------------------------------------------------------------------------------------------------------------------------------------
        try:
            #html = urllib3.urlopen(webSite, timeout=15).read()
            sourceWordHtml = self.http.request('GET', webSite, timeout=15)
            phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 15)
            levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
        
        except urllib3.exceptions.HTTPError:
            print ("catch a exceptions : urllib3.exceptions.HTTPError")
            if sourceWordHtml.data == '' or sourceWordHtml.data == ' *' or \
                        phoneticHtml.data == ''or phoneticHtml.data == ' *':
                        return False  
            """
            loopNum = 0
            while sourceWordHtml.data == '' or sourceWordHtml.data == ' *' or \
                        phoneticHtml.data == ''or phoneticHtml.data == ' *':
                try:
                    
                    sourceWordHtml = self.http.request('GET', webSite, timeout=5)
                    phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 5)
                    levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
                except urllib3.exceptions.HTTPError:
                    pass
                except urllib3.exceptions.ReadTimeoutError:
                    pass
                except urllib3.exceptions.MaxRetryError:
                    pass
                time.sleep(0.5)
                loopNum = loopNum + 1
                if loopNum == 5 :
                    break
                """
               
                    
        except urllib3.exceptions.ReadTimeoutError:
            print ("catch a exceptions    :urllib3.exceptions.ReadTimeoutError")
            loopNum = 0
            
            while sourceWordHtml.data == '' or sourceWordHtml.data == ' *' or \
                        phoneticHtml.data == ''or phoneticHtml.data == ' *':
                try:
                
                    sourceWordHtml = self.http.request('GET', webSite, timeout=5)
                    phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 5)
                    levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
                except urllib3.exceptions.HTTPError:
                    pass
                except socket.timeout:
                    pass
                except socket.error:
                    pass
                time.sleep(0.5)
                
                loopNum = loopNum + 1
                if loopNum == 5 :
                    break
            
        except urllib3.exceptions.MaxRetryError:
            print("catch a exceptions : urllib3.exceptions.MaxRetryError")
            
            #print ( sourceWordHtml.data)
            
          
            
        print("-------------------   sucessful goes into the html texts parsed  ---------------------------")
            
            
            
        #------------------------------------------- parser the html texts for another use---------------------------------------------------------------------------
        time.sleep(0.3)
        soup = BeautifulSoup(sourceWordHtml.data, "html.parser")
        soup2 = BeautifulSoup(phoneticHtml.data, "html.parser")
        soup3 =  BeautifulSoup(levelHtml.data, "html.parser")
        
        
        
        fullResult = soup.find('div', attrs={'id':'dictionary'})
        #print(fullResult.get_text())
        #print(word)
        if 'PIE' in fullResult.get_text():      #To determine whether an element exists in the list, the keyword "in" the list name
            
            #print(fullResult.get_text())
            print("word:"+word)
            
            for lineText in fullResult.get_text().split("\n"):
                wordTest = re.sub(r'\(.*', '', lineText)
                wordTest = re.sub('\ ', '', wordTest)
                
                if (word == str(wordTest)):

                    lineText = lineText[lineText.find("(")-1:lineText.find(")")+1]
                    #lineText = re.findall('\(.*?\)',lineText)
                    GlobalVariable.rootWord[1] = lineText
                elif(re.match('.*PIE.*', lineText)):
                    #print(lineText.find("PIE"))
                    #print(lineText.find("("))
                    PIENum = lineText.find("PIE")
                    END = len(lineText)
                    firstColon = lineText.find('\"', PIENum, END)
                    secondColon = lineText.find('\"', firstColon, END)
                    PIEWord = lineText[lineText.find("PIE"):lineText.find("(", secondColon,END)]
                    
                    GlobalVariable.rootWord[6] = PIEWord
                    GlobalVariable.rootWord[8] = lineText
                    print("PIEWord:"+PIEWord)
                    #print(lineText)
                else:
                    pass
            # acquire the phonetic and englishMeaning of the word from the Internet
            if soup2.div.bdo  is not None :
                englishMeaning = soup2.find('div', attrs={'class':'layout en'})
                phonetic = soup2.find('div', attrs={'class':'phonetic'})
                chineseMeaning = soup2.find('ul', attrs={'class':'dict-basic-ul'})
                if englishMeaning and phonetic is not None:
                    englishMeaning = englishMeaning.get_text()
                    phonetic = phonetic.get_text()
                    
                    
                    
                    phonetic = re.sub(r'\t', '', phonetic)
                    phonetic = re.sub(r'\n', '', phonetic)
                    phonetic = re.sub(r'\\','',phonetic)
                    GlobalVariable.rootWord[3] = phonetic
                    
                    
                    englishMeaning = re.sub(r'\t', '', englishMeaning)
                    englishMeaning = re.sub(r'\\','',englishMeaning)
                    englishMeaning = re.sub(r'\n\n','',englishMeaning)
                    GlobalVariable.rootWord[4] = englishMeaning
 
                else:
                    GlobalVariable.rootWord[3] = None
                    #print(result2_sub)
                    GlobalVariable.rootWord[4] = None
                    
                if chineseMeaning is not None:
                    chineseMeaning = chineseMeaning.get_text()
                    
                    """
                    if chineseMeaning.find("[a-z]"):
                        for i in chineseMeaning:
                            if re.match('[a-z]', chineseMeaning):
                    """            
                    chineseMeaning = re.sub(r'\n', '', chineseMeaning)
                    chineseMeaning = re.sub(r'if.*', '', chineseMeaning)
                    chineseMeaning = re.sub(r'\ ', '', chineseMeaning)
                    chineseMeaning = re.sub(r'\t', '', chineseMeaning)
                    GlobalVariable.rootWord[11] = chineseMeaning
                else:
                    GlobalVariable.rootWord[11] = None
                
            else:
                pass
            
            # parse the GRE/TOEFL/CET-6/CET-4 level of the word
            if soup3.find('div', attrs={'class':'base-level'})is not None:

                levelresult = soup3.find('div', attrs={'class':'base-level'})
                
                levelresult = levelresult.get_text()
                levelresult = re.escape(levelresult)
                levelresult = re.sub(r'\\','',levelresult)
                levelresult = re.sub(r'\ ', '', levelresult)
                levelresult = re.sub(r'\n', '', levelresult)
                GlobalVariable.rootWord[2] = levelresult
            else:
                GlobalVariable.rootWord[2] = None
                """
                if soup3.find('ul', attrs={'class':'base-list switch_part'}) is not None:
                    chineseMeaning = soup3.find('ul', attrs={'class':'base-list switch_part'})
                    chineseMeaning = chineseMeaning.get_text()
                    
                    chineseMeaning = re.escape(chineseMeaning)
                    chineseMeaning = re.sub(r'\\','',chineseMeaning)
                    chineseMeaning = re.sub(r'\ ', '', chineseMeaning)
                    
                    print(chineseMeaning)
                    GlobalVariable.rootWord[11] = chineseMeaning
                else:
                    #GlobalVariable.rootWord[11] = None
                    pass
                """

            return True
        else:
            return False
