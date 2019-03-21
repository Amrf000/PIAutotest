#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T15:56:52
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11
#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PIAuto
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    instrumentwidget.cpp \
    instconfig.cpp \
    emb.cpp \
    workerthread.cpp \
    widgetcontainer.cpp \
    injection.cpp \
    server.cpp

HEADERS += \
        mainwindow.h \
    instrumentwidget.h \
    instconfig.h \
    emb.h \
    workerthread.h \
    widgetcontainer.h \
    injection.h \
    server.h \
    Registry.hpp

FORMS += \
        mainwindow.ui \
    instrumentwidget.ui

##D:/Python271/include
#LIBS += D:/Python271/libs/libpython27.a
# #-LD:/Python271/libs -lpython27



include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

DESTDIR=../bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files

RESOURCES += \
    res.qrc

#INCLUDEPATH += D:\dev\VISA\client\libvncserver-win-master\libvncserver-win-master
#LIBS += -LD:\dev\VISA\client\libvncserver-win-master\libvncserver-win-master\build\Debug -lvncclient
#INCLUDEPATH += D:\dev\VISA\client\SDL2-master\include
#LIBS += -LD:\dev\VISA\client\SDL2-master\VisualC\Win32\Debug -lSDL2main
#LIBS += -LD:\dev\VISA\client\SDL2-master\VisualC\Win32\Debug -lSDL2
LIBS += -lWs2_32
LIBS += -lkernel32
LIBS += -luser32
LIBS += -lwinmm
LIBS += -ladvapi32
LIBS += -lOle32
LIBS += -lGdi32
LIBS += -lVersion
LIBS += -lImm32
LIBS += -lOleAut32
LIBS += -lSetupapi
LIBS += -lShell32

win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
INCLUDEPATH += D:\Users\lwx531574\AppData\Local\Programs\Python\Python37-32\include
LIBS += -LD:\Users\lwx531574\AppData\Local\Programs\Python\Python37-32\libs -lpython3

    } else {
        message("x86_64 build")
INCLUDEPATH += C:\Users\liuyawu\AppData\Local\Programs\Python\Python35\include
LIBS += -LC:\Users\liuyawu\AppData\Local\Programs\Python\Python35\libs -lpython3
    }
}

DEFINES += _ATL_NATIVE_INITIALIZATION
