import time
from FileParser import FileParser
from threading import Thread




class SaveAsThread(Thread):
   def __init__(self):
       Thread.__init__(self)
       

   def run(self):
       while True:
            fileParser = FileParser()
            start_time = time.time()
            print("the Execution Time11:%s seconds " % (time.time() - start_time))
        
        
       
            fileParser.FileParser()
            print("the Execution Time22:%s seconds " % (time.time() - start_time))
