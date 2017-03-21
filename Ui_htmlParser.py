# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'E:\htmlParser\htmlParser.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(1035, 700)
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.Save_As = QtGui.QPushButton(self.centralWidget)
        self.Save_As.setGeometry(QtCore.QRect(830, 60, 121, 28))
        self.Save_As.setObjectName(_fromUtf8("Save_As"))
        self.lineEdit = QtGui.QLineEdit(self.centralWidget)
        self.lineEdit.setGeometry(QtCore.QRect(30, 60, 781, 31))
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.View_Text = QtGui.QTextBrowser(self.centralWidget)
        self.View_Text.setGeometry(QtCore.QRect(30, 360, 951, 281))
        self.View_Text.setObjectName(_fromUtf8("View_Text"))
        self.dateTimeEdit = QtGui.QDateTimeEdit(self.centralWidget)
        self.dateTimeEdit.setGeometry(QtCore.QRect(840, 10, 194, 22))
        self.dateTimeEdit.setObjectName(_fromUtf8("dateTimeEdit"))
        self.lcdNumber = QtGui.QLCDNumber(self.centralWidget)
        self.lcdNumber.setGeometry(QtCore.QRect(750, 10, 64, 23))
        self.lcdNumber.setObjectName(_fromUtf8("lcdNumber"))
        self.RootWord = QtGui.QPushButton(self.centralWidget)
        self.RootWord.setGeometry(QtCore.QRect(40, 150, 75, 23))
        self.RootWord.setObjectName(_fromUtf8("RootWord"))
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 1035, 23))
        self.menuBar.setObjectName(_fromUtf8("menuBar"))
        self.File = QtGui.QMenu(self.menuBar)
        self.File.setObjectName(_fromUtf8("File"))
        self.menuSetting = QtGui.QMenu(self.menuBar)
        self.menuSetting.setObjectName(_fromUtf8("menuSetting"))
        MainWindow.setMenuBar(self.menuBar)
        self.OpenFile = QtGui.QAction(MainWindow)
        self.OpenFile.setObjectName(_fromUtf8("OpenFile"))
        self.File.addAction(self.OpenFile)
        self.menuBar.addAction(self.File.menuAction())
        self.menuBar.addAction(self.menuSetting.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.Save_As.setText(_translate("MainWindow", "Save As", None))
        self.RootWord.setText(_translate("MainWindow", "Root Word", None))
        self.File.setTitle(_translate("MainWindow", "File", None))
        self.menuSetting.setTitle(_translate("MainWindow", "Setting", None))
        self.OpenFile.setText(_translate("MainWindow", "Open", None))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

