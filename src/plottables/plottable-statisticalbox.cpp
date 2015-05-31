/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

#include "plottable-statisticalbox.h"

#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPStatisticalBox
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPStatisticalBox
  \brief A plottable representing a single statistical box in a plot.

  \image html QCPStatisticalBox.png
  
  To plot data, assign it with the individual parameter functions or use \ref setData to set all
  parameters at once. The individual functions are:
  \li \ref setMinimum
  \li \ref setLowerQuartile
  \li \ref setMedian
  \li \ref setUpperQuartile
  \li \ref setMaximum
  
  Additionally you can define a list of outliers, drawn as scatter datapoints:
  \li \ref setOutliers
  
  \section appearance Changing the appearance
  
  The appearance of the box itself is controlled via \ref setPen and \ref setBrush. You may change
  the width of the box with \ref setWidth in plot coordinates (not pixels).

  Analog functions exist for the minimum/maximum-whiskers: \ref setWhiskerPen, \ref
  setWhiskerBarPen, \ref setWhiskerWidth. The whisker width is the width of the bar at the top
  (maximum) and bottom (minimum).
  
  The median indicator line has its own pen, \ref setMedianPen.
  
  If the whisker backbone pen is changed, make sure to set the capStyle to Qt::FlatCap. Else, the
  backbone line might exceed the whisker bars by a few pixels due to the pen cap being not
  perfectly flat.
  
  The Outlier data points are drawn as normal scatter points. Their look can be controlled with
  \ref setOutlierStyle
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPStatisticalBox is a plottable.
  Usually, you first create an instance and add it to the customPlot:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpstatisticalbox-creation-1
  and then modify the properties of the newly created plottable, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpstatisticalbox-creation-2
*/

/*!
  Constructs a statistical box which uses \a keyAxis as its key axis ("x") and \a valueAxis as its
  value axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and
  not have the same orientation. If either of these restrictions is violated, a corresponding
  message is printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed statistical box can be added to the plot with QCustomPlot::addPlottable,
  QCustomPlot then takes ownership of the statistical box.
*/
QCPStatisticalBox::QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mKey(0),
  mMinimum(0),
  mLowerQuartile(0),
  mMedian(0),
  mUpperQuartile(0),
  mMaximum(0)
{
  setOutlierStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, 6));
  setWhiskerWidth(0.2);
  setWidth(0.5);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2.5));
  setMedianPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
  setWhiskerPen(QPen(Qt::black, 0, Qt::DashLine, Qt::FlatCap));
  setWhiskerBarPen(QPen(Qt::black));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

/*!
  Sets the key coordinate of the statistical box.
*/
void QCPStatisticalBox::setKey(double key)
{
  mKey = key;
}

/*!
  Sets the parameter "minimum" of the statistical box plot. This is the position of the lower
  whisker, typically the minimum measurement of the sample that's not considered an outlier.
  
  \see setMaximum, setWhiskerPen, setWhiskerBarPen, setWhiskerWidth
*/
void QCPStatisticalBox::setMinimum(double value)
{
  mMinimum = value;
}

/*!
  Sets the parameter "lower Quartile" of the statistical box plot. This is the lower end of the
  box. The lower and the upper quartiles are the two statistical quartiles around the median of the
  sample, they contain 50% of the sample data.
  
  \see setUpperQuartile, setPen, setBrush, setWidth
*/
void QCPStatisticalBox::setLowerQuartile(double value)
{
  mLowerQuartile = value;
}

/*!
  Sets the parameter "median" of the statistical box plot. This is the value of the median mark
  inside the quartile box. The median separates the sample data in half (50% of the sample data is
  below/above the median).
  
  \see setMedianPen
*/
void QCPStatisticalBox::setMedian(double value)
{
  mMedian = value;
}

/*!
  Sets the parameter "upper Quartile" of the statistical box plot. This is the upper end of the
  box. The lower and the upper quartiles are the two statistical quartiles around the median of the
  sample, they contain 50% of the sample data.
  
  \see setLowerQuartile, setPen, setBrush, setWidth
*/
void QCPStatisticalBox::setUpperQuartile(double value)
{
  mUpperQuartile = value;
}

/*!
  Sets the parameter "maximum" of the statistical box plot. This is the position of the upper
  whisker, typically the maximum measurement of the sample that's not considered an outlier.
  
  \see setMinimum, setWhiskerPen, setWhiskerBarPen, setWhiskerWidth
*/
void QCPStatisticalBox::setMaximum(double value)
{
  mMaximum = value;
}

/*!
  Sets a vector of outlier values that will be drawn as scatters. Any data points in the sample
  that are not within the whiskers (\ref setMinimum, \ref setMaximum) should be considered outliers
  and displayed as such.
  
  \see setOutlierStyle
*/
void QCPStatisticalBox::setOutliers(const QVector<double> &values)
{
  mOutliers = values;
}

/*!
  Sets all parameters of the statistical box plot at once.
  
  \see setKey, setMinimum, setLowerQuartile, setMedian, setUpperQuartile, setMaximum
*/
void QCPStatisticalBox::setData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum)
{
  setKey(key);
  setMinimum(minimum);
  setLowerQuartile(lowerQuartile);
  setMedian(median);
  setUpperQuartile(upperQuartile);
  setMaximum(maximum);
}

/*!
  Sets the width of the box in key coordinates.
  
  \see setWhiskerWidth
*/
void QCPStatisticalBox::setWidth(double width)
{
  mWidth = width;
}

/*!
  Sets the width of the whiskers (\ref setMinimum, \ref setMaximum) in key coordinates.
  
  \see setWidth
*/
void QCPStatisticalBox::setWhiskerWidth(double width)
{
  mWhiskerWidth = width;
}

/*!
  Sets the pen used for drawing the whisker backbone (That's the line parallel to the value axis).
  
  Make sure to set the \a pen capStyle to Qt::FlatCap to prevent the whisker backbone from reaching
  a few pixels past the whisker bars, when using a non-zero pen width.
  
  \see setWhiskerBarPen
*/
void QCPStatisticalBox::setWhiskerPen(const QPen &pen)
{
  mWhiskerPen = pen;
}

/*!
  Sets the pen used for drawing the whisker bars (Those are the lines parallel to the key axis at
  each end of the whisker backbone).
  
  \see setWhiskerPen
*/
void QCPStatisticalBox::setWhiskerBarPen(const QPen &pen)
{
  mWhiskerBarPen = pen;
}

/*!
  Sets the pen used for drawing the median indicator line inside the statistical box.
*/
void QCPStatisticalBox::setMedianPen(const QPen &pen)
{
  mMedianPen = pen;
}

/*!
  Sets the appearance of the outlier data points.
  
  \see setOutliers
*/
void QCPStatisticalBox::setOutlierStyle(const QCPScatterStyle &style)
{
  mOutlierStyle = style;
}

/* inherits documentation from base class */
void QCPStatisticalBox::clearData()
{
  setOutliers(QVector<double>());
  setKey(0);
  setMinimum(0);
  setLowerQuartile(0);
  setMedian(0);
  setUpperQuartile(0);
  setMaximum(0);
}

/* inherits documentation from base class */
double QCPStatisticalBox::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    double posKey, posValue;
    pixelsToCoords(pos, posKey, posValue);
    // quartile box:
    QCPRange keyRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    QCPRange valueRange(mLowerQuartile, mUpperQuartile);
    if (keyRange.contains(posKey) && valueRange.contains(posValue))
      return mParentPlot->selectionTolerance()*0.99;
    
    // min/max whiskers:
    if (QCPRange(mMinimum, mMaximum).contains(posValue))
      return qAbs(mKeyAxis.data()->coordToPixel(mKey)-mKeyAxis.data()->coordToPixel(posKey));
  }
  return -1;
}

/* inherits documentation from base class */
void QCPStatisticalBox::draw(QCPPainter *painter)
{
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

  // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  if (QCP::isInvalidData(mKey, mMedian) ||
      QCP::isInvalidData(mLowerQuartile, mUpperQuartile) ||
      QCP::isInvalidData(mMinimum, mMaximum))
    qDebug() << Q_FUNC_INFO << "Data point at" << mKey << "of drawn range has invalid data." << "Plottable name:" << name();
  for (int i=0; i<mOutliers.size(); ++i)
    if (QCP::isInvalidData(mOutliers.at(i)))
      qDebug() << Q_FUNC_INFO << "Data point outlier at" << mKey << "of drawn range invalid." << "Plottable name:" << name();
#endif
  
  QRectF quartileBox;
  drawQuartileBox(painter, &quartileBox);
  
  painter->save();
  painter->setClipRect(quartileBox, Qt::IntersectClip);
  drawMedian(painter);
  painter->restore();
  
  drawWhiskers(painter);
  drawOutliers(painter);
}

/* inherits documentation from base class */
void QCPStatisticalBox::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mPen);
  painter->setBrush(mBrush);
  QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*! \internal
  
  Draws the quartile box. \a box is an output parameter that returns the quartile box (in pixel
  coordinates) which is used to set the clip rect of the painter before calling \ref drawMedian (so
  the median doesn't draw outside the quartile box).
*/
void QCPStatisticalBox::drawQuartileBox(QCPPainter *painter, QRectF *quartileBox) const
{
  QRectF box;
  box.setTopLeft(coordsToPixels(mKey-mWidth*0.5, mUpperQuartile));
  box.setBottomRight(coordsToPixels(mKey+mWidth*0.5, mLowerQuartile));
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mainPen());
  painter->setBrush(mainBrush());
  painter->drawRect(box);
  if (quartileBox)
    *quartileBox = box;
}

/*! \internal
  
  Draws the median line inside the quartile box.
*/
void QCPStatisticalBox::drawMedian(QCPPainter *painter) const
{
  QLineF medianLine;
  medianLine.setP1(coordsToPixels(mKey-mWidth*0.5, mMedian));
  medianLine.setP2(coordsToPixels(mKey+mWidth*0.5, mMedian));
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mMedianPen);
  painter->drawLine(medianLine);
}

/*! \internal
  
  Draws both whisker backbones and bars.
*/
void QCPStatisticalBox::drawWhiskers(QCPPainter *painter) const
{
  QLineF backboneMin, backboneMax, barMin, barMax;
  backboneMax.setPoints(coordsToPixels(mKey, mUpperQuartile), coordsToPixels(mKey, mMaximum));
  backboneMin.setPoints(coordsToPixels(mKey, mLowerQuartile), coordsToPixels(mKey, mMinimum));
  barMax.setPoints(coordsToPixels(mKey-mWhiskerWidth*0.5, mMaximum), coordsToPixels(mKey+mWhiskerWidth*0.5, mMaximum));
  barMin.setPoints(coordsToPixels(mKey-mWhiskerWidth*0.5, mMinimum), coordsToPixels(mKey+mWhiskerWidth*0.5, mMinimum));
  applyErrorBarsAntialiasingHint(painter);
  painter->setPen(mWhiskerPen);
  painter->drawLine(backboneMin);
  painter->drawLine(backboneMax);
  painter->setPen(mWhiskerBarPen);
  painter->drawLine(barMin);
  painter->drawLine(barMax);
}

/*! \internal
  
  Draws the outlier scatter points.
*/
void QCPStatisticalBox::drawOutliers(QCPPainter *painter) const
{
  applyScattersAntialiasingHint(painter);
  mOutlierStyle.applyTo(painter, mPen);
  for (int i=0; i<mOutliers.size(); ++i)
    mOutlierStyle.drawShape(painter, coordsToPixels(mKey, mOutliers.at(i)));
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  if (inSignDomain == sdBoth)
  {
    return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
  } else if (inSignDomain == sdNegative)
  {
    if (mKey+mWidth*0.5 < 0)
      return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    else if (mKey < 0)
      return QCPRange(mKey-mWidth*0.5, mKey);
    else
    {
      foundRange = false;
      return QCPRange();
    }
  } else if (inSignDomain == sdPositive)
  {
    if (mKey-mWidth*0.5 > 0)
      return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    else if (mKey > 0)
      return QCPRange(mKey, mKey+mWidth*0.5);
    else
    {
      foundRange = false;
      return QCPRange();
    }
  }
  foundRange = false;
  return QCPRange();
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QVector<double> values; // values that must be considered (i.e. all outliers and the five box-parameters)
  values.reserve(mOutliers.size() + 5);
  values << mMaximum << mUpperQuartile << mMedian << mLowerQuartile << mMinimum;
  values << mOutliers;
  // go through values and find the ones in legal range:
  bool haveUpper = false;
  bool haveLower = false;
  double upper = 0;
  double lower = 0;
  for (int i=0; i<values.size(); ++i)
  {
    if ((inSignDomain == sdNegative && values.at(i) < 0) ||
        (inSignDomain == sdPositive && values.at(i) > 0) ||
        (inSignDomain == sdBoth))
    {
      if (values.at(i) > upper || !haveUpper)
      {
        upper = values.at(i);
        haveUpper = true;
      }
      if (values.at(i) < lower || !haveLower)
      {
        lower = values.at(i);
        haveLower = true;
      }
    }
  }
  // return the bounds if we found some sensible values:
  if (haveLower && haveUpper)
  {
    foundRange = true;
    return QCPRange(lower, upper);
  } else // might happen if all values are in other sign domain
  {
    foundRange = false;
    return QCPRange();
  }
}
