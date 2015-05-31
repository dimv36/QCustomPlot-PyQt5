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

#include "plottable-financial.h"

#include "../painter.h"
#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancialData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancialData
  \brief Holds the data of one single data point for QCPFinancial.
  
  The container for storing multiple data points is \ref QCPFinancialDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this data point
  \li \a open: The opening value at the data point
  \li \a high: The high/maximum value at the data point
  \li \a low: The low/minimum value at the data point
  \li \a close: The closing value at the data point
  
  \see QCPFinancialDataMap
*/

/*!
  Constructs a data point with key and all values set to zero.
*/
QCPFinancialData::QCPFinancialData() :
  key(0),
  open(0),
  high(0),
  low(0),
  close(0)
{
}

/*!
  Constructs a data point with the specified \a key and OHLC values.
*/
QCPFinancialData::QCPFinancialData(double key, double open, double high, double low, double close) :
  key(key),
  open(open),
  high(high),
  low(low),
  close(close)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancial
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancial
  \brief A plottable representing a financial stock chart

  \image html QCPFinancial.png
  
  This plottable represents time series data binned to certain intervals, mainly used for stock
  charts. The two common representations OHLC (Open-High-Low-Close) bars and Candlesticks can be
  set via \ref setChartStyle.
  
  The data is passed via \ref setData as a set of open/high/low/close values at certain keys
  (typically times). This means the data must be already binned appropriately. If data is only
  available as a series of values (e.g. \a price against \a time), you can use the static
  convenience function \ref timeSeriesToOhlc to generate binned OHLC-data which can then be passed
  to \ref setData.
  
  The width of the OHLC bars/candlesticks can be controlled with \ref setWidth and is given in plot
  key coordinates. A typical choice is to set it to (or slightly less than) one bin interval width.
  
  \section appearance Changing the appearance
  
  Charts can be either single- or two-colored (\ref setTwoColored). If set to be single-colored,
  lines are drawn with the plottable's pen (\ref setPen) and fills with the brush (\ref setBrush).
  
  If set to two-colored, positive changes of the value during an interval (\a close >= \a open) are
  represented with a different pen and brush than negative changes (\a close < \a open). These can
  be configured with \ref setPenPositive, \ref setPenNegative, \ref setBrushPositive, and \ref
  setBrushNegative. In two-colored mode, the normal plottable pen/brush is ignored. Upon selection
  however, the normal selected pen/brush (\ref setSelectedPen, \ref setSelectedBrush) is used,
  irrespective of whether the chart is single- or two-colored.
  
*/

/* start of documentation of inline functions */

/*! \fn QCPFinancialDataMap *QCPFinancial::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPFinancialDataMap. You may use it to
  directly manipulate the data, which may be more convenient and faster than using the regular \ref
  setData or \ref addData methods, in certain situations.
*/

/* end of documentation of inline functions */

/*!
  Constructs a financial chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPFinancial can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the financial chart.
*/
QCPFinancial::QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mData(0),
  mChartStyle(csOhlc),
  mWidth(0.5),
  mTwoColored(false),
  mBrushPositive(QBrush(QColor(210, 210, 255))),
  mBrushNegative(QBrush(QColor(255, 210, 210))),
  mPenPositive(QPen(QColor(10, 40, 180))),
  mPenNegative(QPen(QColor(180, 40, 10)))
{
  mData = new QCPFinancialDataMap;
  
  setSelectedPen(QPen(QColor(80, 80, 255), 2.5));
  setSelectedBrush(QBrush(QColor(80, 80, 255)));
}

QCPFinancial::~QCPFinancial()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
  
  Alternatively, you can also access and modify the plottable's data via the \ref data method, which
  returns a pointer to the internal \ref QCPFinancialDataMap.
  
  \see timeSeriesToOhlc
*/
void QCPFinancial::setData(QCPFinancialDataMap *data, bool copy)
{
  if (mData == data)
  {
    qDebug() << Q_FUNC_INFO << "The data pointer is already in (and owned by) this plottable" << reinterpret_cast<quintptr>(data);
    return;
  }
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided open/high/low/close data. The provided vectors should
  have equal length. Else, the number of added points will be the size of the smallest vector.
  
  \see timeSeriesToOhlc
*/
void QCPFinancial::setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, open.size());
  n = qMin(n, high.size());
  n = qMin(n, low.size());
  n = qMin(n, close.size());
  for (int i=0; i<n; ++i)
  {
    mData->insertMulti(key[i], QCPFinancialData(key[i], open[i], high[i], low[i], close[i]));
  }
}

/*!
  Sets which representation style shall be used to display the OHLC data.
*/
void QCPFinancial::setChartStyle(QCPFinancial::ChartStyle style)
{
  mChartStyle = style;
}

/*!
  Sets the width of the individual bars/candlesticks to \a width in plot key coordinates.
  
  A typical choice is to set it to (or slightly less than) one bin interval width.
*/
void QCPFinancial::setWidth(double width)
{
  mWidth = width;
}

/*!
  Sets whether this chart shall contrast positive from negative trends per data point by using two
  separate colors to draw the respective bars/candlesticks.
  
  If \a twoColored is false, the normal plottable's pen and brush are used (\ref setPen, \ref
  setBrush).
  
  \see setPenPositive, setPenNegative, setBrushPositive, setBrushNegative
*/
void QCPFinancial::setTwoColored(bool twoColored)
{
  mTwoColored = twoColored;
}

/*!
  If \ref setTwoColored is set to true, this function controls the brush that is used to draw fills
  of data points with a positive trend (i.e. bars/candlesticks with close >= open).
  
  If \a twoColored is false, the normal plottable's pen and brush are used (\ref setPen, \ref
  setBrush).
  
  \see setBrushNegative, setPenPositive, setPenNegative
*/
void QCPFinancial::setBrushPositive(const QBrush &brush)
{
  mBrushPositive = brush;
}

/*!
  If \ref setTwoColored is set to true, this function controls the brush that is used to draw fills
  of data points with a negative trend (i.e. bars/candlesticks with close < open).
  
  If \a twoColored is false, the normal plottable's pen and brush are used (\ref setPen, \ref
  setBrush).
  
  \see setBrushPositive, setPenNegative, setPenPositive
*/
void QCPFinancial::setBrushNegative(const QBrush &brush)
{
  mBrushNegative = brush;
}

/*!
  If \ref setTwoColored is set to true, this function controls the pen that is used to draw
  outlines of data points with a positive trend (i.e. bars/candlesticks with close >= open).
  
  If \a twoColored is false, the normal plottable's pen and brush are used (\ref setPen, \ref
  setBrush).
  
  \see setPenNegative, setBrushPositive, setBrushNegative
*/
void QCPFinancial::setPenPositive(const QPen &pen)
{
  mPenPositive = pen;
}

/*!
  If \ref setTwoColored is set to true, this function controls the pen that is used to draw
  outlines of data points with a negative trend (i.e. bars/candlesticks with close < open).
  
  If \a twoColored is false, the normal plottable's pen and brush are used (\ref setPen, \ref
  setBrush).
  
  \see setPenPositive, setBrushNegative, setBrushPositive
*/
void QCPFinancial::setPenNegative(const QPen &pen)
{
  mPenNegative = pen;
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialDataMap.
  
  \see removeData
*/
void QCPFinancial::addData(const QCPFinancialDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  
  Adds the provided single data point in \a data to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(const QCPFinancialData &data)
{
  mData->insertMulti(data.key, data);
}

/*! \overload
  
  Adds the provided single data point given by \a key, \a open, \a high, \a low, and \a close to
  the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(double key, double open, double high, double low, double close)
{
  mData->insertMulti(key, QCPFinancialData(key, open, high, low, close));
}

/*! \overload
  
  Adds the provided open/high/low/close data to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close)
{
  int n = key.size();
  n = qMin(n, open.size());
  n = qMin(n, high.size());
  n = qMin(n, low.size());
  n = qMin(n, close.size());
  for (int i=0; i<n; ++i)
  {
    mData->insertMulti(key[i], QCPFinancialData(key[i], open[i], high[i], low[i], close[i]));
  }
}

/*!
  Removes all data points with keys smaller than \a key.
  
  \see addData, clearData
*/
void QCPFinancial::removeDataBefore(double key)
{
  QCPFinancialDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < key)
    it = mData->erase(it);
}

/*!
  Removes all data points with keys greater than \a key.
  
  \see addData, clearData
*/
void QCPFinancial::removeDataAfter(double key)
{
  if (mData->isEmpty()) return;
  QCPFinancialDataMap::iterator it = mData->upperBound(key);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with keys between \a fromKey and \a toKey. if \a fromKey is greater or
  equal to \a toKey, the function does nothing. To remove a single data point with known key, use
  \ref removeData(double key).
  
  \see addData, clearData
*/
void QCPFinancial::removeData(double fromKey, double toKey)
{
  if (fromKey >= toKey || mData->isEmpty()) return;
  QCPFinancialDataMap::iterator it = mData->upperBound(fromKey);
  QCPFinancialDataMap::iterator itEnd = mData->upperBound(toKey);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at \a key. If the position is not known with absolute precision,
  consider using \ref removeData(double fromKey, double toKey) with a small fuzziness interval
  around the suspected position, depeding on the precision with which the key is known.

  \see addData, clearData
*/
void QCPFinancial::removeData(double key)
{
  mData->remove(key);
}

/*!
  Removes all data points.
  
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPFinancial::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPFinancial::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    // get visible data range:
    QCPFinancialDataMap::const_iterator lower, upper; // note that upper is the actual upper point, and not 1 step after the upper point
    getVisibleDataBounds(lower, upper);
    if (lower == mData->constEnd() || upper == mData->constEnd())
      return -1;
    // perform select test according to configured style:
    switch (mChartStyle)
    {
      case QCPFinancial::csOhlc:
        return ohlcSelectTest(pos, lower, upper+1); break;
      case QCPFinancial::csCandlestick:
        return candlestickSelectTest(pos, lower, upper+1); break;
    }
  }
  return -1;
}

/*!
  A convenience function that converts time series data (\a value against \a time) to OHLC binned
  data points. The return value can then be passed on to \ref setData.
  
  The size of the bins can be controlled with \a timeBinSize in the same units as \a time is given.
  For example, if the unit of \a time is seconds and single OHLC/Candlesticks should span an hour
  each, set \a timeBinSize to 3600.
  
  \a timeBinOffset allows to control precisely at what \a time coordinate a bin should start. The
  value passed as \a timeBinOffset doesn't need to be in the range encompassed by the \a time keys.
  It merely defines the mathematical offset/phase of the bins that will be used to process the
  data.
*/
QCPFinancialDataMap QCPFinancial::timeSeriesToOhlc(const QVector<double> &time, const QVector<double> &value, double timeBinSize, double timeBinOffset)
{
  QCPFinancialDataMap map;
  int count = qMin(time.size(), value.size());
  if (count == 0)
    return QCPFinancialDataMap();
  
  QCPFinancialData currentBinData(0, value.first(), value.first(), value.first(), value.first());
  int currentBinIndex = qFloor((time.first()-timeBinOffset)/timeBinSize+0.5);
  for (int i=0; i<count; ++i)
  {
    int index = qFloor((time.at(i)-timeBinOffset)/timeBinSize+0.5);
    if (currentBinIndex == index) // data point still in current bin, extend high/low:
    {
      if (value.at(i) < currentBinData.low) currentBinData.low = value.at(i);
      if (value.at(i) > currentBinData.high) currentBinData.high = value.at(i);
      if (i == count-1) // last data point is in current bin, finalize bin:
      {
        currentBinData.close = value.at(i);
        currentBinData.key = timeBinOffset+(index)*timeBinSize;
        map.insert(currentBinData.key, currentBinData);
      }
    } else // data point not anymore in current bin, set close of old and open of new bin, and add old to map:
    {
      // finalize current bin:
      currentBinData.close = value.at(i-1);
      currentBinData.key = timeBinOffset+(index-1)*timeBinSize;
      map.insert(currentBinData.key, currentBinData);
      // start next bin:
      currentBinIndex = index;
      currentBinData.open = value.at(i);
      currentBinData.high = value.at(i);
      currentBinData.low = value.at(i);
    }
  }
  
  return map;
}

/* inherits documentation from base class */
void QCPFinancial::draw(QCPPainter *painter)
{
  // get visible data range:
  QCPFinancialDataMap::const_iterator lower, upper; // note that upper is the actual upper point, and not 1 step after the upper point
  getVisibleDataBounds(lower, upper);
  if (lower == mData->constEnd() || upper == mData->constEnd())
    return;
  
  // draw visible data range according to configured style:
  switch (mChartStyle)
  {
    case QCPFinancial::csOhlc:
      drawOhlcPlot(painter, lower, upper+1); break;
    case QCPFinancial::csCandlestick:
      drawCandlestickPlot(painter, lower, upper+1); break;
  }
}

/* inherits documentation from base class */
void QCPFinancial::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  painter->setAntialiasing(false); // legend icon especially of csCandlestick looks better without antialiasing
  if (mChartStyle == csOhlc)
  {
    if (mTwoColored)
    {
      // draw upper left half icon with positive color:
      painter->setBrush(mBrushPositive);
      painter->setPen(mPenPositive);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint() << rect.topLeft().toPoint()));
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.2, rect.height()*0.3, rect.width()*0.2, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.8, rect.height()*0.5, rect.width()*0.8, rect.height()*0.7).translated(rect.topLeft()));
      // draw bottom right hald icon with negative color:
      painter->setBrush(mBrushNegative);
      painter->setPen(mPenNegative);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint() << rect.bottomRight().toPoint()));
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.2, rect.height()*0.3, rect.width()*0.2, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.8, rect.height()*0.5, rect.width()*0.8, rect.height()*0.7).translated(rect.topLeft()));
    } else
    {
      painter->setBrush(mBrush);
      painter->setPen(mPen);
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.2, rect.height()*0.3, rect.width()*0.2, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.8, rect.height()*0.5, rect.width()*0.8, rect.height()*0.7).translated(rect.topLeft()));
    }
  } else if (mChartStyle == csCandlestick)
  {
    if (mTwoColored)
    {
      // draw upper left half icon with positive color:
      painter->setBrush(mBrushPositive);
      painter->setPen(mPenPositive);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint() << rect.topLeft().toPoint()));
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width()*0.25, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.75, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width()*0.25, rect.height()*0.25, rect.width()*0.5, rect.height()*0.5).translated(rect.topLeft()));
      // draw bottom right hald icon with negative color:
      painter->setBrush(mBrushNegative);
      painter->setPen(mPenNegative);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint() << rect.bottomRight().toPoint()));
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width()*0.25, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.75, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width()*0.25, rect.height()*0.25, rect.width()*0.5, rect.height()*0.5).translated(rect.topLeft()));
    } else
    {
      painter->setBrush(mBrush);
      painter->setPen(mPen);
      painter->drawLine(QLineF(0, rect.height()*0.5, rect.width()*0.25, rect.height()*0.5).translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width()*0.75, rect.height()*0.5, rect.width(), rect.height()*0.5).translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width()*0.25, rect.height()*0.25, rect.width()*0.5, rect.height()*0.5).translated(rect.topLeft()));
    }
  }
}

/* inherits documentation from base class */
QCPRange QCPFinancial::getKeyRange(bool &foundRange, QCPAbstractPlottable::SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  QCPFinancialDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().key;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  // determine exact range by including width of bars/flags:
  if (haveLower && mKeyAxis)
    range.lower = range.lower-mWidth*0.5;
  if (haveUpper && mKeyAxis)
    range.upper = range.upper+mWidth*0.5;
  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPFinancial::getValueRange(bool &foundRange, QCPAbstractPlottable::SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  QCPFinancialDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    // high:
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && it.value().high < 0) || (inSignDomain == sdPositive && it.value().high > 0))
    {
      if (it.value().high < range.lower || !haveLower)
      {
        range.lower = it.value().high;
        haveLower = true;
      }
      if (it.value().high > range.upper || !haveUpper)
      {
        range.upper = it.value().high;
        haveUpper = true;
      }
    }
    // low:
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && it.value().low < 0) || (inSignDomain == sdPositive && it.value().low > 0))
    {
      if (it.value().low < range.lower || !haveLower)
      {
        range.lower = it.value().low;
        haveLower = true;
      }
      if (it.value().low > range.upper || !haveUpper)
      {
        range.upper = it.value().low;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}

/*! \internal
  
  Draws the data from \a begin to \a end as OHLC bars with the provided \a painter.

  This method is a helper function for \ref draw. It is used when the chart style is \ref csOhlc.
*/
void QCPFinancial::drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end)
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  QPen linePen;
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
        linePen = mSelectedPen;
      else if (mTwoColored)
        linePen = it.value().close >= it.value().open ? mPenPositive : mPenNegative;
      else
        linePen = mPen;
      painter->setPen(linePen);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw backbone:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().high)), QPointF(keyPixel, valueAxis->coordToPixel(it.value().low)));
      // draw open:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5); // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(keyPixel-keyWidthPixels, openPixel), QPointF(keyPixel, openPixel));
      // draw close:
      painter->drawLine(QPointF(keyPixel, closePixel), QPointF(keyPixel+keyWidthPixels, closePixel));
    }
  } else
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
        linePen = mSelectedPen;
      else if (mTwoColored)
        linePen = it.value().close >= it.value().open ? mPenPositive : mPenNegative;
      else
        linePen = mPen;
      painter->setPen(linePen);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw backbone:
      painter->drawLine(QPointF(valueAxis->coordToPixel(it.value().high), keyPixel), QPointF(valueAxis->coordToPixel(it.value().low), keyPixel));
      // draw open:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5); // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(openPixel, keyPixel-keyWidthPixels), QPointF(openPixel, keyPixel));
      // draw close:
      painter->drawLine(QPointF(closePixel, keyPixel), QPointF(closePixel, keyPixel+keyWidthPixels));
    }
  }
}

/*! \internal
  
  Draws the data from \a begin to \a end as Candlesticks with the provided \a painter.

  This method is a helper function for \ref draw. It is used when the chart style is \ref csCandlestick.
*/
void QCPFinancial::drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end)
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  QPen linePen;
  QBrush boxBrush;
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
      {
        linePen = mSelectedPen;
        boxBrush = mSelectedBrush;
      } else if (mTwoColored)
      {
        if (it.value().close >= it.value().open)
        {
          linePen = mPenPositive;
          boxBrush = mBrushPositive;
        } else
        {
          linePen = mPenNegative;
          boxBrush = mBrushNegative;
        }
      } else
      {
        linePen = mPen;
        boxBrush = mBrush;
      }
      painter->setPen(linePen);
      painter->setBrush(boxBrush);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw high:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().high)), QPointF(keyPixel, valueAxis->coordToPixel(qMax(it.value().open, it.value().close))));
      // draw low:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().low)), QPointF(keyPixel, valueAxis->coordToPixel(qMin(it.value().open, it.value().close))));
      // draw open-close box:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5);
      painter->drawRect(QRectF(QPointF(keyPixel-keyWidthPixels, closePixel), QPointF(keyPixel+keyWidthPixels, openPixel)));
    }
  } else // keyAxis->orientation() == Qt::Vertical
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
      {
        linePen = mSelectedPen;
        boxBrush = mSelectedBrush;
      } else if (mTwoColored)
      {
        if (it.value().close >= it.value().open)
        {
          linePen = mPenPositive;
          boxBrush = mBrushPositive;
        } else
        {
          linePen = mPenNegative;
          boxBrush = mBrushNegative;
        }
      } else
      {
        linePen = mPen;
        boxBrush = mBrush;
      }
      painter->setPen(linePen);
      painter->setBrush(boxBrush);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw high:
      painter->drawLine(QPointF(valueAxis->coordToPixel(it.value().high), keyPixel), QPointF(valueAxis->coordToPixel(qMax(it.value().open, it.value().close)), keyPixel));
      // draw low:
      painter->drawLine(QPointF(valueAxis->coordToPixel(it.value().low), keyPixel), QPointF(valueAxis->coordToPixel(qMin(it.value().open, it.value().close)), keyPixel));
      // draw open-close box:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5);
      painter->drawRect(QRectF(QPointF(closePixel, keyPixel-keyWidthPixels), QPointF(openPixel, keyPixel+keyWidthPixels)));
    }
  }
}

/*! \internal
  
  This method is a helper function for \ref selectTest. It is used to test for selection when the
  chart style is \ref csOhlc. It only tests against the data points between \a begin and \a end.
*/
double QCPFinancial::ohlcSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }

  double minDistSqr = std::numeric_limits<double>::max();
  QCPFinancialDataMap::const_iterator it;
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    for (it = begin; it != end; ++it)
    {
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      // calculate distance to backbone:
      double currentDistSqr = distSqrToLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().high)), QPointF(keyPixel, valueAxis->coordToPixel(it.value().low)), pos);
      if (currentDistSqr < minDistSqr)
        minDistSqr = currentDistSqr;
    }
  } else // keyAxis->orientation() == Qt::Vertical
  {
    for (it = begin; it != end; ++it)
    {
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      // calculate distance to backbone:
      double currentDistSqr = distSqrToLine(QPointF(valueAxis->coordToPixel(it.value().high), keyPixel), QPointF(valueAxis->coordToPixel(it.value().low), keyPixel), pos);
      if (currentDistSqr < minDistSqr)
        minDistSqr = currentDistSqr;
    }
  }
  return qSqrt(minDistSqr);
}

/*! \internal
  
  This method is a helper function for \ref selectTest. It is used to test for selection when the
  chart style is \ref csCandlestick. It only tests against the data points between \a begin and \a
  end.
*/
double QCPFinancial::candlestickSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }

  double minDistSqr = std::numeric_limits<double>::max();
  QCPFinancialDataMap::const_iterator it;
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    for (it = begin; it != end; ++it)
    {
      double currentDistSqr;
      // determine whether pos is in open-close-box:
      QCPRange boxKeyRange(it.value().key-mWidth*0.5, it.value().key+mWidth*0.5);
      QCPRange boxValueRange(it.value().close, it.value().open);
      double posKey, posValue;
      pixelsToCoords(pos, posKey, posValue);
      if (boxKeyRange.contains(posKey) && boxValueRange.contains(posValue)) // is in open-close-box
      {
        currentDistSqr = mParentPlot->selectionTolerance()*0.99 * mParentPlot->selectionTolerance()*0.99;
      } else
      {
        // calculate distance to high/low lines:
        double keyPixel = keyAxis->coordToPixel(it.value().key);
        double highLineDistSqr = distSqrToLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().high)), QPointF(keyPixel, valueAxis->coordToPixel(qMax(it.value().open, it.value().close))), pos);
        double lowLineDistSqr = distSqrToLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().low)), QPointF(keyPixel, valueAxis->coordToPixel(qMin(it.value().open, it.value().close))), pos);
        currentDistSqr = qMin(highLineDistSqr, lowLineDistSqr);
      }
      if (currentDistSqr < minDistSqr)
        minDistSqr = currentDistSqr;
    }
  } else // keyAxis->orientation() == Qt::Vertical
  {
    for (it = begin; it != end; ++it)
    {
      double currentDistSqr;
      // determine whether pos is in open-close-box:
      QCPRange boxKeyRange(it.value().key-mWidth*0.5, it.value().key+mWidth*0.5);
      QCPRange boxValueRange(it.value().close, it.value().open);
      double posKey, posValue;
      pixelsToCoords(pos, posKey, posValue);
      if (boxKeyRange.contains(posKey) && boxValueRange.contains(posValue)) // is in open-close-box
      {
        currentDistSqr = mParentPlot->selectionTolerance()*0.99 * mParentPlot->selectionTolerance()*0.99;
      } else
      {
        // calculate distance to high/low lines:
        double keyPixel = keyAxis->coordToPixel(it.value().key);
        double highLineDistSqr = distSqrToLine(QPointF(valueAxis->coordToPixel(it.value().high), keyPixel), QPointF(valueAxis->coordToPixel(qMax(it.value().open, it.value().close)), keyPixel), pos);
        double lowLineDistSqr = distSqrToLine(QPointF(valueAxis->coordToPixel(it.value().low), keyPixel), QPointF(valueAxis->coordToPixel(qMin(it.value().open, it.value().close)), keyPixel), pos);
        currentDistSqr = qMin(highLineDistSqr, lowLineDistSqr);
      }
      if (currentDistSqr < minDistSqr)
        minDistSqr = currentDistSqr;
    }
  }
  return qSqrt(minDistSqr);
}

/*!  \internal
  
  called by the drawing methods to determine which data (key) range is visible at the current key
  axis range setting, so only that needs to be processed.
  
  \a lower returns an iterator to the lowest data point that needs to be taken into account when
  plotting. Note that in order to get a clean plot all the way to the edge of the axis rect, \a
  lower may still be just outside the visible range.
  
  \a upper returns an iterator to the highest data point. Same as before, \a upper may also lie
  just outside of the visible range.
  
  if the plottable contains no data, both \a lower and \a upper point to constEnd.
  
  \see QCPGraph::getVisibleDataBounds
*/
void QCPFinancial::getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const
{
  if (!mKeyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  if (mData->isEmpty())
  {
    lower = mData->constEnd();
    upper = mData->constEnd();
    return;
  }
  
  // get visible data range as QMap iterators
  QCPFinancialDataMap::const_iterator lbound = mData->lowerBound(mKeyAxis.data()->range().lower);
  QCPFinancialDataMap::const_iterator ubound = mData->upperBound(mKeyAxis.data()->range().upper);
  bool lowoutlier = lbound != mData->constBegin(); // indicates whether there exist points below axis range
  bool highoutlier = ubound != mData->constEnd(); // indicates whether there exist points above axis range
  
  lower = (lowoutlier ? lbound-1 : lbound); // data point range that will be actually drawn
  upper = (highoutlier ? ubound : ubound-1); // data point range that will be actually drawn
}
