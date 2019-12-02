/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPAxisTickerTime : public QCPAxisTicker
{
%TypeHeaderCode
#include <QCustomPlot/src/axis/axistickertime.h>
%End
public:
  enum TimeUnit { tuMilliseconds ///< Milliseconds, one thousandth of a second (%%z in \ref setTimeFormat)
                  ,tuSeconds     ///< Seconds (%%s in \ref setTimeFormat)
                  ,tuMinutes     ///< Minutes (%%m in \ref setTimeFormat)
                  ,tuHours       ///< Hours (%%h in \ref setTimeFormat)
                  ,tuDays        ///< Days (%%d in \ref setTimeFormat)
                };

  QCPAxisTickerTime();

  // getters:
  QString timeFormat();
  int fieldWidth(TimeUnit unit);

  // setters:
  void setTimeFormat(const QString &format);
  void setFieldWidth(TimeUnit unit, int width);
};
