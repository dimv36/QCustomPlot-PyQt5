/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPPlottableInterface1D /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/plottable1d.h>
%End
public:
  virtual ~QCPPlottableInterface1D();
  // introduced pure virtual methods:
  virtual int dataCount() const = 0;
  virtual double dataMainKey(int index) const = 0;
  virtual double dataSortKey(int index) const = 0;
  virtual double dataMainValue(int index) const = 0;
  virtual QCPRange dataValueRange(int index) const = 0;
  virtual QPointF dataPixelPosition(int index) const = 0;
  virtual bool sortKeyIsMainKey() const = 0;
  virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const = 0;
  virtual int findBegin(double sortKey, bool expandedRange=true) const = 0;
  virtual int findEnd(double sortKey, bool expandedRange=true) const = 0;
};

template <DataType>
class QCPAbstractPlottable1D : public QCPAbstractPlottable, public QCPPlottableInterface1D /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
#include "helper.h"
%End
public:
  QCPAbstractPlottable1D(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPAbstractPlottable1D();
  
  // virtual methods of 1d plottable interface:
  virtual int dataCount() const;
  virtual double dataMainKey(int index) const;
  virtual double dataSortKey(int index) const;
  virtual double dataMainValue(int index) const;
  virtual QCPRange dataValueRange(int index) const;
  virtual QPointF dataPixelPosition(int index) const;
  virtual bool sortKeyIsMainKey() const;
  virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const;
  virtual int findBegin(double sortKey, bool expandedRange=true) const;
  virtual int findEnd(double sortKey, bool expandedRange=true) const;
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  virtual QCPPlottableInterface1D *interface1D();
};
