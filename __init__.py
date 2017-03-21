#!/usr/bin/python
from PyQt4.QtGui import QApplication
from htmlParser import MainWindow
#No module named ui.MainWindow is an error--only the folder has a __init__.py can be called package
#A py file is a module

def main():
    import sys
    app = QApplication(sys.argv)
    #initializes and starts the Qt library so that it handles mouse movements, mouse button presses, and keyboard key presses
    wnd = MainWindow()
    wnd.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
