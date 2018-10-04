/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPCurveData
{
%TypeHeaderCode
#include <QCustomPlot/src/plottables/plottable-curve.h>
%End
public:
  QCPCurveData();
  QCPCurveData(double t, double key, double value);

  double sortKey() const;
  static QCPCurveData fromSortKey(double sortKey);
  static bool sortKeyIsMainKey();

  double mainKey() const;
  double mainValue() const;

  QCPRange valueRange() const;

  double t;
  double key;
  double value;
};

typedef QCPAbstractPlottable1D<QCPCurveData> QCPAbstractPlottable1D_QCPCurveData;

class QCPCurve : public QCPAbstractPlottable1D_QCPCurveData
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>

typedef QCPAbstractPlottable1D<QCPCurveData> QCPAbstractPlottable1D_QCPCurveData;
%End
public:
  enum LineStyle { lsNone  ///< No line is drawn between data points (e.g. only scatters)
                   ,lsLine ///< Data points are connected with a straight line
                 };

  // Ownership acquired by QCustomPlot tied to the axis
  explicit QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis) /Transfer/;
  virtual ~QCPCurve();

  // getters:
  // TODO: provide methodcode
  // QSharedPointer<QCPCurveDataContainer> data() const { return mDataContainer; }
  QCPScatterStyle scatterStyle() const;
  int scatterSkip() const;
  LineStyle lineStyle() const;

  // setters:
  // TODO: provide methodcode
  // void setData(QSharedPointer<QCPCurveDataContainer> data);
  void setData(const QVector<double> &t, const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
  void setData(const QVector<double> &keys, const QVector<double> &values);
  void setScatterStyle(const QCPScatterStyle &style);
  void setScatterSkip(int skip);
  void setLineStyle(LineStyle style);

  // non-property methods:
  void addData(const QVector<double> &t, const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void addData(double t, double key, double value);
  void addData(double key, double value);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const;
};
