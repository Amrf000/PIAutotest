#-------------------------------------------------
#
# Project created by QtCreator 2019-03-16T23:50:27
#
#-------------------------------------------------

QT       -= gui
QT += network
TARGET = Vncdll
TEMPLATE = lib

DEFINES += VNCDLL_LIBRARY

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
        vncdll.cpp \
    dllmain.cpp \
    client.cpp

HEADERS += \
        vncdll.h \
        vncdll_global.h \  
    client.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR=../../bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files

LIBS += -lUser32
LIBS += -lGdi32
#LIBS += -lKERNEL32

INCLUDEPATH += E:\dev\visa\PIAutotest\Detours-master\include
LIBS += -LE:\dev\visa\PIAutotest\Detours-master\lib.X64 -ldetours
