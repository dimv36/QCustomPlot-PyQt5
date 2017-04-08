#
#  Project to compile QCustomPlot as shared library (.so/.dll) from the amalgamated sources
#

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY
TEMPLATE = lib
CONFIG += static
VERSION = 1.3.1

TARGET = qcustomplot

QMAKE_TARGET_PRODUCT = "QCustomPlot"
QMAKE_TARGET_DESCRIPTION = "Plotting library for Qt"

QMAKE_TARGET_COMPANY = "www.qcustomplot.com"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) by Emanuel Eichhammer"

SOURCES += qcustomplot.cpp
HEADERS += qcustomplot.h
