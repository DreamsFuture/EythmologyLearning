from bs4 import BeautifulSoup
import socket
import urllib3
import time
import re

class RootWordHtmlParser:
    def __init__(self):
        self.http = urllib3.PoolManager()

    def RootWordHtmlParser(self, word):
        
        word = re.sub(r'\ ', '', word)
        word  = word.rstrip('\n') 
        
        webSite = ''.join([r'http://www.etymonline.com/index.php?term=', word, r'&allowed_in_frame=0'])        
        phoneticwebsite = ''.join([r'http://dict.cn/', word])        
        levelwebsite = ''.join([r'http://www.iciba.com/',word])
        loopNum = 0
        
        try:
            #html = urllib3.urlopen(webSite, timeout=15).read()
            sourceWordHtml = self.http.request('GET', webSite, timeout=15)
            phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 15)
            levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
        except socket.timeout:
            print ("socket.timeout.print first:" + sourceWordHtml)
            # the "sourceWordHtml" is none when the code is executed now
            while (sourceWordHtml.data == '' or sourceWordHtml.data == ' *'or phoneticHtml.data == '' or phoneticHtml.data == ' *'):
                try:
                    print ('1   :sourceWordHtml = urllib3.urlopen(webSite, timeout=5).read()')
                    #sourceWordHtml = urllib3.urlopen(webSite, timeout=5).read()
                    sourceWordHtml = self.http.request('GET', webSite, timeout=5)
                    phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 5)
                    levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
                except urllib3.exceptions.HTTPError:
                    pass
                except socket.timeout:
                    pass
                except socket.error:
                    pass
                
                loopNum = loopNum + 1
                time.sleep(0.5)                
                if loopNum == 5 :
                    break
            print ( sourceWordHtml.data)
            
        except socket.gaierror:
            loopNum = loopNum + 1
            while sourceWordHtml.data == '' or sourceWordHtml.data == ' *' or phoneticHtml.data == ''or phoneticHtml.data == ' *':
                
                # avoid the exception to deadly loop in the situation
                try:
                    print ("2 ++socket.gaierror++   :sourceWordHtml = urllib3.urlopen(webSite, timeout=5).read()")
                    sourceWordHtml = self.http.request('GET', webSite, timeout=5)
                    phoneticHtml = self.http.request('GET', phoneticwebsite, timeout = 5)
                    levelHtml = self.http.request('GET', levelwebsite, timeout = 15)
                # avoid the exception raised in the while loop, and give anther change to acquire the sourceWordHtml data 
                except urllib3.exceptions.HTTPError:
                    pass
                except socket.timeout:
                    pass
                except socket.gaierror:
                    pass
                time.sleep(0.5)
                print ('urllib3.URLError'+ sourceWordHtml.data )
                if loopNum == 10 :
                    break
            print ( "socket.gaierror exception:"+sourceWordHtml.data)
        except urllib3.exceptions.HTTPError:
            loopNum = loopNum + 1
            try:
                print ("3    :sourceWordHtml = urllib3.urlopen(webSite, timeout=5).read()")
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
            print ( sourceWordHtml.data)
        
        time.sleep(0.3)
        soup = BeautifulSoup(sourceWordHtml.data, "sourceWordHtml.parser")      
        soup2 = BeautifulSoup(phoneticHtml.data, "phoneticHtml.parser")        
        soup3 =  BeautifulSoup(levelHtml.data, "levelHtml.parser")
        
        fullResult = soup.find('div', attrs={'id':'dictionary'})
        
        if re.match('.*PIE.*', fullResult.get_text()):
            import GlobalVariable
            
            GlobalVariable.rootWord[1] = word
        
            for lineText in fullResult.get_text().split("\n"):
                wordTest = re.sub(r'\(.*', '', lineText)
                wordTest = re.sub('\ ', '', wordTest)
                if (word == str(wordTest)):

                    lineText = lineText[lineText.find("(")-1:lineText.find(")")+1]
                    #lineText = re.findall('\(.*?\)',lineText)
                    GlobalVariable.rootWord[2] = lineText
                elif(re.match('.*PIE.*', lineText)):
                    print(lineText.find("PIE"))
                    print(lineText.find("("))
                    PIENum = lineText.find("PIE")
                    END = len(lineText)
                    firstColon = lineText.find('\"', PIENum, END)
                    secondColon = lineText.find('\"', firstColon, END)
                    PIEWord = lineText[lineText.find("PIE"):lineText.find("(", secondColon,END)]
                    GlobalVariable.rootWord[7] = PIEWord
                    GlobalVariable.rootWord[9] = lineText
                else:
                    pass
            
            if soup2.div.bdo  is not None :
                result = soup2.find('div', attrs={'class':'layout en'})
                result2 = soup2.find('div', attrs={'class':'phonetic'})
                if result and result2 is not None:
                    result = result.get_text()
                    result2 = result2.get_text()
                    result_sub = re.sub(r'\t', '', result)
                    result2_sub = re.sub(r'\t', '', result2)
                    
                    result2_sub = re.sub(r'\\','',result2_sub)
                    result_sub = re.sub(r'\\','',result_sub)
                    #print (result_sub, result2_sub)
                    GlobalVariable.rootWord[4] = result2_sub
                    GlobalVariable.rootWord[5] = result_sub
                    
                else:
                    pass
            else:
                pass
            
            
            if soup3.find('div', attrs={'class':'base-level'})  is not None:
                levelresult = soup3.find('div', attrs={'class':'base-level'})
                levelresult = levelresult.get_text()
                levelresult = re.escape(levelresult)
                levelresult = re.sub(r'\\','',levelresult)
                levelresult = re.sub(r'\ ', '', levelresult)
                levelresult = re.sub(r'\n', '', levelresult)
                GlobalVariable.rootWord[3] = levelresult
            else:
                result = ' '
            print (result) 
            return True
        else:
            return False        
        
        
