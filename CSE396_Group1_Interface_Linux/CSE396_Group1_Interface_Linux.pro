#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T22:04:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CSE396_Group1_Interface_Linux
TEMPLATE = app



SOURCES += main.cpp \
        cse396_group1_interface.cpp \
        qcustomplot.cpp \
        irrlichtWidget.cpp \
        serialport.cpp

HEADERS  += cse396_group1_interface.h \
        qcustomplot.h \
        irrlichtWidget.h \
        serialport.h \
        irrlichtWidget.h

FORMS    += cse396_group1_interface.ui

INCLUDEPATH +=  /usr/include/irrlicht \
                /usr/X11R6/include

LIBS += -L/usr/X11R6/lib \
        -L/usr/include/irrlicht \
        -lIrrlicht \
        -lGL \
        -lX11 \
        -lQtOpenGL
