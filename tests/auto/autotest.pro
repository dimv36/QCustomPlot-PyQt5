
QT += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TEMPLATE = app
DEPENDPATH = .
INCLUDEPATH = .

HEADERS += ../../qcustomplot.h \
    test-qcustomplot/test-qcustomplot.h\
    test-qcpgraph/test-qcpgraph.h \
    test-qcplayout/test-qcplayout.h \
    test-qcpaxisrect/test-qcpaxisrect.h \
    test-colormap/test-colormap.h

SOURCES += ../../qcustomplot.cpp \
           autotest.cpp \
    test-qcustomplot/test-qcustomplot.cpp\
    test-qcpgraph/test-qcpgraph.cpp \
    test-qcplayout/test-qcplayout.cpp \
    test-qcpaxisrect/test-qcpaxisrect.cpp \
    test-colormap/test-colormap.cpp
    
