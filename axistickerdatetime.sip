/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPAxisTickerDateTime : public QCPAxisTicker
{
%TypeHeaderCode
#include <QCustomPlot/src/axis/axistickerdatetime.h>
%End
public:
  QCPAxisTickerDateTime();

  // getters:
  QString dateTimeFormat() const;
  Qt::TimeSpec dateTimeSpec() const;

  // setters:
  void setDateTimeFormat(const QString &format);
  void setDateTimeSpec(Qt::TimeSpec spec);
  void setTickOrigin(double origin); // hides base class method but calls baseclass implementation ("using" throws off IDEs and doxygen)
  void setTickOrigin(const QDateTime &origin);

  // static methods:
  static QDateTime keyToDateTime(double key);
  static double dateTimeToKey(const QDateTime dateTime);
  static double dateTimeToKey(const QDate date);
};
