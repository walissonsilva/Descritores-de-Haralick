#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T14:01:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = descritores-de-haralick
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

SOURCES += main.cpp \
    haralick01.cpp \
    haralick02.cpp

HEADERS += \
    haralick01.h \
    haralick02.h
