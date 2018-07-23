#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T12:08:20
#
#-------------------------------------------------

QT       += core gui network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SURN_imitator
TEMPLATE = app


SOURCES += main.cpp\
    SURN_Imitator.cpp \
    base/motionpointsofspase.cpp \
    base/clockrealtime.cpp \
    base/pointsofspace.cpp \
    src/tp.c
 #   QCustomPlot/qcustomplot.cpp \


HEADERS  += SURN_Imitator.h \
    base/motionpointsofspase.h \
    base/clockrealtime.h \
    base/pointsofspace.h \
    src/tp.h
 #   QCustomPlot/qcustomplot.h \



FORMS    +=  form_mnemoshem.ui

#include($$PWD/../lib/base/base.pri)
#include($$PWD/../lib/base/params/params.pri)
#include($$PWD/../sr-cube/cube-lib/cubeparsrv/cubeparsrv.pri)


OTHER_FILES +=
