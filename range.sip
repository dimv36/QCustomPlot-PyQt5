/** PyQt5 binding for QCustomPlot v2.0.0
 *  
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *  
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPRange
{
%TypeHeaderCode
#include <QCustomPlot/src/axis/range.h>
%End

public:
  double lower;
  double upper;
  
  QCPRange();
  QCPRange(double lower, double upper);
  
  bool operator==(const QCPRange& other) const;
  bool operator!=(const QCPRange& other) const;
  
  QCPRange &operator+=(const double& value);
  QCPRange &operator-=(const double& value);
  QCPRange &operator*=(const double& value);
  QCPRange &operator/=(const double& value);
  
  double size() const;
  double center() const;
  void normalize();
  void expand(const QCPRange &otherRange);
  void expand(double includeCoord);
  QCPRange expanded(const QCPRange &otherRange) const;
  QCPRange expanded(double includeCoord) const;
  QCPRange bounded(double lowerBound, double upperBound) const;
  QCPRange sanitizedForLogScale() const;
  QCPRange sanitizedForLinScale() const;
  bool contains(double value) const;
  
  static bool validRange(double lower, double upper);
  static bool validRange(const QCPRange &range);
  static const double minRange;
  static const double maxRange;
};
