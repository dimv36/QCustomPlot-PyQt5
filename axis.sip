/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPGrid :public QCPLayerable
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
%End
public:
  explicit QCPGrid(QCPAxis *parentAxis /TransferThis/);

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
};


class QCPAxis : public QCPLayerable
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
%End
public:
  enum AxisType { atLeft    = 0x01  ///< <tt>0x01</tt> Axis is vertical and on the left side of the axis rect
                  ,atRight  = 0x02  ///< <tt>0x02</tt> Axis is vertical and on the right side of the axis rect
                  ,atTop    = 0x04  ///< <tt>0x04</tt> Axis is horizontal and on the top side of the axis rect
                  ,atBottom = 0x08  ///< <tt>0x08</tt> Axis is horizontal and on the bottom side of the axis rect
                };
  typedef QFlags<QCPAxis::AxisType> AxisTypes;

  enum LabelSide { lsInside    ///< Tick labels will be displayed inside the axis rect and clipped to the inner axis rect
                   ,lsOutside  ///< Tick labels will be displayed outside the axis rect
                 };

  enum ScaleType { stLinear       ///< Linear scaling
                   ,stLogarithmic ///< Logarithmic scaling with correspondingly transformed axis coordinates (possibly also \ref setTicker to a \ref QCPAxisTickerLog instance).
                 };

  enum SelectablePart { spNone        = 0      ///< None of the selectable parts
                        ,spAxis       = 0x001  ///< The axis backbone and tick marks
                        ,spTickLabels = 0x002  ///< Tick labels (numbers) of this axis (as a whole, not individually)
                        ,spAxisLabel  = 0x004  ///< The axis label
                      };
  typedef QFlags<QCPAxis::SelectablePart> SelectableParts;

  explicit QCPAxis(QCPAxisRect *parent /TransferThis/, AxisType type);
  virtual ~QCPAxis();

  // getters:
  AxisType axisType() const;
  QCPAxisRect *axisRect() const;
  ScaleType scaleType() const;
  const QCPRange range() const;
  bool rangeReversed() const;
  // TODO: provide %MethodCode%
  //QSharedPointer<QCPAxisTicker> ticker() const;
  bool ticks() const;
  bool tickLabels() const;
  int tickLabelPadding() const;
  QFont tickLabelFont() const;
  QColor tickLabelColor() const;
  double tickLabelRotation() const;
  LabelSide tickLabelSide() const;
  QString numberFormat() const;
  int numberPrecision() const;
  QVector<double> tickVector() const;
  QVector<QString> tickVectorLabels() const;
  int tickLengthIn() const;
  int tickLengthOut() const;
  bool subTicks() const;
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
  Q_SLOT void setRange(const QCPRange &range);
  void setRange(double lower, double upper);
  void setRange(double position, double size, Qt::AlignmentFlag alignment);
  void setRangeLower(double lower);
  void setRangeUpper(double upper);
  void setRangeReversed(bool reversed);
  // TODO: provide %MethodCode%
  //QSharedPointer<QCPAxisTicker> ticker() const;
  //void setTicker(QSharedPointer<QCPAxisTicker> ticker);
  void setTicks(bool show);
  void setTickLabels(bool show);
  void setTickLabelPadding(int padding);
  void setTickLabelFont(const QFont &font);
  void setTickLabelColor(const QColor &color);
  void setTickLabelRotation(double degrees);
  void setTickLabelSide(LabelSide side);
  void setNumberFormat(const QString &formatCode);
  void setNumberPrecision(int precision);
  void setTickLength(int inside, int outside=0);
  void setTickLengthIn(int inside);
  void setTickLengthOut(int outside);
  void setSubTicks(bool show);
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
  int pixelOrientation() const;
  void moveRange(double diff);
  void scaleRange(double factor);
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
  void rangeChanged(const QCPRange &newRange);
  void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
  void scaleTypeChanged(QCPAxis::ScaleType scaleType);
  void selectionChanged(const QCPAxis::SelectableParts &parts);
  void selectableChanged(const QCPAxis::SelectableParts &parts);
};
