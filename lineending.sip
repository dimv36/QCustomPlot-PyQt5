/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPLineEnding
{
%TypeHeaderCode
#include <QCustomPlot/src/lineending.h>
%End
public:
  enum EndingStyle { esNone          ///< No ending decoration
                     ,esFlatArrow    ///< A filled arrow head with a straight/flat back (a triangle)
                     ,esSpikeArrow   ///< A filled arrow head with an indented back
                     ,esLineArrow    ///< A non-filled arrow head with open back
                     ,esDisc         ///< A filled circle
                     ,esSquare       ///< A filled square
                     ,esDiamond      ///< A filled diamond (45 degrees rotated square)
                     ,esBar          ///< A bar perpendicular to the line
                     ,esHalfBar      ///< A bar perpendicular to the line, pointing out to only one side (to which side can be changed with \ref setInverted)
                     ,esSkewedBar    ///< A bar that is skewed (skew controllable via \ref setLength)
                   };

  QCPLineEnding();
  QCPLineEnding(EndingStyle style, double width=8, double length=10, bool inverted=false);

  // getters:
  EndingStyle style() const;
  double width() const;
  double length() const;
  bool inverted() const;

  // setters:
  void setStyle(EndingStyle style);
  void setWidth(double width);
  void setLength(double length);
  void setInverted(bool inverted);

  // non-property methods:
  double boundingDistance() const;
  double realLength() const;
  void draw(QCPPainter *painter, const QCPVector2D &pos, const QCPVector2D &dir) const;
  void draw(QCPPainter *painter, const QCPVector2D &pos, double angle) const;
};
