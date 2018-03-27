/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPVector2D
{
%TypeHeaderCode
#include <QCustomPlot/src/vector2d.h>
%End
public:
  QCPVector2D();
  QCPVector2D(double x, double y);
  QCPVector2D(const QPoint &point);
  QCPVector2D(const QPointF &point);

  // getters:
  double x() const;
  double y() const;

  // setters:
  void setX(double x);
  void setY(double y);

  // non-virtual methods:
  double length() const;
  double lengthSquared() const;
  QPoint toPoint() const;
  QPointF toPointF() const;

  bool isNull() const;
  void normalize();
  QCPVector2D normalized() const;
  QCPVector2D perpendicular() const;
  double dot(const QCPVector2D &vec) const;
  double distanceSquaredToLine(const QCPVector2D &start, const QCPVector2D &end) const;
  double distanceSquaredToLine(const QLineF &line) const;
  double distanceToStraightLine(const QCPVector2D &base, const QCPVector2D &direction) const;

  QCPVector2D &operator*=(double factor);
  QCPVector2D &operator/=(double divisor);
  QCPVector2D &operator+=(const QCPVector2D &vector);
  QCPVector2D &operator-=(const QCPVector2D &vector);
};
