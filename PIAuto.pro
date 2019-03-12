#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T15:56:52
#
#-------------------------------------------------

QT       += core gui

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
    workerthread.cpp

HEADERS += \
        mainwindow.h \
    instrumentwidget.h \
    instconfig.h \
    emb.h \
    workerthread.h

FORMS += \
        mainwindow.ui \
    instrumentwidget.ui

INCLUDEPATH += C:/Users/lwx531574/AppData/Local/Programs/Python/Python37/include#D:/Python271/include
#LIBS += D:/Python271/libs/libpython27.a
LIBS += -LC:/Users/lwx531574/AppData/Local/Programs/Python/Python37/libs -lpython3 #-LD:/Python271/libs -lpython27

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

DESTDIR=bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files

RESOURCES += \
    res.qrc
