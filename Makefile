TARGET = qcustomplot.so
OFILES = sipqcustomplotcmodule.o sipqcustomplotQVector0100QCPData.o sipqcustomplotQList0101QCPLegend.o sipqcustomplotQList0101QCPAxis.o sipqcustomplotQList0101QCPAbstractItem.o sipqcustomplotQList0101QCPGraph.o sipqcustomplotQList0101QCPAbstractPlottable.o sipqcustomplotQList0101QCPItemAnchor.o sipqcustomplotQList0101QCPItemPosition.o sipqcustomplotQList0101QCPAbstractLegendItem.o sipqcustomplotQVector0100QString.o sipqcustomplotQList0101QCPLayerable.o sipqcustomplotQCPItemBracket.o sipqcustomplotQCPItemTracer.o sipqcustomplotQCPItemPixmap.o sipqcustomplotQCPItemEllipse.o sipqcustomplotQCPItemText.o sipqcustomplotQCPItemRect.o sipqcustomplotQCPItemCurve.o sipqcustomplotQCPItemLine.o sipqcustomplotQCPItemStraightLine.o sipqcustomplotQCPStatisticalBox.o sipqcustomplotQCPBars.o sipqcustomplotQCPBarData.o sipqcustomplotQCPCurve.o sipqcustomplotQCPCurveData.o sipqcustomplotQCPGraph.o sipqcustomplotQCPData.o sipqcustomplotQCustomPlotInteractions.o sipqcustomplotQCustomPlot.o sipqcustomplotQCPLineEnding.o sipqcustomplotQCPAbstractItem.o sipqcustomplotQCPItemPosition.o sipqcustomplotQCPItemAnchor.o sipqcustomplotQCPAbstractPlottable.o sipqcustomplotQCPLegendSelectableParts.o sipqcustomplotQCPLegend.o sipqcustomplotQCPPlottableLegendItem.o sipqcustomplotQCPAbstractLegendItem.o sipqcustomplotQCPAxisSelectableParts.o sipqcustomplotQCPAxis.o sipqcustomplotQCPGrid.o sipqcustomplotQCPRange.o sipqcustomplotQCPLayerable.o sipqcustomplotQCPLayer.o sipqcustomplotQCPPainter.o sipqcustomplotQCPPlottingHints.o sipqcustomplotQCPAntialiasedElements.o sipqcustomplotQCP.o
HFILES = sipAPIqcustomplot.h 

CC = gcc
CXX = g++
LINK = g++
CPPFLAGS = -DNDEBUG -I. -I/usr/include/qt5/ -I/usr/include/python3.4m
CFLAGS = -O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches  -m64 -mtune=generic -fPIC -O2 -Wall -W
CXXFLAGS = -O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches  -m64 -mtune=generic -fPIC -O2 -Wall -W
LFLAGS = -shared -Wl,--version-script=qcustomplot.exp
LIBS = -L. -lqcustomplot
.SUFFIXES: .c .o .cpp .cc .cxx .C


.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: $(TARGET)

$(OFILES): $(HFILES)

$(TARGET): $(OFILES)
	@echo '{ global: PyInit_qcustomplot; local: *; };' > qcustomplot.exp
	$(LINK) $(LFLAGS) -o $(TARGET) $(OFILES) $(LIBS)

install: $(TARGET)
	@test -d $(DESTDIR)/usr/lib64/python3.4/site-packages || mkdir -p $(DESTDIR)/usr/lib64/python3.4/site-packages
	cp -f $(TARGET) $(DESTDIR)/usr/lib64/python3.4/site-packages/$(TARGET)
	@test -d $(DESTDIR)/usr/share/python3-sip/qcustomplot || mkdir -p $(DESTDIR)/usr/share/python3-sip/qcustomplot
	cp -f qcustomplot.sip $(DESTDIR)/usr/share/python3-sip/qcustomplot/qcustomplot.sip

clean:
	-rm -f $(TARGET)
	-rm -f sipqcustomplotcmodule.o
	-rm -f sipqcustomplotQVector0100QCPData.o
	-rm -f sipqcustomplotQList0101QCPLegend.o
	-rm -f sipqcustomplotQList0101QCPAxis.o
	-rm -f sipqcustomplotQList0101QCPAbstractItem.o
	-rm -f sipqcustomplotQList0101QCPGraph.o
	-rm -f sipqcustomplotQList0101QCPAbstractPlottable.o
	-rm -f sipqcustomplotQList0101QCPItemAnchor.o
	-rm -f sipqcustomplotQList0101QCPItemPosition.o
	-rm -f sipqcustomplotQList0101QCPAbstractLegendItem.o
	-rm -f sipqcustomplotQVector0100QString.o
	-rm -f sipqcustomplotQList0101QCPLayerable.o
	-rm -f sipqcustomplotQCPItemBracket.o
	-rm -f sipqcustomplotQCPItemTracer.o
	-rm -f sipqcustomplotQCPItemPixmap.o
	-rm -f sipqcustomplotQCPItemEllipse.o
	-rm -f sipqcustomplotQCPItemText.o
	-rm -f sipqcustomplotQCPItemRect.o
	-rm -f sipqcustomplotQCPItemCurve.o
	-rm -f sipqcustomplotQCPItemLine.o
	-rm -f sipqcustomplotQCPItemStraightLine.o
	-rm -f sipqcustomplotQCPStatisticalBox.o
	-rm -f sipqcustomplotQCPBars.o
	-rm -f sipqcustomplotQCPBarData.o
	-rm -f sipqcustomplotQCPCurve.o
	-rm -f sipqcustomplotQCPCurveData.o
	-rm -f sipqcustomplotQCPGraph.o
	-rm -f sipqcustomplotQCPData.o
	-rm -f sipqcustomplotQCustomPlotInteractions.o
	-rm -f sipqcustomplotQCustomPlot.o
	-rm -f sipqcustomplotQCPLineEnding.o
	-rm -f sipqcustomplotQCPAbstractItem.o
	-rm -f sipqcustomplotQCPItemPosition.o
	-rm -f sipqcustomplotQCPItemAnchor.o
	-rm -f sipqcustomplotQCPAbstractPlottable.o
	-rm -f sipqcustomplotQCPLegendSelectableParts.o
	-rm -f sipqcustomplotQCPLegend.o
	-rm -f sipqcustomplotQCPPlottableLegendItem.o
	-rm -f sipqcustomplotQCPAbstractLegendItem.o
	-rm -f sipqcustomplotQCPAxisSelectableParts.o
	-rm -f sipqcustomplotQCPAxis.o
	-rm -f sipqcustomplotQCPGrid.o
	-rm -f sipqcustomplotQCPRange.o
	-rm -f sipqcustomplotQCPLayerable.o
	-rm -f sipqcustomplotQCPLayer.o
	-rm -f sipqcustomplotQCPPainter.o
	-rm -f sipqcustomplotQCPPlottingHints.o
	-rm -f sipqcustomplotQCPAntialiasedElements.o
	-rm -f sipqcustomplotQCP.o
	-rm -f qcustomplot.exp
