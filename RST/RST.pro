#-------------------------------------------------
#
# Project created by QtCreator 2013-08-26T19:50:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RST
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/LMST/LMST.cpp \
    lib/SMST/SMST.cpp \
    lib/SMST/PointsGenerator.cpp \
    lib/ZMST/ZMST.cpp \
    sketchpad.cpp \
    addpoint.cpp

HEADERS  += mainwindow.h \
    lib/Headers/overlap.h \
    lib/Headers/data_format.h \
    lib/LMST/LMST.h \
    lib/SMST/SMST.h \
    lib/SMST/PointsGenerator.h \
    lib/ZMST/ZMST.h \
    sketchpad.h \
    addpoint.h

FORMS    += \
    mainwindow.ui
