#
# Project to contain and test various code example snippets for the QCustomPlot documentation
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = doc-code-snippets
TEMPLATE = app

HEADERS  += mainwindow.h\
            ../../qcustomplot.h

SOURCES += main.cpp\
        mainwindow.cpp\
        ../../qcustomplot.cpp

FORMS    += mainwindow.ui
