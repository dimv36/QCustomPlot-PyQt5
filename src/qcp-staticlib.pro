#
# Project to compile QCustomPlot as static library (.a/.lib) from the non-amalgamated sources
#
# It creates two static library files: qcustomplot and qcustomplotd, for release and debug mode respectively.
# If you want to be able to step into QCustomPlot code in your debug-mode-application, link with qcustomplotd.
# If compiling your application in release mode, you should link with qcustomplot.
#
# In your project's HEADERS, you may use either the amalgamated header qcustomplot.h or all separated headers.
# (note that qmake understands "*.h" if you choose the latter option.)
#

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TEMPLATE = lib
CONFIG += qt staticlib debug_and_release build_all

VERSION = 1.3.1
TARGET = qcustomplot
CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d) # if compiling in debug mode, append a "d" to the library name
  MOC_DIR = build-debug
  OBJECTS_DIR = build-debug
} else {
  MOC_DIR = build-release
  OBJECTS_DIR = build-release
}

HEADERS += \
global.h \
painter.h \
layer.h \
range.h \
axis.h \
plottable.h \
item.h \
lineending.h \
core.h \
layout.h \
plottables/plottable-graph.h \
plottables/plottable-curve.h \
plottables/plottable-bars.h \
plottables/plottable-statisticalbox.h \
plottables/plottable-colormap.h \
plottables/plottable-financial.h \
items/item-straightline.h \
items/item-line.h \
items/item-curve.h \
items/item-rect.h \
items/item-text.h \
items/item-ellipse.h \
items/item-pixmap.h \
items/item-tracer.h \
items/item-bracket.h \
layoutelements/layoutelement-axisrect.h \
    layoutelements/layoutelement-legend.h \
    layoutelements/layoutelement-plottitle.h \
    layoutelements/layoutelement-colorscale.h \
    colorgradient.h

SOURCES += \
painter.cpp \
layer.cpp \
range.cpp \
axis.cpp \
plottable.cpp \
item.cpp \
lineending.cpp \
core.cpp \
layout.cpp \
plottables/plottable-graph.cpp \
plottables/plottable-curve.cpp \
plottables/plottable-bars.cpp \
plottables/plottable-statisticalbox.cpp \
plottables/plottable-colormap.cpp \
plottables/plottable-financial.cpp \
items/item-straightline.cpp \
items/item-line.cpp \
items/item-curve.cpp \
items/item-rect.cpp \
items/item-text.cpp \
items/item-ellipse.cpp \
items/item-pixmap.cpp \
items/item-tracer.cpp \
items/item-bracket.cpp \
layoutelements/layoutelement-axisrect.cpp \
    layoutelements/layoutelement-legend.cpp \
    layoutelements/layoutelement-plottitle.cpp \
    layoutelements/layoutelement-colorscale.cpp \
    colorgradient.cpp

OTHER_FILES += \
    ../changelog.txt \
    doc-classoverview.dox \
    doc-layoutsystem.dox \
    doc-mainpage.dox \
    doc-specialqtflags.dox \
    doc-performance.dox
