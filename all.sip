%Copying
PyQt5 binding for QCustomPlot v2.0.0

Authors: Dmitry Voronin, Giuseppe Corbelli
License: MIT

QCustomPlot author: Emanuel Eichhammer
QCustomPlot Website/Contact: http://www.qcustomplot.com
%End

%Module QCustomPlot


%Import QtCore/QtCoremod.sip
%Import QtGui/QtGuimod.sip
%Import QtWidgets/QtWidgetsmod.sip

// Include order taken from qcp.h
%Include global.sip
%Include vector2d.sip
%Include painter.sip
// #include "paintbuffer.h"
%Include layer.sip
%Include range.sip
%Include selection.sip
%Include selectionrect.sip
%Include layout.sip
%Include lineending.sip
%Include axisticker.sip
%Include axistickerdatetime.sip
%Include axistickertext.sip
%Include axistickertime.sip
//#include "axis/axistickerfixed.h"
//#include "axis/axistickerpi.h"
//#include "axis/axistickerlog.h"
%Include axis.sip
%Include scatterstyle.sip
//%Include datacontainer.sip
%Include plottable.sip
%Include item.sip
%Include core.sip
%Include plottable1d.sip
%Include colorgradient.sip
//#include "selectiondecorator-bracket.h"
%Include layoutelement-axisrect.sip
%Include layoutelement-legend.sip
%Include layoutelement-textelement.sip
//#include "layoutelements/layoutelement-colorscale.h"
%Include plottable-graph.sip
%Include plottable-curve.sip
%Include plottable-bars.sip
//#include "plottables/plottable-statisticalbox.h"
//#include "plottables/plottable-colormap.h"
//#include "plottables/plottable-financial.h"
//#include "plottables/plottable-errorbar.h"
//#include "items/item-straightline.h"
//#include "items/item-line.h"
//#include "items/item-curve.h"
//#include "items/item-rect.h"
//#include "items/item-text.h"
//#include "items/item-ellipse.h"
//#include "items/item-pixmap.h"
//#include "items/item-tracer.h"
//#include "items/item-bracket.h"
