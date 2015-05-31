#
# Project to generate various images for the QCustomPlot documentation
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = doc-image-generator
TEMPLATE = app

HEADERS  += mainwindow.h\
            ../../qcustomplot.h

SOURCES += main.cpp\
        mainwindow.cpp\
        ../../qcustomplot.cpp

FORMS    += mainwindow.ui
