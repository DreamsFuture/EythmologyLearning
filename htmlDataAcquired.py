from abc import ABCMeta
import urllib3
import re
import time
import socket
#from PyQt4.QtCore import QString
#from PyQt4.QtGui import QFileDialog
from bs4 import BeautifulSoup
#import MyException 

class htmlDataAcquired:
    def __init__(self):  
        self.http = urllib3.PoolManager()
        
    
   
    def htmlDataAcquired(self, webSite, whichToParser):
        # definite a class to a variable,  These are problems need to be solved!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        html = ABCMeta
        print(type(html))
        print(isinstance(html, ABCMeta))
        print("*************type of the html :"+str(type(html))+"*************************")
        loopNum = 0
        
        #time.sleep(1)
        try:
            #html = urllib3.urlopen(webSite, timeout=15).read()
            html = self.http.request('GET', webSite, timeout=15)
            #print("*************type of the html :"+type(html)+"*************************")
            #print "before beautiful" 
        #  If the time of socket for getting the html datas is out, do the following codes
        except socket.timeout:
            print ("socket.timeout.print first:" + html)
            # the "html" is none when the code is executed now
            while (html.data == '' or html.data == ' *'):
                try:
                    print ('1   :html = urllib3.urlopen(webSite, timeout=5).read()')
                    #html = urllib3.urlopen(webSite, timeout=5).read()
                    html = self.http.request('GET', webSite, timeout=5)
                except urllib3.exceptions.HTTPError:
                    pass
                except socket.timeout:
                    pass
                except socket.error:
                    pass
                
                loopNum = loopNum + 1
                #print (loopNum)
                
                time.sleep(0.5)                
                #errorDisplay = ''.join([ '\n', str(e) ])
            
                #print('timeout***socket.timeout***\n' + html + errorDisplay)
                if loopNum == 5 :
                    break
            print ( html.data)
            # continue     *****    Syntax error "'continue' not properly in loop"
        
        #  If the url passed by is wrong, executed the following codes
        except socket.gaierror:
            loopNum = loopNum + 1
            while html.data == '' or html.data == ' *':
                
                # avoid the exception to deadly loop in the situation
                try:
                    print ("2 ++socket.gaierror++   :html = urllib3.urlopen(webSite, timeout=5).read()")
                    html = self.http.request('GET', webSite, timeout=5)
                    
                # avoid the exception raised in the while loop, and give anther change to acquire the html data 
                except urllib3.exceptions.HTTPError:
                    pass
                except socket.timeout:
                    pass
                except socket.gaierror:
                    pass
                time.sleep(0.5)
                
                #errorDisplay = ''.join([ '\n', str(e) ])
                print ('urllib3.URLError'+ html.data )
                if loopNum == 10 :
                    break
            print ( "socket.gaierror exception:"+html.data)
        except urllib3.exceptions.HTTPError:
            loopNum = loopNum + 1
            #while html.data == '' or html.data == ' *':
                # avoid the exception to deadly loop in the situation
            try:
                print ("3    :html = urllib3.urlopen(webSite, timeout=5).read()")
                html = self.http.request('GET', webSite, timeout=5)
            except urllib3.exceptions.HTTPError:
                pass
            except socket.timeout:
                pass
            except socket.error:
                pass
            time.sleep(0.5)
                
                #errorDisplay = ''.join([ '\n', str(e) ])
                #print ('urllib2.URLError'+ html + errorDisplay)
                #if loopNum == 10 :
                    #break
            print ( html.data)
        
        
        soup = BeautifulSoup(html.data, "html.parser")
        #print "after beautiful"
        time.sleep(0.3)
        
        if whichToParser == 1:
        
            if soup.dd is not None:
                fullResult = soup.find('div', attrs={'id':'dictionary'})
                fullResult = fullResult.get_text()
                return re.escape(fullResult)
            else:
                fullResult = ' '
            #print (result)
            return ''
        elif whichToParser == 2:
            
            if soup.div.bdo  is not None :
                result = soup.find('div', attrs={'class':'layout en'})
                result2 = soup.find('div', attrs={'class':'phonetic'})
                if result and result2 is not None:
                    result = result.get_text()
                    result2 = result2.get_text()
                    result_sub = re.sub(r'\t', '', result)
                    result2_sub = re.sub(r'\t', '', result2)
                    #print (result_sub, result2_sub)
                    return result2_sub, result_sub
                else:
                    return '', ''
            else:
                return '', ''
        
        elif whichToParser ==3:
            
            if soup.find('div', attrs={'class':'base-level'})  is not None:
                result = soup.find('div', attrs={'class':'base-level'})
                result = result.get_text()
                result = re.escape(result)
            else:
                result = ' '
            print (result)
            return result
        
        else:
            return ''
        
        """
        print "**********************************************************************************"
        print result
        print "**********************************************************************************"
        print(isinstance(result,str))
        print "
        **********************************************************************************"
        """
 
        #print result
        

        
        
        #a = re.escape(result)
        
        """
        #re = str(result)
        print(isinstance(a,str))
        print "**********************************************************************************"
        print a
        print "**********************************************************************************"
        """
        
        #return a
    
    """
    #result = filter(visible, data)
    @retry(urllib2.URLError, tries=4, delay=3, backoff=2)
    def urlopen_with_retry():
        return urllib2.urlopen("http://example.com")

    """
