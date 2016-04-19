#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T15:24:47
#
#-------------------------------------------------

QT       += core gui widgets printsupport

TARGET = qcustomplot
TEMPLATE = lib

DEFINES += QCUSTOMPLOTPYTHON_LIBRARY

SOURCES += \
    qcustomplot.cpp \
    sipqcustomplotcmodule.cpp \
    sipqcustomplotQCP.cpp \
    sipqcustomplotQCPAbstractItem.cpp \
    sipqcustomplotQCPAbstractLegendItem.cpp \
    sipqcustomplotQCPAbstractPlottable.cpp \
    sipqcustomplotQCPAntialiasedElements.cpp \
    sipqcustomplotQCPAxis.cpp \
    sipqcustomplotQCPAxisAxisTypes.cpp \
    sipqcustomplotQCPAxisRect.cpp \
    sipqcustomplotQCPAxisSelectableParts.cpp \
    sipqcustomplotQCPBarData.cpp \
    sipqcustomplotQCPBarDataMap.cpp \
    sipqcustomplotQCPBars.cpp \
    sipqcustomplotQCPBarsGroup.cpp \
    sipqcustomplotQCPColorGradient.cpp \
    sipqcustomplotQCPColorMap.cpp \
    sipqcustomplotQCPColorMapData.cpp \
    sipqcustomplotQCPColorScale.cpp \
    sipqcustomplotQCPColorScaleAxisRectPrivate.cpp \
    sipqcustomplotQCPCurve.cpp \
    sipqcustomplotQCPCurveData.cpp \
    sipqcustomplotQCPCurveDataMap.cpp \
    sipqcustomplotQCPData.cpp \
    sipqcustomplotQCPDataMap.cpp \
    sipqcustomplotQCPFinancial.cpp \
    sipqcustomplotQCPFinancialData.cpp \
    sipqcustomplotQCPFinancialDataMap.cpp \
    sipqcustomplotQCPGraph.cpp \
    sipqcustomplotQCPGrid.cpp \
    sipqcustomplotQCPInteractions.cpp \
    sipqcustomplotQCPItemAnchor.cpp \
    sipqcustomplotQCPItemBracket.cpp \
    sipqcustomplotQCPItemCurve.cpp \
    sipqcustomplotQCPItemEllipse.cpp \
    sipqcustomplotQCPItemLine.cpp \
    sipqcustomplotQCPItemPixmap.cpp \
    sipqcustomplotQCPItemPosition.cpp \
    sipqcustomplotQCPItemRect.cpp \
    sipqcustomplotQCPItemStraightLine.cpp \
    sipqcustomplotQCPItemText.cpp \
    sipqcustomplotQCPItemTracer.cpp \
    sipqcustomplotQCPLayer.cpp \
    sipqcustomplotQCPLayerable.cpp \
    sipqcustomplotQCPLayout.cpp \
    sipqcustomplotQCPLayoutElement.cpp \
    sipqcustomplotQCPLayoutGrid.cpp \
    sipqcustomplotQCPLayoutInset.cpp \
    sipqcustomplotQCPLegend.cpp \
    sipqcustomplotQCPLegendSelectableParts.cpp \
    sipqcustomplotQCPLineEnding.cpp \
    sipqcustomplotQCPMarginGroup.cpp \
    sipqcustomplotQCPMarginSides.cpp \
    sipqcustomplotQCPPainter.cpp \
    sipqcustomplotQCPPainterPainterModes.cpp \
    sipqcustomplotQCPPlottableLegendItem.cpp \
    sipqcustomplotQCPPlottingHints.cpp \
    sipqcustomplotQCPPlotTitle.cpp \
    sipqcustomplotQCPRange.cpp \
    sipqcustomplotQCPScatterStyle.cpp \
    sipqcustomplotQCPStatisticalBox.cpp \
    sipqcustomplotQCustomPlot.cpp \
    sipqcustomplotQHash0100QCPMarginSide0101QCPMarginGroup.cpp \
    sipqcustomplotQList0101QCPAbstractItem.cpp \
    sipqcustomplotQList0101QCPAbstractLegendItem.cpp \
    sipqcustomplotQList0101QCPAbstractPlottable.cpp \
    sipqcustomplotQList0101QCPAxis.cpp \
    sipqcustomplotQList0101QCPAxisRect.cpp \
    sipqcustomplotQList0101QCPBars.cpp \
    sipqcustomplotQList0101QCPColorMap.cpp \
    sipqcustomplotQList0101QCPGraph.cpp \
    sipqcustomplotQList0101QCPItemAnchor.cpp \
    sipqcustomplotQList0101QCPItemPosition.cpp \
    sipqcustomplotQList0101QCPLayerable.cpp \
    sipqcustomplotQList0101QCPLayoutElement.cpp \
    sipqcustomplotQList0101QCPLegend.cpp \
    sipqcustomplotQMap24000100QColor.cpp \
    sipqcustomplotQVector0100QCPData.cpp \
    sipqcustomplotQVector0100QString.cpp

HEADERS += \
    qcustomplot.h \
    sipAPIqcustomplot.h

unix {
    target.path = /usr/lib64/python3.4/site-packages/qcustomplot/
    INCLUDEPATH += /usr/include/python3.4m
    INSTALLS += target
}

win32 {
    LIBS += C:/Python34/libs/python34.lib
    INCLUDEPATH += "C:/Python34/include"
}

DISTFILES += \
    qcustomplot.sip
