/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPGraphData
{
%TypeHeaderCode
#include <QCustomPlot/src/plottables/plottable-graph.h>
%End
public:
  QCPGraphData();
  QCPGraphData(double key, double value);

  double sortKey() const;
  static QCPGraphData fromSortKey(double sortKey);
  static bool sortKeyIsMainKey();

  double mainKey() const;
  double mainValue() const;

  QCPRange valueRange() const;

  double key;
  double value;
};

typedef QCPDataContainer<QCPGraphData> QCPGraphDataContainer;

// Changed inheritance from QCPAbstractPlottable1D<QCPGraphData>
class QCPGraph : public QCPAbstractPlottable
{
%TypeHeaderCode
#include <QCustomPlot/src/plottables/plottable-graph.h>
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

  explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPGraph();

  // getters:
  // TODO: provide methodcode
  // QSharedPointer<QCPGraphDataContainer> data() const;
  LineStyle lineStyle() const;
  QCPScatterStyle scatterStyle() const;
  int scatterSkip() const;
  QCPGraph *channelFillGraph() const;
  bool adaptiveSampling() const;

  // setters:
  // TODO: provide methodcode
  // void setData(QSharedPointer<QCPGraphDataContainer> data);
  void setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
  void setLineStyle(LineStyle ls);
  void setScatterStyle(const QCPScatterStyle &style);
  void setScatterSkip(int skip);
  void setChannelFillGraph(QCPGraph *targetGraph);
  void setAdaptiveSampling(bool enabled);

  // non-property methods:
  void addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
  void addData(double key, double value);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const;};
