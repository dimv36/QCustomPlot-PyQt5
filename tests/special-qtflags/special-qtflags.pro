QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = special-qtflags
TEMPLATE = app

# These are the special defines that are tested here:
DEFINES += QT_NO_KEYWORDS QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += qcustomplot.h
SOURCES += qcustomplot.cpp main.cpp 
