/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPSelectionDecorator /NoDefaultCtors/
{
%TypeHeaderCode
#include <QCustomPlot/src/plottable.h>
%End
public:
  QCPSelectionDecorator();
  virtual ~QCPSelectionDecorator();

  // getters:
  QPen pen() const;
  QBrush brush() const;
  QCPScatterStyle scatterStyle() const;
  QCPScatterStyle::ScatterProperties usedScatterProperties() const;

  // setters:
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setScatterStyle(const QCPScatterStyle &scatterStyle, QCPScatterStyle::ScatterProperties usedProperties=QCPScatterStyle::spPen);
  void setUsedScatterProperties(const QCPScatterStyle::ScatterProperties &properties);

  // non-virtual methods:
  void applyPen(QCPPainter *painter) const;
  void applyBrush(QCPPainter *painter) const;
  QCPScatterStyle getFinalScatterStyle(const QCPScatterStyle &unselectedStyle) const;

  // introduced virtual methods:
  virtual void copyFrom(const QCPSelectionDecorator *other);
  virtual void drawDecoration(QCPPainter *painter, QCPDataSelection selection);
};


class QCPAbstractPlottable : public QCPLayerable /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/plottable.h>
%End
public:
  QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPAbstractPlottable();

  // getters:
  QString name() const;
  bool antialiasedFill() const;
  bool antialiasedScatters() const;
  QPen pen() const;
  QBrush brush() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  QCP::SelectionType selectable() const;
  bool selected() const;
  QCPDataSelection selection() const;
  QCPSelectionDecorator *selectionDecorator() const;

  // setters:
  void setName(const QString &name);
  void setAntialiasedFill(bool enabled);
  void setAntialiasedScatters(bool enabled);
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setKeyAxis(QCPAxis *axis);
  void setValueAxis(QCPAxis *axis);
  Q_SLOT void setSelectable(QCP::SelectionType selectable);
  Q_SLOT void setSelection(QCPDataSelection selection);
  void setSelectionDecorator(QCPSelectionDecorator *decorator);

  // introduced virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  virtual QCPPlottableInterface1D *interface1D();
  virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const = 0;
  virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const = 0;

  // non-property methods:
  const QPointF coordsToPixels(double key, double value) const;
  void pixelsToCoords(double x, double y, double &key, double &value) const;
  void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;
  void rescaleAxes(bool onlyEnlarge=false) const;
  void rescaleKeyAxis(bool onlyEnlarge=false) const;
  void rescaleValueAxis(bool onlyEnlarge=false, bool inKeyRange=false) const;
  bool addToLegend(QCPLegend *legend);
  bool addToLegend();
  bool removeFromLegend(QCPLegend *legend) const;
  bool removeFromLegend() const;

signals:
  void selectionChanged(bool selected);
  void selectionChanged(const QCPDataSelection &selection);
  void selectableChanged(QCP::SelectionType selectable);
};
