/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

%Include qmap_double.sip

class QCPAxisTickerText : public QCPAxisTicker
{
%TypeHeaderCode
#include <QCustomPlot/src/axis/axistickertext.h>
%End
public:
  QCPAxisTickerText();

  // getters:
  // This DOES NOT return a reference as it does in C++ version
  // Cannot mark as const as the base C++ method is not
  QMap<double, QString> ticks();
  int subTickCount() const;

  // setters:
  void setTicks(const QMap<double, QString> &ticks);
  void setTicks(const QVector<double> &positions, const QVector<QString> &labels);
  void setSubTickCount(int subTicks);

  // non-virtual methods:
  void clear();
  void addTick(double position, const QString &label);
  void addTicks(const QMap<double, QString> &ticks);
  void addTicks(const QVector<double> &positions, const QVector<QString> &labels);
};
