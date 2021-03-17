# Based on the Qt Test tutorial.
# https://doc.qt.io/qt-5/qttestlib-tutorial4-example.html
QT += testlib widgets
QT -= gui

#CONFIG += qt console warn_on depend_includepath testcase
#CONFIG -= app_bundle

TEMPLATE = app

#INCDIR1 = "/home/student/COMP3004-DP-Testing/COMP3004-DenasProject/app"
INCDIR1 = ".."
#INCDIR2 = "../../build-COMP3004-Project-Desktop_Qt_5_13_0_GCC_64bit-Debug"
INCLUDEPATH += . $$INCDIR1 #$$INCDIR2

SOURCES +=  device.cpp \
            tst_device1.cpp

HEADERS +=  device.h

FORMS += device.ui
