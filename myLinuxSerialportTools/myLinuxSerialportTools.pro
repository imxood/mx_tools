#-------------------------------------------------
#
# Project created by QtCreator 2011-12-23T12:59:44
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += console

TARGET = myLinuxSerialportTools
TEMPLATE = app


SOURCES +=\
        frmmain.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp \
    servo_tool.cpp

HEADERS  += frmmain.h \
    myStaticClass.h \
    myinclude.h \
    qextserialbase.h \
    posix_qextserialport.h

FORMS    += frmmain.ui

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
