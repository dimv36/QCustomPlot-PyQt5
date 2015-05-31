#
# Project to test various use cases
#

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = test
TEMPLATE = app

CONFIG(debug, debug|release) {
  qcplib.commands = cd ../../src && $(MAKE) debug
} else {
  qcplib.commands = cd ../../src && $(MAKE) release
}
QMAKE_EXTRA_TARGETS += qcplib
PRE_TARGETDEPS += qcplib

SOURCES += main.cpp\
mainwindow.cpp
HEADERS  += mainwindow.h \
../../src/*.h
FORMS    += mainwindow.ui
OTHER_FILES += \
../../changelog.txt

LIBS += -L../../src
CONFIG(debug, debug|release) {
  LIBS += -lqcustomplotd
} else {
  LIBS += -lqcustomplot
}



