/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1-mod                                            **
**          Modified: Dmitry Voronin                                      **
**          date: 18.07.15                                                **
****************************************************************************/

%Module qcustomplot
%Import QtGui/QtGuimod.sip
%Import QtWidgets/QtWidgetsmod.sip

namespace QCP
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

enum MarginSide { msLeft     = 0x01 ///< <tt>0x01</tt> left margin
                  ,msRight   = 0x02 ///< <tt>0x02</tt> right margin
                  ,msTop     = 0x04 ///< <tt>0x04</tt> top margin
                  ,msBottom  = 0x08 ///< <tt>0x08</tt> bottom margin
                  ,msAll     = 0xFF ///< <tt>0xFF</tt> all margins
                  ,msNone    = 0x00 ///< <tt>0x00</tt> no margin
                };
typedef QFlags<QCP::MarginSide> MarginSides;

enum AntialiasedElement { aeAxes           = 0x0001 ///< <tt>0x0001</tt> Axis base line and tick marks
                          ,aeGrid          = 0x0002 ///< <tt>0x0002</tt> Grid lines
                          ,aeSubGrid       = 0x0004 ///< <tt>0x0004</tt> Sub grid lines
                          ,aeLegend        = 0x0008 ///< <tt>0x0008</tt> Legend box
                          ,aeLegendItems   = 0x0010 ///< <tt>0x0010</tt> Legend items
                          ,aePlottables    = 0x0020 ///< <tt>0x0020</tt> Main lines of plottables (excluding error bars, see element \ref aeErrorBars)
                          ,aeItems         = 0x0040 ///< <tt>0x0040</tt> Main lines of items
                          ,aeScatters      = 0x0080 ///< <tt>0x0080</tt> Scatter symbols of plottables (excluding scatter symbols of type ssPixmap)
                          ,aeErrorBars     = 0x0100 ///< <tt>0x0100</tt> Error bars
                          ,aeFills         = 0x0200 ///< <tt>0x0200</tt> Borders of fills (e.g. under or between graphs)
                          ,aeZeroLine      = 0x0400 ///< <tt>0x0400</tt> Zero-lines, see \ref QCPAxis::setZeroLinePen
                          ,aeAll           = 0xFFFF ///< <tt>0xFFFF</tt> All elements
                          ,aeNone          = 0x0000 ///< <tt>0x0000</tt> No elements
                        };
typedef QFlags<QCP::AntialiasedElement> AntialiasedElements;

enum PlottingHint { phNone            = 0x000 ///< <tt>0x000</tt> No hints are set
                    ,phFastPolylines  = 0x001 ///< <tt>0x001</tt> Graph/Curve lines are drawn with a faster method. This reduces the quality
                                              ///<                especially of the line segment joins. (Only relevant for solid line pens.)
                    ,phForceRepaint   = 0x002 ///< <tt>0x002</tt> causes an immediate repaint() instead of a soft update() when QCustomPlot::replot() is called with parameter \ref QCustomPlot::rpHint.
                                              ///<                This is set by default to prevent the plot from freezing on fast consecutive replots (e.g. user drags ranges with mouse).
                    ,phCacheLabels    = 0x004 ///< <tt>0x004</tt> axis (tick) labels will be cached as pixmaps, increasing replot performance.
                  };
typedef QFlags<QCP::PlottingHint> PlottingHints;

enum Interaction { iRangeDrag         = 0x001 ///< <tt>0x001</tt> Axis ranges are draggable (see \ref QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeDragAxes)
                   ,iRangeZoom        = 0x002 ///< <tt>0x002</tt> Axis ranges are zoomable with the mouse wheel (see \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeZoomAxes)
                   ,iMultiSelect      = 0x004 ///< <tt>0x004</tt> The user can select multiple objects by holding the modifier set by \ref QCustomPlot::setMultiSelectModifier while clicking
                   ,iSelectPlottables = 0x008 ///< <tt>0x008</tt> Plottables are selectable (e.g. graphs, curves, bars,... see QCPAbstractPlottable)
                   ,iSelectAxes       = 0x010 ///< <tt>0x010</tt> Axes are selectable (or parts of them, see QCPAxis::setSelectableParts)
                   ,iSelectLegend     = 0x020 ///< <tt>0x020</tt> Legends are selectable (or their child items, see QCPLegend::setSelectableParts)
                   ,iSelectItems      = 0x040 ///< <tt>0x040</tt> Items are selectable (Rectangles, Arrows, Textitems, etc. see \ref QCPAbstractItem)
                   ,iSelectOther      = 0x080 ///< <tt>0x080</tt> All other objects are selectable (e.g. your own derived layerables, the plot title,...)
                 };
typedef QFlags<QCP::Interaction> Interactions;

bool isInvalidData(double value);
bool isInvalidData(double value1, double value2);
void setMarginValue(QMargins &margins, QCP::MarginSide side, int value);
int getMarginValue(const QMargins &margins, QCP::MarginSide side);
};


class QCPScatterStyle
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum ScatterShape { ssNone       ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                      ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                      ,ssCross     ///< \enumimage{ssCross.png} a cross
                      ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                      ,ssCircle    ///< \enumimage{ssCircle.png} a circle
                      ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle)
                      ,ssSquare    ///< \enumimage{ssSquare.png} a square
                      ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond
                      ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
                      ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline
                      ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
                      ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square with a cross inside
                      ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square with a plus inside
                      ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle with a cross inside
                      ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle with a plus inside
                      ,ssPeace     ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines
                      ,ssPixmap    ///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates
                      ,ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
                    };

  QCPScatterStyle();
  QCPScatterStyle(ScatterShape shape, double size=6);
  QCPScatterStyle(ScatterShape shape, const QColor &color, double size);
  QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);
  QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);
  QCPScatterStyle(const QPixmap &pixmap);
  QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush=Qt::NoBrush, double size=6);

  // getters:
  double size() const;
  ScatterShape shape() const;
  QPen pen() const;
  QBrush brush() const;
  QPixmap pixmap() const;
  QPainterPath customPath() const;

  // setters:
  void setSize(double size);
  void setShape(ScatterShape shape);
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setPixmap(const QPixmap &pixmap);
  void setCustomPath(const QPainterPath &customPath);

  // non-property methods:
  bool isNone() const;
  bool isPenDefined();
  void applyTo(QCPPainter *painter, const QPen &defaultPen) const;
  void drawShape(QCPPainter *painter, QPointF pos) const;
  void drawShape(QCPPainter *painter, double x, double y) const;
};


class QCPPainter : public QPainter
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum PainterMode { pmDefault       = 0x00   ///< <tt>0x00</tt> Default mode for painting on screen devices
                     ,pmVectorized   = 0x01   ///< <tt>0x01</tt> Mode for vectorized painting (e.g. PDF export). For example, this prevents some antialiasing fixes.
                     ,pmNoCaching    = 0x02   ///< <tt>0x02</tt> Mode for all sorts of exports (e.g. PNG, PDF,...). For example, this prevents using cached pixmap labels
                     ,pmNonCosmetic  = 0x04   ///< <tt>0x04</tt> Turns pen widths 0 to 1, i.e. disables cosmetic pens. (A cosmetic pen is always drawn with width 1 pixel in the vector image/pdf viewer, independent of zoom.)
                   };
  typedef QFlags<QCPPainter::PainterMode> PainterModes;

  QCPPainter();
  QCPPainter(QPaintDevice *device);
  ~QCPPainter();

  // getters:
  bool antialiasing() const;
  PainterModes modes() const;

  // setters:
  void setAntialiasing(bool enabled);
  void setMode(PainterMode mode, bool enabled=true);
  void setModes(PainterModes modes);

  // methods hiding non-virtual base class functions (QPainter bug workarounds):
  bool begin(QPaintDevice *device);
  void setPen(const QPen &pen);
  void setPen(const QColor &color);
  void setPen(Qt::PenStyle penStyle);
  void drawLine(const QLineF &line);
  void drawLine(const QPointF &p1, const QPointF &p2);
  void save();
  void restore();

  // non-virtual methods:
  void makeNonCosmetic();
};


class QCPLayer
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPLayer(QCustomPlot* parentPlot, const QString &layerName);
  ~QCPLayer();

  // getters:
  QCustomPlot *parentPlot() const;
  QString name() const;
  int index() const;
  QList<QCPLayerable*> children() const;
  bool visible() const;

  // setters:
  void setVisible(bool visible);

protected:
  // non-virtual methods:
  void addChild(QCPLayerable *layerable, bool prepend);
  void removeChild(QCPLayerable *layerable);

private:
  QCPLayer(const QCPLayer &);
};

class  QCPLayerable : public QObject
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPLayerable(QCustomPlot *plot, QString targetLayer=QString(), QCPLayerable *parentLayerable=0);
  ~QCPLayerable();

  // getters:
  bool visible() const;
  QCustomPlot *parentPlot() const;
  QCPLayerable *parentLayerable() const;
  QCPLayer *layer() const;
  bool antialiased() const;

  // setters:
  void setVisible(bool on);
  Q_SLOT bool setLayer(QCPLayer *layer);
  bool setLayer(const QString &layerName);
  void setAntialiased(bool enabled);

  // introduced virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-property methods:
  bool realVisibility() const;

signals:
  void layerChanged(QCPLayer *newLayer);

protected:
  // introduced virtual methods:
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  virtual QCP::Interaction selectionCategory() const;
  virtual QRect clipRect() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const = 0;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // non-property methods:
  void initializeParentPlot(QCustomPlot *parentPlot);
  void setParentLayerable(QCPLayerable* parentLayerable);
  bool moveToLayer(QCPLayer *layer, bool prepend);
  void applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;
};


class  QCPRange
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  double lower;
  double upper;

  QCPRange();
  QCPRange(double lower, double upper);

  double size() const;
  double center() const;
  void normalize();
  void expand(const QCPRange &otherRange);
  QCPRange expanded(const QCPRange &otherRange) const;
  QCPRange sanitizedForLogScale() const;
  QCPRange sanitizedForLinScale() const;
  bool contains(double value) const;

  static bool validRange(double lower, double upper);
  static bool validRange(const QCPRange &range);
  static const double minRange; //1e-280;
  static const double maxRange; //1e280;
};


class QCPMarginGroup : public QObject
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPMarginGroup(QCustomPlot *parentPlot);
  ~QCPMarginGroup();

  // non-virtual methods:
  QList<QCPLayoutElement*> elements(QCP::MarginSide side) const;
  bool isEmpty() const;
  void clear();

protected:
  // non-virtual methods:
  int commonMargin(QCP::MarginSide side) const;
  void addChild(QCP::MarginSide side, QCPLayoutElement *element);
  void removeChild(QCP::MarginSide side, QCPLayoutElement *element);
};


class QCPLayoutElement : public QCPLayerable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum UpdatePhase { upPreparation ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
                     ,upMargins    ///< Phase in which the margins are calculated and set
                     ,upLayout     ///< Final phase in which the layout system places the rects of the elements
                   };

  explicit QCPLayoutElement(QCustomPlot *parentPlot=0);
  virtual ~QCPLayoutElement();

  // getters:
  QCPLayout *layout() const;
  QRect rect() const;
  QRect outerRect() const;
  QMargins margins() const;
  QMargins minimumMargins() const;
  QCP::MarginSides autoMargins() const;

  QSize minimumSize() const;
  QSize maximumSize() const;
  QCPMarginGroup *marginGroup(QCP::MarginSide side) const;
  QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const;

  // setters:
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setMinimumMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMinimumSize(int width, int height);
  void setMaximumSize(const QSize &size);
  void setMaximumSize(int width, int height);
  void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);

  // introduced virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

protected:
  // introduced virtual methods:
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
};


class QCPLayout : public QCPLayoutElement
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPLayout();

  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;

  // introduced virtual methods:
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual void simplify();

  // non-virtual methods:
  bool removeAt(int index);
  bool remove(QCPLayoutElement* element);
  void clear();

protected:
  // introduced virtual methods:
  virtual void updateLayout();

  // non-virtual methods:
  void sizeConstraintsChanged() const;
  void adoptElement(QCPLayoutElement *el);
  void releaseElement(QCPLayoutElement *el);
  QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const;
};


class QCPLayoutGrid : public QCPLayout
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPLayoutGrid();
  virtual ~QCPLayoutGrid();

  // getters:
  int rowCount() const;
  int columnCount() const;
  QList<double> columnStretchFactors() const;
  QList<double> rowStretchFactors() const;
  int columnSpacing() const;
  int rowSpacing() const;

  // setters:
  void setColumnStretchFactor(int column, double factor);
  void setColumnStretchFactors(const QList<double> &factors);
  void setRowStretchFactor(int row, double factor);
  void setRowStretchFactors(const QList<double> &factors);
  void setColumnSpacing(int pixels);
  void setRowSpacing(int pixels);

  // reimplemented virtual methods:
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  virtual void simplify();
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;

  // non-virtual methods:
  QCPLayoutElement *element(int row, int column) const;
  bool addElement(int row, int column, QCPLayoutElement *element);
  bool hasElement(int row, int column);
  void expandTo(int newRowCount, int newColumnCount);
  void insertRow(int newIndex);
  void insertColumn(int newIndex);

protected:
  // non-virtual methods:
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
};


class QCPLayoutInset : public QCPLayout
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum InsetPlacement { ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
                        ,ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
                      };

  explicit QCPLayoutInset();
  virtual ~QCPLayoutInset();

  // getters:
  InsetPlacement insetPlacement(int index) const;
  Qt::Alignment insetAlignment(int index) const;
  QRectF insetRect(int index) const;

  // setters:
  void setInsetPlacement(int index, InsetPlacement placement);
  void setInsetAlignment(int index, Qt::Alignment alignment);
  void setInsetRect(int index, const QRectF &rect);

  // reimplemented virtual methods:
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual void simplify();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-virtual methods:
  void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
  void addElement(QCPLayoutElement *element, const QRectF &rect);
};


class QCPLineEnding
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum EndingStyle { esNone          ///< No ending decoration
                     ,esFlatArrow    ///< A filled arrow head with a straight/flat back (a triangle)
                     ,esSpikeArrow   ///< A filled arrow head with an indented back
                     ,esLineArrow    ///< A non-filled arrow head with open back
                     ,esDisc         ///< A filled circle
                     ,esSquare       ///< A filled square
                     ,esDiamond      ///< A filled diamond (45° rotated square)
                     ,esBar          ///< A bar perpendicular to the line
                     ,esHalfBar      ///< A bar perpendicular to the line, pointing out to only one side (to which side can be changed with \ref setInverted)
                     ,esSkewedBar    ///< A bar that is skewed (skew controllable via \ref setLength)
                   };

  QCPLineEnding();
  QCPLineEnding(EndingStyle style, double width=8, double length=10, bool inverted=false);

  // getters:
  EndingStyle style() const;
  double width() const;
  double length() const;
  bool inverted() const;

  // setters:
  void setStyle(EndingStyle style);
  void setWidth(double width);
  void setLength(double length);
  void setInverted(bool inverted);

  // non-property methods:
  double boundingDistance() const;
  double realLength() const;
  void draw(QCPPainter *painter, const QVector2D &pos, const QVector2D &dir) const;
  void draw(QCPPainter *painter, const QVector2D &pos, double angle) const;
};


class QCPGrid : public QCPLayerable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPGrid(QCPAxis *parentAxis);

  // getters:
  bool subGridVisible() const;
  bool antialiasedSubGrid() const;
  bool antialiasedZeroLine() const;
  QPen pen() const;
  QPen subGridPen() const;
  QPen zeroLinePen() const;

  // setters:
  void setSubGridVisible(bool visible);
  void setAntialiasedSubGrid(bool enabled);
  void setAntialiasedZeroLine(bool enabled);
  void setPen(const QPen &pen);
  void setSubGridPen(const QPen &pen);
  void setZeroLinePen(const QPen &pen);

protected:
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  void drawGridLines(QCPPainter *painter) const;
  void drawSubGridLines(QCPPainter *painter) const;
};


class QCPAxis : public QCPLayerable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum AxisType { atLeft    = 0x01  ///< <tt>0x01</tt> Axis is vertical and on the left side of the axis rect
                  ,atRight  = 0x02  ///< <tt>0x02</tt> Axis is vertical and on the right side of the axis rect
                  ,atTop    = 0x04  ///< <tt>0x04</tt> Axis is horizontal and on the top side of the axis rect
                  ,atBottom = 0x08  ///< <tt>0x08</tt> Axis is horizontal and on the bottom side of the axis rect
                };
  typedef QFlags<QCPAxis::AxisType> AxisTypes;
  enum LabelType { ltNumber    ///< Tick coordinate is regarded as normal number and will be displayed as such. (see \ref setNumberFormat)
                   ,ltDateTime ///< Tick coordinate is regarded as a date/time (seconds since 1970-01-01T00:00:00 UTC) and will be displayed and formatted as such. (for details, see \ref setDateTimeFormat)
                 };
  enum LabelSide { lsInside    ///< Tick labels will be displayed inside the axis rect and clipped to the inner axis rect
                   ,lsOutside  ///< Tick labels will be displayed outside the axis rect
                 };
  enum ScaleType { stLinear       ///< Linear scaling
                   ,stLogarithmic ///< Logarithmic scaling with correspondingly transformed plots and (major) tick marks at every base power (see \ref setScaleLogBase).
                 };
  enum SelectablePart { spNone        = 0      ///< None of the selectable parts
                        ,spAxis       = 0x001  ///< The axis backbone and tick marks
                        ,spTickLabels = 0x002  ///< Tick labels (numbers) of this axis (as a whole, not individually)
                        ,spAxisLabel  = 0x004  ///< The axis label
                      };
//  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  typedef QFlags<QCPAxis::SelectablePart> SelectableParts;

  explicit QCPAxis(QCPAxisRect *parent, AxisType type);
  virtual ~QCPAxis();

  // getters:
  AxisType axisType() const;
  QCPAxisRect *axisRect() const;
  ScaleType scaleType() const;
  double scaleLogBase() const;
  const QCPRange range() const;
  bool rangeReversed() const;
  bool autoTicks() const;
  int autoTickCount() const;
  bool autoTickLabels() const;
  bool autoTickStep() const;
  bool autoSubTicks() const;
  bool ticks() const;
  bool tickLabels() const;
  int tickLabelPadding() const;
  LabelType tickLabelType() const;
  QFont tickLabelFont() const;
  QColor tickLabelColor() const;
  double tickLabelRotation() const;
  LabelSide tickLabelSide() const;
  QString dateTimeFormat() const;
  Qt::TimeSpec dateTimeSpec() const;
  QString numberFormat() const;
  int numberPrecision() const;
  double tickStep() const;
  QVector<double> tickVector() const;
  QVector<QString> tickVectorLabels() const;
  int tickLengthIn() const;
  int tickLengthOut() const;
  int subTickCount() const;
  int subTickLengthIn() const;
  int subTickLengthOut() const;
  QPen basePen() const;
  QPen tickPen() const;
  QPen subTickPen() const;
  QFont labelFont() const;
  QColor labelColor() const;
  QString label() const;
  int labelPadding() const;
  int padding() const;
  int offset() const;
  SelectableParts selectedParts() const;
  SelectableParts selectableParts() const;
  QFont selectedTickLabelFont() const;
  QFont selectedLabelFont() const;
  QColor selectedTickLabelColor() const;
  QColor selectedLabelColor() const;
  QPen selectedBasePen() const;
  QPen selectedTickPen() const;
  QPen selectedSubTickPen() const;
  QCPLineEnding lowerEnding() const;
  QCPLineEnding upperEnding() const;
  QCPGrid *grid() const;

  // setters:
  Q_SLOT void setScaleType(QCPAxis::ScaleType type);
  void setScaleLogBase(double base);
  Q_SLOT void setRange(const QCPRange &range);
  void setRange(double lower, double upper);
  void setRange(double position, double size, Qt::AlignmentFlag alignment);
  void setRangeLower(double lower);
  void setRangeUpper(double upper);
  void setRangeReversed(bool reversed);
  void setAutoTicks(bool on);
  void setAutoTickCount(int approximateCount);
  void setAutoTickLabels(bool on);
  void setAutoTickStep(bool on);
  void setAutoSubTicks(bool on);
  void setTicks(bool show);
  void setTickLabels(bool show);
  void setTickLabelPadding(int padding);
  void setTickLabelType(LabelType type);
  void setTickLabelFont(const QFont &font);
  void setTickLabelColor(const QColor &color);
  void setTickLabelRotation(double degrees);
  void setTickLabelSide(LabelSide side);
  void setDateTimeFormat(const QString &format);
  void setDateTimeSpec(const Qt::TimeSpec &timeSpec);
  void setNumberFormat(const QString &formatCode);
  void setNumberPrecision(int precision);
  void setTickStep(double step);
  void setTickVector(const QVector<double> &vec);
  void setTickVectorLabels(const QVector<QString> &vec);
  void setTickLength(int inside, int outside=0);
  void setTickLengthIn(int inside);
  void setTickLengthOut(int outside);
  void setSubTickCount(int count);
  void setSubTickLength(int inside, int outside=0);
  void setSubTickLengthIn(int inside);
  void setSubTickLengthOut(int outside);
  void setBasePen(const QPen &pen);
  void setTickPen(const QPen &pen);
  void setSubTickPen(const QPen &pen);
  void setLabelFont(const QFont &font);
  void setLabelColor(const QColor &color);
  void setLabel(const QString &str);
  void setLabelPadding(int padding);
  void setPadding(int padding);
  void setOffset(int offset);
  void setSelectedTickLabelFont(const QFont &font);
  void setSelectedLabelFont(const QFont &font);
  void setSelectedTickLabelColor(const QColor &color);
  void setSelectedLabelColor(const QColor &color);
  void setSelectedBasePen(const QPen &pen);
  void setSelectedTickPen(const QPen &pen);
  void setSelectedSubTickPen(const QPen &pen);
  Q_SLOT void setSelectableParts(const QCPAxis::SelectableParts &selectableParts);
  Q_SLOT void setSelectedParts(const QCPAxis::SelectableParts &selectedParts);
  void setLowerEnding(const QCPLineEnding &ending);
  void setUpperEnding(const QCPLineEnding &ending);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-property methods:
  Qt::Orientation orientation() const;
  void moveRange(double diff);
  void scaleRange(double factor, double center);
  void setScaleRatio(const QCPAxis *otherAxis, double ratio=1.0);
  void rescale(bool onlyVisiblePlottables=false);
  double pixelToCoord(double value) const;
  double coordToPixel(double value) const;
  SelectablePart getPartAt(const QPointF &pos) const;
  QList<QCPAbstractPlottable*> plottables() const;
  QList<QCPGraph*> graphs() const;
  QList<QCPAbstractItem*> items() const;

  static AxisType marginSideToAxisType(QCP::MarginSide side);
  static Qt::Orientation orientation(AxisType type);
  static AxisType opposite(AxisType type);

signals:
  void ticksRequest();
  void rangeChanged(const QCPRange &newRange);
  void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
  void scaleTypeChanged(QCPAxis::ScaleType scaleType);
  void selectionChanged(const QCPAxis::SelectableParts &parts);
  void selectableChanged(const QCPAxis::SelectableParts &parts);

protected:
  // introduced virtual methods:
  virtual void setupTickVectors();
  virtual void generateAutoTicks();
  virtual int calculateAutoSubTickCount(double tickStep) const;
  virtual int calculateMargin();

  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual QCP::Interaction selectionCategory() const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // non-virtual methods:
  void visibleTickBounds(int &lowIndex, int &highIndex) const;
  double baseLog(double value) const;
  double basePow(double value) const;
  QPen getBasePen() const;
  QPen getTickPen() const;
  QPen getSubTickPen() const;
  QFont getTickLabelFont() const;
  QFont getLabelFont() const;
  QColor getTickLabelColor() const;
  QColor getLabelColor() const;
};


class QCPAbstractPlottable : public QCPLayerable /Abstract/
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis);

  // getters:
  QString name() const;
  bool antialiasedFill() const;
  bool antialiasedScatters() const;
  bool antialiasedErrorBars() const;
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setName(const QString &name);
  void setAntialiasedFill(bool enabled);
  void setAntialiasedScatters(bool enabled);
  void setAntialiasedErrorBars(bool enabled);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setKeyAxis(QCPAxis *axis);
  void setValueAxis(QCPAxis *axis);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // introduced virtual methods:
  virtual void clearData() = 0;
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  virtual bool addToLegend();
  virtual bool removeFromLegend() const;

  // non-property methods:
  void rescaleAxes(bool onlyEnlarge=false) const;
  void rescaleKeyAxis(bool onlyEnlarge=false) const;
  void rescaleValueAxis(bool onlyEnlarge=false) const;

public:
  /*!
    Represents negative and positive sign domain for passing to \ref getKeyRange and \ref getValueRange.
  */
  enum SignDomain { sdNegative  ///< The negative sign domain, i.e. numbers smaller than zero
                   ,sdBoth     ///< Both sign domains, including zero, i.e. all (rational) numbers
                   ,sdPositive ///< The positive sign domain, i.e. numbers greater than zero
                 };

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);

protected:
  // reimplemented virtual methods:
  virtual QRect clipRect() const;
  virtual void draw(QCPPainter *painter) = 0;
  virtual QCP::Interaction selectionCategory() const;
  void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // introduced virtual methods:
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const = 0;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;

  // non-virtual methods:
  //void coordsToPixels(double key, double value, double &x, double &y) const;
  const QPointF coordsToPixels(double key, double value) const;
  //void pixelsToCoords(double x, double y, double &key, double &value) const;
  void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;
  QPen mainPen() const;
  QBrush mainBrush() const;
  void applyFillAntialiasingHint(QCPPainter *painter) const;
  void applyScattersAntialiasingHint(QCPPainter *painter) const;
  void applyErrorBarsAntialiasingHint(QCPPainter *painter) const;
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;
};


class QCPItemAnchor
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name, int anchorId=-1);
  virtual ~QCPItemAnchor();

  // getters:
  QString name() const;
  virtual QPointF pixelPoint() const;

protected:
  // introduced virtual methods:
  virtual QCPItemPosition *toQCPItemPosition();

  // non-virtual methods:
  void addChildX(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  void removeChildX(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
  void addChildY(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  void removeChildY(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted

private:
  QCPItemAnchor(const QCPItemAnchor &);
};


class QCPItemPosition : public QCPItemAnchor
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum PositionType { ptAbsolute        ///< Static positioning in pixels, starting from the top left corner of the viewport/widget.
                      ,ptViewportRatio  ///< Static positioning given by a fraction of the viewport size. For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the viewport/widget. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the viewport/widget, etc.
                      ,ptAxisRectRatio  ///< Static positioning given by a fraction of the axis rect size (see \ref setAxisRect). For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the axis rect. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the axis rect, etc. You can also go beyond the axis rect by providing negative coordinates or coordinates larger than 1.
                      ,ptPlotCoords     ///< Dynamic positioning at a plot coordinate defined by two axes (see \ref setAxes).
                    };

  QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name);
  virtual ~QCPItemPosition();

  // getters:
  PositionType type() const;
  PositionType typeX() const;
  PositionType typeY() const;
  QCPItemAnchor *parentAnchor() const;
  QCPItemAnchor *parentAnchorX() const;
  QCPItemAnchor *parentAnchorY() const;
  double key() const;
  double value() const;
  QPointF coords() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  QCPAxisRect *axisRect() const;
  virtual QPointF pixelPoint() const;

  // setters:
  void setType(PositionType type);
  void setTypeX(PositionType type);
  void setTypeY(PositionType type);
  bool setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  void setCoords(double key, double value);
  void setCoords(const QPointF &coords);
  void setAxes(QCPAxis* keyAxis, QCPAxis* valueAxis);
  void setAxisRect(QCPAxisRect *axisRect);
  void setPixelPoint(const QPointF &pixelPoint);

protected:
  // reimplemented virtual methods:
  virtual QCPItemPosition *toQCPItemPosition();
};


class QCPAbstractItem : public QCPLayerable /Abstract/
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPAbstractItem(QCustomPlot *parentPlot);
  virtual ~QCPAbstractItem();

  // getters:
  bool clipToAxisRect() const;
  QCPAxisRect *clipAxisRect() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setClipToAxisRect(bool clip);
  void setClipAxisRect(QCPAxisRect *rect);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;

  // non-virtual methods:
  QList<QCPItemPosition*> positions() const;
  QList<QCPItemAnchor*> anchors() const;
  QCPItemPosition *position(const QString &name) const;
  QCPItemAnchor *anchor(const QString &name) const;
  bool hasAnchor(const QString &name) const;

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);

protected:
  // reimplemented virtual methods:
  virtual QCP::Interaction selectionCategory() const;
  virtual QRect clipRect() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // introduced virtual methods:
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;
  double rectSelectTest(const QRectF &rect, const QPointF &pos, bool filledRect) const;
  QCPItemPosition *createPosition(const QString &name);
  QCPItemAnchor *createAnchor(const QString &name, int anchorId);
};


class QCustomPlot : public QWidget
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum LayerInsertMode { limBelow  ///< Layer is inserted below other layer
                         ,limAbove ///< Layer is inserted above other layer
                       };
  enum RefreshPriority { rpImmediate ///< The QCustomPlot surface is immediately refreshed, by calling QWidget::repaint() after the replot
                         ,rpQueued   ///< Queues the refresh such that it is performed at a slightly delayed point in time after the replot, by calling QWidget::update() after the replot
                         ,rpHint     ///< Whether to use immediate repaint or queued update depends on whether the plotting hint \ref QCP::phForceRepaint is set, see \ref setPlottingHints.
                       };

  explicit QCustomPlot(QWidget *parent = 0);
  virtual ~QCustomPlot();

  // getters:
  QRect viewport() const;
  QPixmap background() const;
  bool backgroundScaled() const;
  Qt::AspectRatioMode backgroundScaledMode() const;
  QCPLayoutGrid *plotLayout() const;
  QCP::AntialiasedElements antialiasedElements() const;
  QCP::AntialiasedElements notAntialiasedElements() const;
  bool autoAddPlottableToLegend() const;
  const QCP::Interactions interactions() const;
  int selectionTolerance() const;
  bool noAntialiasingOnDrag() const;
  QCP::PlottingHints plottingHints() const;
  Qt::KeyboardModifier multiSelectModifier();

  // setters:
  void setViewport(const QRect &rect);
  void setBackground(const QPixmap &pm);
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  void setBackground(const QBrush &brush);
  void setBackgroundScaled(bool scaled);
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  void setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements);
  void setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled=true);
  void setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements);
  void setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled=true);
  void setAutoAddPlottableToLegend(bool on);
  void setInteractions(const QCP::Interactions &interactions);
  void setInteraction(const QCP::Interaction &interaction, bool enabled=true);
  void setSelectionTolerance(int pixels);
  void setNoAntialiasingOnDrag(bool enabled);
  void setPlottingHints(const QCP::PlottingHints &hints);
  void setPlottingHint(QCP::PlottingHint hint, bool enabled=true);
  void setMultiSelectModifier(Qt::KeyboardModifier modifier);

  // non-property methods:
  // plottable interface:
  QCPAbstractPlottable *plottable(int index);
  QCPAbstractPlottable *plottable();
  bool addPlottable(QCPAbstractPlottable *plottable);
  bool removePlottable(QCPAbstractPlottable *plottable);
  bool removePlottable(int index);
  int clearPlottables();
  int plottableCount() const;
  QList<QCPAbstractPlottable*> selectedPlottables() const;
  QCPAbstractPlottable *plottableAt(const QPointF &pos, bool onlySelectable=false) const;
  QCPBars *plottableBarsAt(const QPointF &pos, bool onlySelectable=false) const;
  QCPGraph *plottableGraphAt(const QPointF &pos, bool onlySelectable=false) const;
  bool hasPlottable(QCPAbstractPlottable *plottable) const;

  // specialized interface for QCPGraph:
  QCPGraph *graph(int index) const;
  QCPGraph *graph() const;
  QCPGraph *addGraph(QCPAxis *keyAxis=0, QCPAxis *valueAxis=0);
  bool removeGraph(QCPGraph *graph);
  bool removeGraph(int index);
  int clearGraphs();
  int graphCount() const;
  QList<QCPGraph*> selectedGraphs() const;

  // item interface:
  QCPAbstractItem *item(int index) const;
  QCPAbstractItem *item() const;
  bool addItem(QCPAbstractItem* item);
  bool removeItem(QCPAbstractItem *item);
  bool removeItem(int index);
  int clearItems();
  int itemCount() const;
  QList<QCPAbstractItem*> selectedItems() const;
  QCPAbstractItem *itemAt(const QPointF &pos, bool onlySelectable=false) const;
  bool hasItem(QCPAbstractItem *item) const;

  // layer interface:
  QCPLayer *layer(const QString &name) const;
  QCPLayer *layer(int index) const;
  QCPLayer *currentLayer() const;
  bool setCurrentLayer(const QString &name);
  bool setCurrentLayer(QCPLayer *layer);
  int layerCount() const;
  bool addLayer(const QString &name, QCPLayer *otherLayer=0, LayerInsertMode insertMode=limAbove);
  bool removeLayer(QCPLayer *layer);
  bool moveLayer(QCPLayer *layer, QCPLayer *otherLayer, LayerInsertMode insertMode=limAbove);

  // axis rect/layout interface:
  int axisRectCount() const;
  QCPAxisRect* axisRect(int index=0) const;
  QList<QCPAxisRect*> axisRects() const;
  QCPLayoutElement* layoutElementAt(const QPointF &pos) const;
  Q_SLOT void rescaleAxes(bool onlyVisiblePlottables=false);

  QList<QCPAxis*> selectedAxes() const;
  QList<QCPLegend*> selectedLegends() const;
  Q_SLOT void deselectAll();

  bool savePdf(const QString &fileName, bool noCosmeticPen=false, int width=0, int height=0, const QString &pdfCreator=QString(), const QString &pdfTitle=QString());
  bool savePng(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
  bool saveJpg(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
  bool saveBmp(const QString &fileName, int width=0, int height=0, double scale=1.0);
  bool saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality=-1);
  QPixmap toPixmap(int width=0, int height=0, double scale=1.0);
  void toPainter(QCPPainter *painter, int width=0, int height=0);
  Q_SLOT void replot(QCustomPlot::RefreshPriority refreshPriority=QCustomPlot::rpHint);

  QCPAxis *xAxis;
  QCPAxis *yAxis;
  QCPAxis *xAxis2;
  QCPAxis *yAxis2;
  QCPLegend *legend;

signals:
  void mouseDoubleClick(QMouseEvent *event);
  void mousePress(QMouseEvent *event);
  void mouseMove(QMouseEvent *event);
  void mouseRelease(QMouseEvent *event);
  void mouseWheel(QWheelEvent *event);

  void plottableClick(QCPAbstractPlottable *plottable, QMouseEvent *event);
  void plottableDoubleClick(QCPAbstractPlottable *plottable, QMouseEvent *event);
  void itemClick(QCPAbstractItem *item, QMouseEvent *event);
  void itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event);
  void axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
  void axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
  void legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
  void legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event);
  void titleClick(QMouseEvent *event, QCPPlotTitle *title);
  void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);

  void selectionChangedByUser();
  void beforeReplot();
  void afterReplot();

protected:
  // reimplemented virtual methods:
  virtual QSize minimumSizeHint() const;
  virtual QSize sizeHint() const;
  virtual void paintEvent(QPaintEvent *event);
  virtual void resizeEvent(QResizeEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

  // introduced virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void axisRemoved(QCPAxis *axis);
  virtual void legendRemoved(QCPLegend *legend);

  // non-virtual methods:
  void updateLayerIndices() const;
  QCPLayerable *layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails=0) const;
  void drawBackground(QCPPainter *painter);
};

%MappedType QMap<double, QColor>
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <qmap.h>
#include <qcolor.h>
%End
    %ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QColor>::const_iterator it = sipCpp->constBegin();
    QMap<double, QColor>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);

            return 0;
        }

        QColor *v = new QColor(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QColor,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);

            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);

            return 0;
        }

        ++it;
    }

    return d;
    %End

    %ConvertToTypeCode
        if (!sipIsErr)
            return PyDict_Check(sipPy);

            QMap<double, QColor> *qm = new QMap<double, QColor>;

            SIP_SSIZE_T pos = 0;
            PyObject *kobj, *vobj;

            while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
            {
                PyErr_Clear();
                double k = PyFloat_AsDouble(kobj);

                if (PyErr_Occurred())
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a key has type '%s' but 'float' is expected",
                                 Py_TYPE(kobj)->tp_name);

                    delete qm;
                    *sipIsErr = 1;

                    return 0;
                }

                int vstate;
                QColor *v = reinterpret_cast< QColor *>(
                            sipForceConvertToType(vobj, sipType_QColor, sipTransferObj,
                                                  SIP_NOT_NONE, &vstate, sipIsErr));

                if (*sipIsErr)
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a value has type '%s' but 'QColor' is expected",
                                 Py_TYPE(vobj)->tp_name);

                    delete qm;
                    return 0;
                }

                qm->insert(k, *v);

                sipReleaseType(v, sipType_QCPBarData, vstate);
            }

            *sipCppPtr = qm;

            return sipGetState(sipTransferObj);
    %End
};

class QCPColorGradient
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum ColorInterpolation { ciRGB  ///< Color channels red, green and blue are linearly interpolated
                            ,ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
                          };
  enum GradientPreset { gpGrayscale  ///< Continuous lightness from black to white (suited for non-biased data representation)
                        ,gpHot       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
                        ,gpCold      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
                        ,gpNight     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
                        ,gpCandy     ///< Blue over pink to white
                        ,gpGeography ///< Colors suitable to represent different elevations on geographical maps
                        ,gpIon       ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
                        ,gpThermal   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
                        ,gpPolar     ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
                        ,gpSpectrum  ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
                        ,gpJet       ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
                        ,gpHues      ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
                      };
  QCPColorGradient(GradientPreset preset=gpCold);
  bool operator==(const QCPColorGradient &other) const;
  bool operator!=(const QCPColorGradient &other) const;

  // getters:
  int levelCount() const;
  QMap<double, QColor> colorStops() const;
  ColorInterpolation colorInterpolation() const;
  bool periodic() const;

  // setters:
  void setLevelCount(int n);
  void setColorStops(const QMap<double, QColor> &colorStops);
  void setColorStopAt(double position, const QColor &color);
  void setColorInterpolation(ColorInterpolation interpolation);
  void setPeriodic(bool enabled);

  // non-property methods:
  void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false);
  QRgb color(double position, const QCPRange &range, bool logarithmic=false);
  void loadPreset(GradientPreset preset);
  void clearColorStops();
  QCPColorGradient inverted() const;

protected:
  void updateColorBuffer();
};


class QCPAxisRect : public QCPLayoutElement
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes=true);
  virtual ~QCPAxisRect();

  // getters:
  QPixmap background() const;
  bool backgroundScaled() const;
  Qt::AspectRatioMode backgroundScaledMode() const;
  Qt::Orientations rangeDrag() const;
  Qt::Orientations rangeZoom() const;
  QCPAxis *rangeDragAxis(Qt::Orientation orientation);
  QCPAxis *rangeZoomAxis(Qt::Orientation orientation);
  double rangeZoomFactor(Qt::Orientation orientation);

  // setters:
  void setBackground(const QPixmap &pm);
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  void setBackground(const QBrush &brush);
  void setBackgroundScaled(bool scaled);
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  void setRangeDrag(Qt::Orientations orientations);
  void setRangeZoom(Qt::Orientations orientations);
  void setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeZoomFactor(double horizontalFactor, double verticalFactor);
  void setRangeZoomFactor(double factor);

  // non-property methods:
  int axisCount(QCPAxis::AxisType type) const;
  QCPAxis *axis(QCPAxis::AxisType type, int index=0) const;
  QList<QCPAxis*> axes(QCPAxis::AxisTypes types) const;
  QList<QCPAxis*> axes() const;
  QCPAxis *addAxis(QCPAxis::AxisType type, QCPAxis *axis=0);
  QList<QCPAxis*> addAxes(QCPAxis::AxisTypes types);
  bool removeAxis(QCPAxis *axis);
  QCPLayoutInset *insetLayout() const;

  void setupFullAxesBox(bool connectRanges=false);
  QList<QCPAbstractPlottable*> plottables() const;
  QList<QCPGraph*> graphs() const;
  QList<QCPAbstractItem*> items() const;

  // read-only interface imitating a QRect:
  int left() const;
  int right() const;
  int top() const;
  int bottom() const;
  int width() const;
  int height() const;
  QSize size() const;
  QPoint topLeft() const;
  QPoint topRight() const;
  QPoint bottomLeft() const;
  QPoint bottomRight() const;
  QPoint center() const;

  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;

protected:
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

  // non-property methods:
  void drawBackground(QCPPainter *painter);
  void updateAxesOffset(QCPAxis::AxisType type);
};


class QCPAbstractLegendItem : public QCPLayoutElement /Abstract/
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPAbstractLegendItem(QCPLegend *parent);

  // getters:
  QCPLegend *parentLegend() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);

protected:
  // reimplemented virtual methods:
  virtual QCP::Interaction selectionCategory() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual QRect clipRect() const;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
};


class QCPPlottableLegendItem : public QCPAbstractLegendItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);

  // getters:
  QCPAbstractPlottable *plottable();

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QSize minimumSizeHint() const;

  // non-virtual methods:
  QPen getIconBorderPen() const;
  QColor getTextColor() const;
  QFont getFont() const;
};


class QCPLegend : public QCPLayoutGrid
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum SelectablePart { spNone       = 0x000  ///< <tt>0x000</tt> None
                        ,spLegendBox  = 0x001 ///< <tt>0x001</tt> The legend box (frame)
                        ,spItems      = 0x002 ///< <tt>0x002</tt> Legend items individually (see \ref selectedItems)
                      };
  typedef QFlags<QCPLegend::SelectablePart> SelectableParts;

  explicit QCPLegend();
  virtual ~QCPLegend();

  // getters:
  QPen borderPen() const;
  QBrush brush() const;
  QFont font() const;
  QColor textColor() const;
  QSize iconSize() const;
  int iconTextPadding() const;
  QPen iconBorderPen() const;
  SelectableParts selectableParts() const;
  SelectableParts selectedParts() const;
  QPen selectedBorderPen() const;
  QPen selectedIconBorderPen() const;
  QBrush selectedBrush() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;

  // setters:
  void setBorderPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setIconSize(const QSize &size);
  void setIconSize(int width, int height);
  void setIconTextPadding(int padding);
  void setIconBorderPen(const QPen &pen);
  Q_SLOT void setSelectableParts(const SelectableParts &selectableParts);
  Q_SLOT void setSelectedParts(const SelectableParts &selectedParts);
  void setSelectedBorderPen(const QPen &pen);
  void setSelectedIconBorderPen(const QPen &pen);
  void setSelectedBrush(const QBrush &brush);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-virtual methods:
  QCPAbstractLegendItem *item(int index) const;
  QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;
  int itemCount() const;
  bool hasItem(QCPAbstractLegendItem *item) const;
  bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;
  bool addItem(QCPAbstractLegendItem *item);
  bool removeItem(int index);
  bool removeItem(QCPAbstractLegendItem *item);
  void clearItems();
  QList<QCPAbstractLegendItem*> selectedItems() const;

signals:
  void selectionChanged(QCPLegend::SelectableParts parts);
  void selectableChanged(QCPLegend::SelectableParts parts);

protected:
  // reimplemented virtual methods:
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  virtual QCP::Interaction selectionCategory() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // non-virtual methods:
  QPen getBorderPen() const;
  QBrush getBrush() const;
};


class QCPPlotTitle : public QCPLayoutElement
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPPlotTitle(QCustomPlot *parentPlot);
  explicit QCPPlotTitle(QCustomPlot *parentPlot, const QString &text);

  // getters:
  QString text() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setText(const QString &text);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);

protected:
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);

  // non-virtual methods:
  QFont mainFont() const;
  QColor mainTextColor() const;
};


class QCPColorScaleAxisRectPrivate : public QCPAxisRect
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale);
protected:
  virtual void draw(QCPPainter *painter);
  void updateGradientImage();
  Q_SLOT void axisSelectionChanged(QCPAxis::SelectableParts selectedParts);
  Q_SLOT void axisSelectableChanged(QCPAxis::SelectableParts selectableParts);
};


class QCPColorScale : public QCPLayoutElement
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPColorScale(QCustomPlot *parentPlot);
  virtual ~QCPColorScale();

  // getters:
  QCPAxis *axis() const;
  QCPAxis::AxisType type() const;
  QCPRange dataRange() const;
  QCPAxis::ScaleType dataScaleType() const;
  QCPColorGradient gradient() const;
  QString label() const;
  int barWidth () const;
  bool rangeDrag() const;
  bool rangeZoom() const;

  // setters:
  void setType(QCPAxis::AxisType type);
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  void setLabel(const QString &str);
  void setBarWidth(int width);
  void setRangeDrag(bool enabled);
  void setRangeZoom(bool enabled);

  // non-property methods:
  QList<QCPColorMap*> colorMaps() const;
  void rescaleDataRange(bool onlyVisibleMaps);

  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);

signals:
  void dataRangeChanged(QCPRange newRange);
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  void gradientChanged(QCPColorGradient newGradient);

protected:
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
};


class QCPData
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPData();
  QCPData(double key, double value);
  double key;
  double value;
  double keyErrorPlus;
  double keyErrorMinus;
  double valueErrorPlus;
  double valueErrorMinus;
};

typedef QMap<double, QCPData> QCPDataMap;
typedef QMapIterator<double, QCPData> QCPDataMapIterator;
typedef QMutableMapIterator<double, QCPData> QCPDataMutableMapIterator;

%MappedType QCPDataMap
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <qmap.h>
#include <qcustomplot.h>
%End
    %ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QCPData>::const_iterator it = sipCpp->constBegin();
    QMap<double, QCPData>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);

            return 0;
        }

        QCPData *v = new QCPData(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QCPData,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);

            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);

            return 0;
        }

        ++it;
    }

    return d;
    %End

    %ConvertToTypeCode
        if (!sipIsErr)
            return PyDict_Check(sipPy);

            QMap<double, QCPData> *qm = new QMap<double, QCPData>;

            SIP_SSIZE_T pos = 0;
            PyObject *kobj, *vobj;

            while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
            {
                PyErr_Clear();
                double k = PyFloat_AsDouble(kobj);

                if (PyErr_Occurred())
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a key has type '%s' but 'float' is expected",
                                 Py_TYPE(kobj)->tp_name);

                    delete qm;
                    *sipIsErr = 1;

                    return 0;
                }

                int vstate;
                QCPData *v = reinterpret_cast< QCPData *>(
                            sipForceConvertToType(vobj, sipType_QCPData, sipTransferObj,
                                                  SIP_NOT_NONE, &vstate, sipIsErr));

                if (*sipIsErr)
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a value has type '%s' but 'QCPData' is expected",
                                 Py_TYPE(vobj)->tp_name);

                    delete qm;
                    return 0;
                }

                qm->insert(k, *v);

                sipReleaseType(v, sipType_QCPData, vstate);
            }

            *sipCppPtr = qm;

            return sipGetState(sipTransferObj);
    %End
};


class QCPGraph : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum LineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
                                 ///< with symbols according to the scatter style, see \ref setScatterStyle)
                   ,lsLine       ///< data points are connected by a straight line
                   ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
                   ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
                   ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
                   ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
                 };
  enum ErrorType { etNone   ///< No error bars are shown
                   ,etKey   ///< Error bars for the key dimension of the data point are shown
                   ,etValue ///< Error bars for the value dimension of the data point are shown
                   ,etBoth  ///< Error bars for both key and value dimensions of the data point are shown
                 };

  explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPGraph();

  // getters:
  QCPDataMap *data() const;
  LineStyle lineStyle() const;
  QCPScatterStyle scatterStyle() const;
  ErrorType errorType() const;
  QPen errorPen() const;
  double errorBarSize() const;
  bool errorBarSkipSymbol() const;
  QCPGraph *channelFillGraph() const;
  bool adaptiveSampling() const;

  // setters:
  void setData(QCPDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueError);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError, const QVector<double> &valueError);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setLineStyle(LineStyle ls);
  void setScatterStyle(const QCPScatterStyle &style);
  void setErrorType(ErrorType errorType);
  void setErrorPen(const QPen &pen);
  void setErrorBarSize(double size);
  void setErrorBarSkipSymbol(bool enabled);
  void setChannelFillGraph(QCPGraph *targetGraph);
  void setAdaptiveSampling(bool enabled);

  // non-property methods:
  void addData(const QCPDataMap &dataMap);
  void addData(const QCPData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);

  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
//  using QCPAbstractPlottable::rescaleAxes;
//  using QCPAbstractPlottable::rescaleKeyAxis;
//  using QCPAbstractPlottable::rescaleValueAxis;
  void rescaleAxes(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleKeyAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleValueAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface

  // introduced virtual methods:
  virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const;
  virtual void drawLinePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawImpulsePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;

  // non-virtual methods:
  void getPreparedData(QVector<QCPData> *lineData, QVector<QCPData> *scatterData) const;
  void getPlotData(QVector<QPointF> *lineData, QVector<QCPData> *scatterData) const;
  void getScatterPlotData(QVector<QCPData> *scatterData) const;
  void getLinePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepLeftPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepRightPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepCenterPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getImpulsePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void drawError(QCPPainter *painter, double x, double y, const QCPData &data) const;
//TODO: реализовать корректную сигнатуру
//  void getVisibleDataBounds(QCPDataMap::const_iterator &lower, QCPDataMap::const_iterator &upper) const;
//  int countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const;
  void addFillBasePoints(QVector<QPointF> *lineData) const;
  void removeFillBasePoints(QVector<QPointF> *lineData) const;
  QPointF lowerFillBasePoint(double lowerKey) const;
  QPointF upperFillBasePoint(double upperKey) const;
  const QPolygonF getChannelFillPolygon(const QVector<QPointF> *lineData) const;
  int findIndexBelowX(const QVector<QPointF> *data, double x) const;
  int findIndexAboveX(const QVector<QPointF> *data, double x) const;
  int findIndexBelowY(const QVector<QPointF> *data, double y) const;
  int findIndexAboveY(const QVector<QPointF> *data, double y) const;
  double pointDistance(const QPointF &pixelPoint) const;
};


class QCPCurveData
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPCurveData();
  QCPCurveData(double t, double key, double value);
  double t;
  double key;
  double value;
};


typedef QMap<double, QCPCurveData> QCPCurveDataMap;
typedef QMapIterator<double, QCPCurveData> QCPCurveDataMapIterator;
typedef QMutableMapIterator<double, QCPCurveData> QCPCurveDataMutableMapIterator;

%MappedType QCPCurveDataMap
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <qmap.h>
#include <qcustomplot.h>
%End
    %ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QCPCurveData>::const_iterator it = sipCpp->constBegin();
    QMap<double, QCPCurveData>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);

            return 0;
        }

        QCPCurveData *v = new QCPCurveData(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QCPCurveData,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);

            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);

            return 0;
        }

        ++it;
    }

    return d;
    %End

    %ConvertToTypeCode
        if (!sipIsErr)
            return PyDict_Check(sipPy);

            QMap<double, QCPCurveData> *qm = new QMap<double, QCPCurveData>;

            SIP_SSIZE_T pos = 0;
            PyObject *kobj, *vobj;

            while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
            {
                PyErr_Clear();
                double k = PyFloat_AsDouble(kobj);

                if (PyErr_Occurred())
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a key has type '%s' but 'float' is expected",
                                 Py_TYPE(kobj)->tp_name);

                    delete qm;
                    *sipIsErr = 1;

                    return 0;
                }

                int vstate;
                QCPCurveData *v = reinterpret_cast< QCPCurveData *>(
                            sipForceConvertToType(vobj, sipType_QCPCurveData, sipTransferObj,
                                                  SIP_NOT_NONE, &vstate, sipIsErr));

                if (*sipIsErr)
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a value has type '%s' but 'QCPCurveData' is expected",
                                 Py_TYPE(vobj)->tp_name);

                    delete qm;
                    return 0;
                }

                qm->insert(k, *v);

                sipReleaseType(v, sipType_QCPCurveData, vstate);
            }

            *sipCppPtr = qm;

            return sipGetState(sipTransferObj);
    %End
};

class QCPCurve : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum LineStyle { lsNone  ///< No line is drawn between data points (e.g. only scatters)
                   ,lsLine ///< Data points are connected with a straight line
                 };
  explicit QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPCurve();

  // getters:
  QCPCurveDataMap *data() const;
  QCPScatterStyle scatterStyle() const;
  LineStyle lineStyle() const;

  // setters:
  void setData(QCPCurveDataMap *data, bool copy=false);
  void setData(const QVector<double> &t, const QVector<double> &key, const QVector<double> &value);
  void setData(const QVector<double> &key, const QVector<double> &value);
  void setScatterStyle(const QCPScatterStyle &style);
  void setLineStyle(LineStyle style);

  // non-property methods:
  void addData(const QCPCurveDataMap &dataMap);
  void addData(const QCPCurveData &data);
  void addData(double t, double key, double value);
  void addData(double key, double value);
  void addData(const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double t);
  void removeDataAfter(double t);
  void removeData(double fromt, double tot);
  void removeData(double t);

  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;

  // introduced virtual methods:
  virtual void drawScatterPlot(QCPPainter *painter, const QVector<QPointF> *pointData) const;

  // non-virtual methods:
  void getCurveData(QVector<QPointF> *lineData) const;
  int getRegion(double x, double y, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  QPointF getOptimizedPoint(int prevRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  QVector<QPointF> getOptimizedCornerPoints(int prevRegion, int currentRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  bool mayTraverse(int prevRegion, int currentRegion) const;
  bool getTraverse(double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom, QPointF &crossA, QPointF &crossB) const;
  void getTraverseCornerPoints(int prevRegion, int currentRegion, double rectLeft, double rectTop, double rectRight, double rectBottom, QVector<QPointF> &beforeTraverse, QVector<QPointF> &afterTraverse) const;
  double pointDistance(const QPointF &pixelPoint) const;
};


class QCPBarsGroup : public QObject
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum SpacingType { stAbsolute       ///< Bar spacing is in absolute pixels
                     ,stAxisRectRatio ///< Bar spacing is given by a fraction of the axis rect size
                     ,stPlotCoords    ///< Bar spacing is in key coordinates and thus scales with the key axis range
                 };
  QCPBarsGroup(QCustomPlot *parentPlot);
  ~QCPBarsGroup();

  // getters:
  SpacingType spacingType() const;
  double spacing() const;

  // setters:
  void setSpacingType(SpacingType spacingType);
  void setSpacing(double spacing);

  // non-virtual methods:
  QList<QCPBars*> bars() const;
  QCPBars* bars(int index) const;
  int size() const;
  bool isEmpty() const;
  void clear();
  bool contains(QCPBars *bars) const;
  void append(QCPBars *bars);
  void insert(int i, QCPBars *bars);
  void remove(QCPBars *bars);

protected:
  // non-virtual methods:
  void registerBars(QCPBars *bars);
  void unregisterBars(QCPBars *bars);

  // virtual methods:
  double keyPixelOffset(const QCPBars *bars, double keyCoord);
  double getPixelSpacing(const QCPBars *bars, double keyCoord);
};


class QCPBarData
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPBarData();
  QCPBarData(double key, double value);
  double key;
  double value;
};


typedef QMap<double, QCPBarData> QCPBarDataMap;
typedef QMapIterator<double, QCPBarData> QCPBarDataMapIterator;
typedef QMutableMapIterator<double, QCPBarData> QCPBarDataMutableMapIterator;

%MappedType QCPBarDataMap
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <qmap.h>
#include <qcustomplot.h>
%End
    %ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QCPBarData>::const_iterator it = sipCpp->constBegin();
    QMap<double, QCPBarData>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);

            return 0;
        }

        QCPBarData *v = new QCPBarData(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QCPBarData,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);

            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);

            return 0;
        }

        ++it;
    }

    return d;
    %End

    %ConvertToTypeCode
        if (!sipIsErr)
            return PyDict_Check(sipPy);

            QMap<double, QCPBarData> *qm = new QMap<double, QCPBarData>;

            SIP_SSIZE_T pos = 0;
            PyObject *kobj, *vobj;

            while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
            {
                PyErr_Clear();
                double k = PyFloat_AsDouble(kobj);

                if (PyErr_Occurred())
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a key has type '%s' but 'float' is expected",
                                 Py_TYPE(kobj)->tp_name);

                    delete qm;
                    *sipIsErr = 1;

                    return 0;
                }

                int vstate;
                QCPBarData *v = reinterpret_cast< QCPBarData *>(
                            sipForceConvertToType(vobj, sipType_QCPBarData, sipTransferObj,
                                                  SIP_NOT_NONE, &vstate, sipIsErr));

                if (*sipIsErr)
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a value has type '%s' but 'QCPBarData' is expected",
                                 Py_TYPE(vobj)->tp_name);

                    delete qm;
                    return 0;
                }

                qm->insert(k, *v);

                sipReleaseType(v, sipType_QCPBarData, vstate);
            }

            *sipCppPtr = qm;

            return sipGetState(sipTransferObj);
    %End
};

class QCPBars : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum WidthType { wtAbsolute       ///< Bar width is in absolute pixels
                   ,wtAxisRectRatio ///< Bar width is given by a fraction of the axis rect size
                   ,wtPlotCoords    ///< Bar width is in key coordinates and thus scales with the key axis range
                 };

  explicit QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPBars();

  // getters:
  double width() const;
  WidthType widthType() const;
  QCPBarsGroup *barsGroup() const;
  double baseValue() const;
  QCPBars *barBelow() const;
  QCPBars *barAbove() const;
  QCPBarDataMap *data() const;

  // setters:
  void setWidth(double width);
  void setWidthType(WidthType widthType);
  void setBarsGroup(QCPBarsGroup *barsGroup);
  void setBaseValue(double baseValue);
  void setData(QCPBarDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);

  // non-property methods:
  void moveBelow(QCPBars *bars);
  void moveAbove(QCPBars *bars);
  void addData(const QCPBarDataMap &dataMap);
  void addData(const QCPBarData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);

  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;

  // non-virtual methods:
//TODO: Реализовать корректную сигнатуру
//  void getVisibleDataBounds(QCPBarDataMap::const_iterator &lower, QCPBarDataMap::const_iterator &upperEnd) const;
  QPolygonF getBarPolygon(double key, double value) const;
  void getPixelWidth(double key, double &lower, double &upper) const;
  double getStackedBaseValue(double key, bool positive) const;
  static void connectBars(QCPBars* lower, QCPBars* upper);
};


class QCPStatisticalBox : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis);

  // getters:
  double key() const;
  double minimum() const;
  double lowerQuartile() const;
  double median() const;
  double upperQuartile() const;
  double maximum() const;
  QVector<double> outliers() const;
  double width() const;
  double whiskerWidth() const;
  QPen whiskerPen() const;
  QPen whiskerBarPen() const;
  QPen medianPen() const;
  QCPScatterStyle outlierStyle() const;

  // setters:
  void setKey(double key);
  void setMinimum(double value);
  void setLowerQuartile(double value);
  void setMedian(double value);
  void setUpperQuartile(double value);
  void setMaximum(double value);
  void setOutliers(const QVector<double> &values);
  void setData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum);
  void setWidth(double width);
  void setWhiskerWidth(double width);
  void setWhiskerPen(const QPen &pen);
  void setWhiskerBarPen(const QPen &pen);
  void setMedianPen(const QPen &pen);
  void setOutlierStyle(const QCPScatterStyle &style);

  // non-property methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;

  // introduced virtual methods:
  virtual void drawQuartileBox(QCPPainter *painter, QRectF *quartileBox=0) const;
  virtual void drawMedian(QCPPainter *painter) const;
  virtual void drawWhiskers(QCPPainter *painter) const;
  virtual void drawOutliers(QCPPainter *painter) const;
};


class QCPColorMapData
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange);
  ~QCPColorMapData();
  QCPColorMapData(const QCPColorMapData &other);
//  QCPColorMapData &operator=(const QCPColorMapData &other);

  // getters:
  int keySize() const;
  int valueSize() const;
  QCPRange keyRange() const;
  QCPRange valueRange() const;
  QCPRange dataBounds() const;
  double data(double key, double value);
  double cell(int keyIndex, int valueIndex);

  // setters:
  void setSize(int keySize, int valueSize);
  void setKeySize(int keySize);
  void setValueSize(int valueSize);
  void setRange(const QCPRange &keyRange, const QCPRange &valueRange);
  void setKeyRange(const QCPRange &keyRange);
  void setValueRange(const QCPRange &valueRange);
  void setData(double key, double value, double z);
  void setCell(int keyIndex, int valueIndex, double z);

  // non-property methods:
  void recalculateDataBounds();
  void clear();
  void fill(double z);
  bool isEmpty() const;
  void coordToCell(double key, double value, int *keyIndex, int *valueIndex) const;
  void cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const;
};


class QCPColorMap : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPColorMap();

  // getters:
  QCPColorMapData *data() const;
  QCPRange dataRange() const;
  QCPAxis::ScaleType dataScaleType() const;
  bool interpolate() const;
  bool tightBoundary() const;
  QCPColorGradient gradient() const;
  QCPColorScale *colorScale() const;

  // setters:
  void setData(QCPColorMapData *data, bool copy=false);
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  void setInterpolate(bool enabled);
  void setTightBoundary(bool enabled);
  void setColorScale(QCPColorScale *colorScale);

  // non-property methods:
  void rescaleDataRange(bool recalculateDataBounds=false);
  Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode=Qt::SmoothTransformation, const QSize &thumbSize=QSize(32, 18));

  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

signals:
  void dataRangeChanged(QCPRange newRange);
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  void gradientChanged(QCPColorGradient newGradient);

protected:
  // introduced virtual methods:
  virtual void updateMapImage();

  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
};


class QCPFinancialData
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPFinancialData();
  QCPFinancialData(double key, double open, double high, double low, double close);
  double key;
  double open;
  double high;
  double low;
  double close;
};


typedef QMap<double, QCPFinancialData> QCPFinancialDataMap;
typedef QMapIterator<double, QCPFinancialData> QCPFinancialDataMapIterator;
typedef QMutableMapIterator<double, QCPFinancialData> QCPFinancialDataMutableMapIterator;

%MappedType QCPFinancialDataMap
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <qmap.h>
#include <qcustomplot.h>
%End
    %ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QCPFinancialData>::const_iterator it = sipCpp->constBegin();
    QMap<double, QCPFinancialData>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);

            return 0;
        }

        QCPFinancialData *v = new QCPFinancialData(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QCPFinancialData,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);

            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);

            return 0;
        }

        ++it;
    }

    return d;
    %End

    %ConvertToTypeCode
        if (!sipIsErr)
            return PyDict_Check(sipPy);

            QMap<double, QCPFinancialData> *qm = new QMap<double, QCPFinancialData>;

            SIP_SSIZE_T pos = 0;
            PyObject *kobj, *vobj;

            while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
            {
                PyErr_Clear();
                double k = PyFloat_AsDouble(kobj);

                if (PyErr_Occurred())
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a key has type '%s' but 'float' is expected",
                                 Py_TYPE(kobj)->tp_name);

                    delete qm;
                    *sipIsErr = 1;

                    return 0;
                }

                int vstate;
                QCPFinancialData *v = reinterpret_cast< QCPFinancialData *>(
                            sipForceConvertToType(vobj, sipType_QCPFinancialData, sipTransferObj,
                                                  SIP_NOT_NONE, &vstate, sipIsErr));

                if (*sipIsErr)
                {
                    PyErr_Format(PyExc_TypeError,
                                 "a value has type '%s' but 'QCPFinancialData' is expected",
                                 Py_TYPE(vobj)->tp_name);

                    delete qm;
                    return 0;
                }

                qm->insert(k, *v);

                sipReleaseType(v, sipType_QCPFinancialData, vstate);
            }

            *sipCppPtr = qm;

            return sipGetState(sipTransferObj);
    %End
};

class QCPFinancial : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum ChartStyle { csOhlc         ///< Open-High-Low-Close bar representation
                   ,csCandlestick  ///< Candlestick representation
                  };

  explicit QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPFinancial();

  // getters:
  QCPFinancialDataMap *data() const;
  ChartStyle chartStyle() const;
  double width() const;
  bool twoColored() const;
  QBrush brushPositive() const;
  QBrush brushNegative() const;
  QPen penPositive() const;
  QPen penNegative() const;

  // setters:
  void setData(QCPFinancialDataMap *data, bool copy=false);
  //void setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void setChartStyle(ChartStyle style);
  void setWidth(double width);
  void setTwoColored(bool twoColored);
  void setBrushPositive(const QBrush &brush);
  void setBrushNegative(const QBrush &brush);
  void setPenPositive(const QPen &pen);
  void setPenNegative(const QPen &pen);

  // non-property methods:
  void addData(const QCPFinancialDataMap &dataMap);
  void addData(const QCPFinancialData &data);
  void addData(double key, double open, double high, double low, double close);
  void addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);

  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // static methods:
  static QCPFinancialDataMap timeSeriesToOhlc(const QVector<double> &time, const QVector<double> &value, double timeBinSize, double timeBinOffset = 0);

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;

  // non-virtual methods:
//TODO: Аналогично
//  void drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
//  void drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
//  double ohlcSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
//  double candlestickSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
//  void getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const;
};


class QCPItemStraightLine : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemStraightLine(QCustomPlot *parentPlot);
  virtual ~QCPItemStraightLine();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  //Removed const! for SIP
  QCPItemPosition *point1
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *point2
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  double distToStraightLine(const QVector2D &point1, const QVector2D &vec, const QVector2D &point) const;
  QLineF getRectClippedStraightLine(const QVector2D &point1, const QVector2D &vec, const QRect &rect) const;
  QPen mainPen() const;
};


class QCPItemLine : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemLine(QCustomPlot *parentPlot);
  virtual ~QCPItemLine();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QCPLineEnding head() const;
  QCPLineEnding tail() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setHead(const QCPLineEnding &head);
  void setTail(const QCPLineEnding &tail);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  //Removed const! for SIP
  QCPItemPosition *start
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *end
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  QLineF getRectClippedLine(const QVector2D &start, const QVector2D &end, const QRect &rect) const;
  QPen mainPen() const;
};


class QCPItemCurve : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemCurve(QCustomPlot *parentPlot);
  virtual ~QCPItemCurve();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QCPLineEnding head() const;
  QCPLineEnding tail() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setHead(const QCPLineEnding &head);
  void setTail(const QCPLineEnding &tail);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  //Removed const! for SIP
  QCPItemPosition *start
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *startDir
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *endDir
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *end
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  QPen mainPen() const;
};


class QCPItemRect : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemRect(QCustomPlot *parentPlot);
  virtual ~QCPItemRect();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  //Removed const! for SIP
  QCPItemPosition *topLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *bottomRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *top
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *right
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottom
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *left
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  QPen mainPen() const;
  QBrush mainBrush() const;
};


class QCPItemText : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemText(QCustomPlot *parentPlot);
  virtual ~QCPItemText();

  // getters:
  QColor color() const;
  QColor selectedColor() const;
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  QFont font() const;
  QFont selectedFont() const;
  QString text() const;
  Qt::Alignment positionAlignment() const;
  Qt::Alignment textAlignment() const;
  double rotation() const;
  QMargins padding() const;

  // setters;
  void setColor(const QColor &color);
  void setSelectedColor(const QColor &color);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setSelectedFont(const QFont &font);
  void setText(const QString &text);
  void setPositionAlignment(Qt::Alignment alignment);
  void setTextAlignment(Qt::Alignment alignment);
  void setRotation(double degrees);
  void setPadding(const QMargins &padding);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  QCPItemPosition *position
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *top
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *right
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottom
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *left
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  QPointF getTextDrawPoint(const QPointF &pos, const QRectF &rect, Qt::Alignment positionAlignment) const;
  QFont mainFont() const;
  QColor mainColor() const;
  QPen mainPen() const;
  QBrush mainBrush() const;
};


class QCPItemEllipse : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemEllipse(QCustomPlot *parentPlot);
  virtual ~QCPItemEllipse();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  QCPItemPosition *topLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *bottomRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topLeftRim
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *top
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topRightRim
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *right
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomRightRim
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottom
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomLeftRim
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *left
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  QPen mainPen() const;
  QBrush mainBrush() const;
};


class QCPItemPixmap : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  QCPItemPixmap(QCustomPlot *parentPlot);
  virtual ~QCPItemPixmap();

  // getters:
  QPixmap pixmap() const;
  bool scaled() const;
  Qt::AspectRatioMode aspectRatioMode() const;
  Qt::TransformationMode transformationMode() const;
  QPen pen() const;
  QPen selectedPen() const;

  // setters;
  void setPixmap(const QPixmap &pixmap);
  void setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode=Qt::KeepAspectRatio, Qt::TransformationMode transformationMode=Qt::SmoothTransformation);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  QCPItemPosition *topLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *bottomRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *top
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *topRight
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *right
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottom
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *bottomLeft
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *left
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  void updateScaledPixmap(QRect finalRect=QRect(), bool flipHorz=false, bool flipVert=false);
  QRect getFinalRect(bool *flippedHorz=0, bool *flippedVert=0) const;
  QPen mainPen() const;
};


class QCPItemTracer : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum TracerStyle { tsNone        ///< The tracer is not visible
                     ,tsPlus       ///< A plus shaped crosshair with limited size
                     ,tsCrosshair  ///< A plus shaped crosshair which spans the complete axis rect
                     ,tsCircle     ///< A circle
                     ,tsSquare     ///< A square
                   };

  QCPItemTracer(QCustomPlot *parentPlot);
  virtual ~QCPItemTracer();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  double size() const;
  TracerStyle style() const;
  QCPGraph *graph() const;
  double graphKey() const;
  bool interpolating() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setSize(double size);
  void setStyle(TracerStyle style);
  void setGraph(QCPGraph *graph);
  void setGraphKey(double key);
  void setInterpolating(bool enabled);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-virtual methods:
  void updatePosition();

//  QCPItemPosition * const position;
  QCPItemPosition *position
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  QPen mainPen() const;
  QBrush mainBrush() const;
};


class QCPItemBracket : public QCPAbstractItem
{
%TypeHeaderCode
#include <qcustomplot.h>
%End

public:
  enum BracketStyle { bsSquare  ///< A brace with angled edges
                      ,bsRound  ///< A brace with round edges
                      ,bsCurly  ///< A curly brace
                      ,bsCalligraphic ///< A curly brace with varying stroke width giving a calligraphic impression
  };

  QCPItemBracket(QCustomPlot *parentPlot);
  virtual ~QCPItemBracket();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  double length() const;
  BracketStyle style() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setLength(double length);
  void setStyle(BracketStyle style);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  QCPItemPosition *left
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemPosition *right
  {
      %SetCode
      // This is a *const, no setter
      %End
  };
  QCPItemAnchor *center
  {
      %SetCode
      // This is a *const, no setter
      %End
  };

protected:
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;

  // non-virtual methods:
  QPen mainPen() const;
};

