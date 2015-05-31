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

#include "plottable-bars.h"

#include "../painter.h"
#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBarsGroup
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBarsGroup
  \brief Groups multiple QCPBars together so they appear side by side
  
  \image html QCPBarsGroup.png
  
  When showing multiple QCPBars in one plot which have bars at identical keys, it may be desirable
  to have them appearing next to each other at each key. This is what adding the respective QCPBars
  plottables to a QCPBarsGroup achieves. (An alternative approach is to stack them on top of each
  other, see \ref QCPBars::moveAbove.)
  
  \section qcpbarsgroup-usage Usage
  
  To add a QCPBars plottable to the group, create a new group and then add the respective bars
  intances:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbarsgroup-creation
  Alternatively to appending to the group like shown above, you can also set the group on the
  QCPBars plottable via \ref QCPBars::setBarsGroup.
  
  The spacing between the bars can be configured via \ref setSpacingType and \ref setSpacing. The
  bars in this group appear in the plot in the order they were appended. To insert a bars plottable
  at a certain index position, or to reposition a bars plottable which is already in the group, use
  \ref insert.
  
  To remove specific bars from the group, use either \ref remove or call \ref
  QCPBars::setBarsGroup "QCPBars::setBarsGroup(0)" on the respective bars plottable.
  
  To clear the entire group, call \ref clear, or simply delete the group.
  
  \section qcpbarsgroup-example Example
  
  The image above is generated with the following code:
  \snippet documentation/doc-image-generator/mainwindow.cpp qcpbarsgroup-example
*/

/* start of documentation of inline functions */

/*! \fn QList<QCPBars*> QCPBarsGroup::bars() const
  
  Returns all bars currently in this group.
  
  \see bars(int index)
*/

/*! \fn int QCPBarsGroup::size() const
  
  Returns the number of QCPBars plottables that are part of this group.
  
*/

/*! \fn bool QCPBarsGroup::isEmpty() const
  
  Returns whether this bars group is empty.
  
  \see size
*/

/*! \fn bool QCPBarsGroup::contains(QCPBars *bars)
  
  Returns whether the specified \a bars plottable is part of this group.
  
*/

/* end of documentation of inline functions */

/*!
  Constructs a new bars group for the specified QCustomPlot instance.
*/
QCPBarsGroup::QCPBarsGroup(QCustomPlot *parentPlot) :
  QObject(parentPlot),
  mParentPlot(parentPlot),
  mSpacingType(stAbsolute),
  mSpacing(4)
{
}

QCPBarsGroup::~QCPBarsGroup()
{
  clear();
}

/*!
  Sets how the spacing between adjacent bars is interpreted. See \ref SpacingType.
  
  The actual spacing can then be specified with \ref setSpacing.

  \see setSpacing
*/
void QCPBarsGroup::setSpacingType(SpacingType spacingType)
{
  mSpacingType = spacingType;
}

/*!
  Sets the spacing between adjacent bars. What the number passed as \a spacing actually means, is
  defined by the current \ref SpacingType, which can be set with \ref setSpacingType.

  \see setSpacingType
*/
void QCPBarsGroup::setSpacing(double spacing)
{
  mSpacing = spacing;
}

/*!
  Returns the QCPBars instance with the specified \a index in this group. If no such QCPBars
  exists, returns 0.

  \see bars(), size
*/
QCPBars *QCPBarsGroup::bars(int index) const
{
  if (index >= 0 && index < mBars.size())
  {
    return mBars.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*!
  Removes all QCPBars plottables from this group.

  \see isEmpty
*/
void QCPBarsGroup::clear()
{
  foreach (QCPBars *bars, mBars) // since foreach takes a copy, removing bars in the loop is okay
    bars->setBarsGroup(0); // removes itself via removeBars
}

/*!
  Adds the specified \a bars plottable to this group. Alternatively, you can also use \ref
  QCPBars::setBarsGroup on the \a bars instance.

  \see insert, remove
*/
void QCPBarsGroup::append(QCPBars *bars)
{
  if (!bars)
  {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }
    
  if (!mBars.contains(bars))
    bars->setBarsGroup(this);
  else
    qDebug() << Q_FUNC_INFO << "bars plottable is already in this bars group:" << reinterpret_cast<quintptr>(bars);
}

/*!
  Inserts the specified \a bars plottable into this group at the specified index position \a i.
  This gives you full control over the ordering of the bars.
  
  \a bars may already be part of this group. In that case, \a bars is just moved to the new index
  position.

  \see append, remove
*/
void QCPBarsGroup::insert(int i, QCPBars *bars)
{
  if (!bars)
  {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }
  
  // first append to bars list normally:
  if (!mBars.contains(bars))
    bars->setBarsGroup(this);
  // then move to according position:
  mBars.move(mBars.indexOf(bars), qBound(0, i, mBars.size()-1));
}

/*!
  Removes the specified \a bars plottable from this group.
  
  \see contains, clear
*/
void QCPBarsGroup::remove(QCPBars *bars)
{
  if (!bars)
  {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }
  
  if (mBars.contains(bars))
    bars->setBarsGroup(0);
  else
    qDebug() << Q_FUNC_INFO << "bars plottable is not in this bars group:" << reinterpret_cast<quintptr>(bars);
}

/*! \internal
  
  Adds the specified \a bars to the internal mBars list of bars. This method does not change the
  barsGroup property on \a bars.
  
  \see unregisterBars
*/
void QCPBarsGroup::registerBars(QCPBars *bars)
{
  if (!mBars.contains(bars))
    mBars.append(bars);
}

/*! \internal
  
  Removes the specified \a bars from the internal mBars list of bars. This method does not change
  the barsGroup property on \a bars.
  
  \see registerBars
*/
void QCPBarsGroup::unregisterBars(QCPBars *bars)
{
  mBars.removeOne(bars);
}

/*! \internal
  
  Returns the pixel offset in the key dimension the specified \a bars plottable should have at the
  given key coordinate \a keyCoord. The offset is relative to the pixel position of the key
  coordinate \a keyCoord.
*/
double QCPBarsGroup::keyPixelOffset(const QCPBars *bars, double keyCoord)
{
  // find list of all base bars in case some mBars are stacked:
  QList<const QCPBars*> baseBars;
  foreach (const QCPBars *b, mBars)
  {
    while (b->barBelow())
      b = b->barBelow();
    if (!baseBars.contains(b))
      baseBars.append(b);
  }
  // find base bar this "bars" is stacked on:
  const QCPBars *thisBase = bars;
  while (thisBase->barBelow())
    thisBase = thisBase->barBelow();
  
  // determine key pixel offset of this base bars considering all other base bars in this barsgroup:
  double result = 0;
  int index = baseBars.indexOf(thisBase);
  if (index >= 0)
  {
    int startIndex;
    double lowerPixelWidth, upperPixelWidth;
    if (baseBars.size() % 2 == 1 && index == (baseBars.size()-1)/2) // is center bar (int division on purpose)
    {
      return result;
    } else if (index < (baseBars.size()-1)/2.0) // bar is to the left of center
    {
      if (baseBars.size() % 2 == 0) // even number of bars
      {
        startIndex = baseBars.size()/2-1;
        result -= getPixelSpacing(baseBars.at(startIndex), keyCoord)*0.5; // half of middle spacing
      } else // uneven number of bars
      {
        startIndex = (baseBars.size()-1)/2-1;
        baseBars.at((baseBars.size()-1)/2)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
        result -= qAbs(upperPixelWidth-lowerPixelWidth)*0.5; // half of center bar
        result -= getPixelSpacing(baseBars.at((baseBars.size()-1)/2), keyCoord); // center bar spacing
      }
      for (int i=startIndex; i>index; --i) // add widths and spacings of bars in between center and our bars
      {
        baseBars.at(i)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
        result -= qAbs(upperPixelWidth-lowerPixelWidth);
        result -= getPixelSpacing(baseBars.at(i), keyCoord);
      }
      // finally half of our bars width:
      baseBars.at(index)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
      result -= qAbs(upperPixelWidth-lowerPixelWidth)*0.5;
    } else // bar is to the right of center
    {
      if (baseBars.size() % 2 == 0) // even number of bars
      {
        startIndex = baseBars.size()/2;
        result += getPixelSpacing(baseBars.at(startIndex), keyCoord)*0.5; // half of middle spacing
      } else // uneven number of bars
      {
        startIndex = (baseBars.size()-1)/2+1;
        baseBars.at((baseBars.size()-1)/2)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
        result += qAbs(upperPixelWidth-lowerPixelWidth)*0.5; // half of center bar
        result += getPixelSpacing(baseBars.at((baseBars.size()-1)/2), keyCoord); // center bar spacing
      }
      for (int i=startIndex; i<index; ++i) // add widths and spacings of bars in between center and our bars
      {
        baseBars.at(i)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
        result += qAbs(upperPixelWidth-lowerPixelWidth);
        result += getPixelSpacing(baseBars.at(i), keyCoord);
      }
      // finally half of our bars width:
      baseBars.at(index)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
      result += qAbs(upperPixelWidth-lowerPixelWidth)*0.5;
    }
  }
  return result;
}

/*! \internal
  
  Returns the spacing in pixels which is between this \a bars and the following one, both at the
  key coordinate \a keyCoord.
  
  \note Typically the returned value doesn't depend on \a bars or \a keyCoord. \a bars is only
  needed to get acces to the key axis transformation and axis rect for the modes \ref
  stAxisRectRatio and \ref stPlotCoords. The \a keyCoord is only relevant for spacings given in
  \ref stPlotCoords on a logarithmic axis.
*/
double QCPBarsGroup::getPixelSpacing(const QCPBars *bars, double keyCoord)
{
  switch (mSpacingType)
  {
    case stAbsolute:
    {
      return mSpacing;
    }
    case stAxisRectRatio:
    {
      if (bars->keyAxis()->orientation() == Qt::Horizontal)
        return bars->keyAxis()->axisRect()->width()*mSpacing;
      else
        return bars->keyAxis()->axisRect()->height()*mSpacing;
    }
    case stPlotCoords:
    {
      double keyPixel = bars->keyAxis()->coordToPixel(keyCoord);
      return bars->keyAxis()->coordToPixel(keyCoord+mSpacing)-keyPixel;
    }
  }
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBarData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBarData
  \brief Holds the data of one single data point (one bar) for QCPBars.
  
  The container for storing multiple data points is \ref QCPBarDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this bar
  \li \a value: height coordinate on the value axis of this bar
  
  \see QCPBarDataaMap
*/

/*!
  Constructs a bar data point with key and value set to zero.
*/
QCPBarData::QCPBarData() :
  key(0),
  value(0)
{
}

/*!
  Constructs a bar data point with the specified \a key and \a value.
*/
QCPBarData::QCPBarData(double key, double value) :
  key(key),
  value(value)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBars
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBars
  \brief A plottable representing a bar chart in a plot.

  \image html QCPBars.png
  
  To plot data, assign it with the \ref setData or \ref addData functions.
  
  \section appearance Changing the appearance
  
  The appearance of the bars is determined by the pen and the brush (\ref setPen, \ref setBrush).
  The width of the individual bars can be controlled with \ref setWidthType and \ref setWidth.
  
  Bar charts are stackable. This means, two QCPBars plottables can be placed on top of each other
  (see \ref QCPBars::moveAbove). So when two bars are at the same key position, they will appear
  stacked.
  
  If you would like to group multiple QCPBars plottables together so they appear side by side as
  shown below, use QCPBarsGroup.
  
  \image html QCPBarsGroup.png
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPBars is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-1
  add it to the customPlot with QCustomPlot::addPlottable:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-2
  and then modify the properties of the newly created plottable, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-3
*/

/* start of documentation of inline functions */

/*! \fn QCPBars *QCPBars::barBelow() const
  Returns the bars plottable that is directly below this bars plottable.
  If there is no such plottable, returns 0.
  
  \see barAbove, moveBelow, moveAbove
*/

/*! \fn QCPBars *QCPBars::barAbove() const
  Returns the bars plottable that is directly above this bars plottable.
  If there is no such plottable, returns 0.
  
  \see barBelow, moveBelow, moveAbove
*/

/* end of documentation of inline functions */

/*!
  Constructs a bar chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPBars can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the bar chart.
*/
QCPBars::QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mData(new QCPBarDataMap),
  mWidth(0.75),
  mWidthType(wtPlotCoords),
  mBarsGroup(0),
  mBaseValue(0)
{
  // modify inherited properties from abstract plottable:
  mPen.setColor(Qt::blue);
  mPen.setStyle(Qt::SolidLine);
  mBrush.setColor(QColor(40, 50, 255, 30));
  mBrush.setStyle(Qt::SolidPattern);
  mSelectedPen = mPen;
  mSelectedPen.setWidthF(2.5);
  mSelectedPen.setColor(QColor(80, 80, 255)); // lighter than Qt::blue of mPen
  mSelectedBrush = mBrush;
}

QCPBars::~QCPBars()
{
  setBarsGroup(0);
  if (mBarBelow || mBarAbove)
    connectBars(mBarBelow.data(), mBarAbove.data()); // take this bar out of any stacking
  delete mData;
}

/*!
  Sets the width of the bars.

  How the number passed as \a width is interpreted (e.g. screen pixels, plot coordinates,...),
  depends on the currently set width type, see \ref setWidthType and \ref WidthType.
*/
void QCPBars::setWidth(double width)
{
  mWidth = width;
}

/*!
  Sets how the width of the bars is defined. See the documentation of \ref WidthType for an
  explanation of the possible values for \a widthType.
  
  The default value is \ref wtPlotCoords.
  
  \see setWidth
*/
void QCPBars::setWidthType(QCPBars::WidthType widthType)
{
  mWidthType = widthType;
}

/*!
  Sets to which QCPBarsGroup this QCPBars instance belongs to. Alternatively, you can also use \ref
  QCPBarsGroup::append.
  
  To remove this QCPBars from any group, set \a barsGroup to 0.
*/
void QCPBars::setBarsGroup(QCPBarsGroup *barsGroup)
{
  // deregister at old group:
  if (mBarsGroup)
    mBarsGroup->unregisterBars(this);
  mBarsGroup = barsGroup;
  // register at new group:
  if (mBarsGroup)
    mBarsGroup->registerBars(this);
}

/*!
  Sets the base value of this bars plottable.

  The base value defines where on the value coordinate the bars start. How far the bars extend from
  the base value is given by their individual value data. For example, if the base value is set to
  1, a bar with data value 2 will have its lowest point at value coordinate 1 and highest point at
  3.
  
  For stacked bars, only the base value of the bottom-most QCPBars has meaning.
  
  The default base value is 0.
*/
void QCPBars::setBaseValue(double baseValue)
{
  mBaseValue = baseValue;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPBars::setData(QCPBarDataMap *data, bool copy)
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
  
  Replaces the current data with the provided points in \a key and \a value tuples. The
  provided vectors should have equal length. Else, the number of added points will be the size of
  the smallest vector.
*/
void QCPBars::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPBarData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Moves this bars plottable below \a bars. In other words, the bars of this plottable will appear
  below the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.
  
  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object below itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.
  
  To remove this bars plottable from any stacking, set \a bars to 0.
  
  \see moveBelow, barAbove, barBelow
*/
void QCPBars::moveBelow(QCPBars *bars)
{
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
  {
    qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar below it:
  if (bars)
  {
    if (bars->mBarBelow)
      connectBars(bars->mBarBelow.data(), this);
    connectBars(this, bars);
  }
}

/*!
  Moves this bars plottable above \a bars. In other words, the bars of this plottable will appear
  above the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.
  
  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object below itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.
  
  To remove this bars plottable from any stacking, set \a bars to 0.
  
  \see moveBelow, barBelow, barAbove
*/
void QCPBars::moveAbove(QCPBars *bars)
{
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
  {
    qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar above it:
  if (bars)
  {
    if (bars->mBarAbove)
      connectBars(this, bars->mBarAbove.data());
    connectBars(bars, this);
  }
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  \see removeData
*/
void QCPBars::addData(const QCPBarDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  Adds the provided single data point in \a data to the current data.
  \see removeData
*/
void QCPBars::addData(const QCPBarData &data)
{
  mData->insertMulti(data.key, data);
}

/*! \overload
  Adds the provided single data point as \a key and \a value tuple to the current data
  \see removeData
*/
void QCPBars::addData(double key, double value)
{
  QCPBarData newData;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.key, newData);
}

/*! \overload
  Adds the provided data points as \a key and \a value tuples to the current data.
  \see removeData
*/
void QCPBars::addData(const QVector<double> &keys, const QVector<double> &values)
{
  int n = keys.size();
  n = qMin(n, values.size());
  QCPBarData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = keys[i];
    newData.value = values[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Removes all data points with key smaller than \a key.
  \see addData, clearData
*/
void QCPBars::removeDataBefore(double key)
{
  QCPBarDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < key)
    it = mData->erase(it);
}

/*!
  Removes all data points with key greater than \a key.
  \see addData, clearData
*/
void QCPBars::removeDataAfter(double key)
{
  if (mData->isEmpty()) return;
  QCPBarDataMap::iterator it = mData->upperBound(key);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with key between \a fromKey and \a toKey. if \a fromKey is
  greater or equal to \a toKey, the function does nothing. To remove a single data point with known
  key, use \ref removeData(double key).
  
  \see addData, clearData
*/
void QCPBars::removeData(double fromKey, double toKey)
{
  if (fromKey >= toKey || mData->isEmpty()) return;
  QCPBarDataMap::iterator it = mData->upperBound(fromKey);
  QCPBarDataMap::iterator itEnd = mData->upperBound(toKey);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at \a key. If the position is not known with absolute precision,
  consider using \ref removeData(double fromKey, double toKey) with a small fuzziness interval
  around the suspected position, depeding on the precision with which the key is known.
  
  \see addData, clearData
*/
void QCPBars::removeData(double key)
{
  mData->remove(key);
}

/*!
  Removes all data points.
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPBars::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPBars::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    QCPBarDataMap::ConstIterator it;
    for (it = mData->constBegin(); it != mData->constEnd(); ++it)
    {
      if (getBarPolygon(it.value().key, it.value().value).boundingRect().contains(pos))
        return mParentPlot->selectionTolerance()*0.99;
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPBars::draw(QCPPainter *painter)
{
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (mData->isEmpty()) return;
  
  QCPBarDataMap::const_iterator it, lower, upperEnd;
  getVisibleDataBounds(lower, upperEnd);
  for (it = lower; it != upperEnd; ++it)
  {
    // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
    if (QCP::isInvalidData(it.value().key, it.value().value))
      qDebug() << Q_FUNC_INFO << "Data point at" << it.key() << "of drawn range invalid." << "Plottable name:" << name();
#endif
    QPolygonF barPolygon = getBarPolygon(it.key(), it.value().value);
    // draw bar fill:
    if (mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0)
    {
      applyFillAntialiasingHint(painter);
      painter->setPen(Qt::NoPen);
      painter->setBrush(mainBrush());
      painter->drawPolygon(barPolygon);
    }
    // draw bar line:
    if (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
    {
      applyDefaultAntialiasingHint(painter);
      painter->setPen(mainPen());
      painter->setBrush(Qt::NoBrush);
      painter->drawPolyline(barPolygon);
    }
  }
}

/* inherits documentation from base class */
void QCPBars::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setBrush(mBrush);
  painter->setPen(mPen);
  QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*!  \internal
  
  called by \ref draw to determine which data (key) range is visible at the current key axis range
  setting, so only that needs to be processed. It also takes into account the bar width.
  
  \a lower returns an iterator to the lowest data point that needs to be taken into account when
  plotting. Note that in order to get a clean plot all the way to the edge of the axis rect, \a
  lower may still be just outside the visible range.
  
  \a upperEnd returns an iterator one higher than the highest visible data point. Same as before, \a
  upperEnd may also lie just outside of the visible range.
  
  if the bars plottable contains no data, both \a lower and \a upperEnd point to constEnd.
*/
void QCPBars::getVisibleDataBounds(QCPBarDataMap::const_iterator &lower, QCPBarDataMap::const_iterator &upperEnd) const
{
  if (!mKeyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  if (mData->isEmpty())
  {
    lower = mData->constEnd();
    upperEnd = mData->constEnd();
    return;
  }
  
  // get visible data range as QMap iterators
  lower = mData->lowerBound(mKeyAxis.data()->range().lower);
  upperEnd = mData->upperBound(mKeyAxis.data()->range().upper);
  double lowerPixelBound = mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().lower);
  double upperPixelBound = mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().upper);
  bool isVisible = false;
  // walk left from lbound to find lower bar that actually is completely outside visible pixel range:
  QCPBarDataMap::const_iterator it = lower;
  while (it != mData->constBegin())
  {
    --it;
    QRectF barBounds = getBarPolygon(it.value().key, it.value().value).boundingRect();
    if (mKeyAxis.data()->orientation() == Qt::Horizontal)
      isVisible = ((!mKeyAxis.data()->rangeReversed() && barBounds.right() >= lowerPixelBound) || (mKeyAxis.data()->rangeReversed() && barBounds.left() <= lowerPixelBound));
    else // keyaxis is vertical
      isVisible = ((!mKeyAxis.data()->rangeReversed() && barBounds.top() <= lowerPixelBound) || (mKeyAxis.data()->rangeReversed() && barBounds.bottom() >= lowerPixelBound));
    if (isVisible)
      lower = it;
    else
      break;
  }
  // walk right from ubound to find upper bar that actually is completely outside visible pixel range:
  it = upperEnd;
  while (it != mData->constEnd())
  {
    QRectF barBounds = getBarPolygon(upperEnd.value().key, upperEnd.value().value).boundingRect();
    if (mKeyAxis.data()->orientation() == Qt::Horizontal)
      isVisible = ((!mKeyAxis.data()->rangeReversed() && barBounds.left() <= upperPixelBound) || (mKeyAxis.data()->rangeReversed() && barBounds.right() >= upperPixelBound));
    else // keyaxis is vertical
      isVisible = ((!mKeyAxis.data()->rangeReversed() && barBounds.bottom() >= upperPixelBound) || (mKeyAxis.data()->rangeReversed() && barBounds.top() <= upperPixelBound));
    if (isVisible)
      upperEnd = it+1;
    else
      break;
    ++it;
  }
}

/*! \internal
  
  Returns the polygon of a single bar with \a key and \a value. The Polygon is open at the bottom
  and shifted according to the bar stacking (see \ref moveAbove) and base value (see \ref
  setBaseValue).
*/
QPolygonF QCPBars::getBarPolygon(double key, double value) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPolygonF(); }
  
  QPolygonF result;
  double lowerPixelWidth, upperPixelWidth;
  getPixelWidth(key, lowerPixelWidth, upperPixelWidth);
  double base = getStackedBaseValue(key, value >= 0);
  double basePixel = valueAxis->coordToPixel(base);
  double valuePixel = valueAxis->coordToPixel(base+value);
  double keyPixel = keyAxis->coordToPixel(key);
  if (mBarsGroup)
    keyPixel += mBarsGroup->keyPixelOffset(this, key);
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    result << QPointF(keyPixel+lowerPixelWidth, basePixel);
    result << QPointF(keyPixel+lowerPixelWidth, valuePixel);
    result << QPointF(keyPixel+upperPixelWidth, valuePixel);
    result << QPointF(keyPixel+upperPixelWidth, basePixel);
  } else
  {
    result << QPointF(basePixel, keyPixel+lowerPixelWidth);
    result << QPointF(valuePixel, keyPixel+lowerPixelWidth);
    result << QPointF(valuePixel, keyPixel+upperPixelWidth);
    result << QPointF(basePixel, keyPixel+upperPixelWidth);
  }
  return result;
}

/*! \internal
  
  This function is used to determine the width of the bar at coordinate \a key, according to the
  specified width (\ref setWidth) and width type (\ref setWidthType).
  
  The output parameters \a lower and \a upper return the number of pixels the bar extends to lower
  and higher keys, relative to the \a key coordinate (so with a non-reversed horizontal axis, \a
  lower is negative and \a upper positive).
*/
void QCPBars::getPixelWidth(double key, double &lower, double &upper) const
{
  switch (mWidthType)
  {
    case wtAbsolute:
    {
      upper = mWidth*0.5;
      lower = -upper;
      if (mKeyAxis && (mKeyAxis.data()->rangeReversed() ^ (mKeyAxis.data()->orientation() == Qt::Vertical)))
        qSwap(lower, upper);
      break;
    }
    case wtAxisRectRatio:
    {
      if (mKeyAxis && mKeyAxis.data()->axisRect())
      {
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
          upper = mKeyAxis.data()->axisRect()->width()*mWidth*0.5;
        else
          upper = mKeyAxis.data()->axisRect()->height()*mWidth*0.5;
        lower = -upper;
        if (mKeyAxis && (mKeyAxis.data()->rangeReversed() ^ (mKeyAxis.data()->orientation() == Qt::Vertical)))
          qSwap(lower, upper);
      } else
        qDebug() << Q_FUNC_INFO << "No key axis or axis rect defined";
      break;
    }
    case wtPlotCoords:
    {
      if (mKeyAxis)
      {
        double keyPixel = mKeyAxis.data()->coordToPixel(key);
        upper = mKeyAxis.data()->coordToPixel(key+mWidth*0.5)-keyPixel;
        lower = mKeyAxis.data()->coordToPixel(key-mWidth*0.5)-keyPixel;
        // no need to qSwap(lower, higher) when range reversed, because higher/lower are gained by
        // coordinate transform which includes range direction
      } else
        qDebug() << Q_FUNC_INFO << "No key axis defined";
      break;
    }
  }
}

/*! \internal
  
  This function is called to find at which value to start drawing the base of a bar at \a key, when
  it is stacked on top of another QCPBars (e.g. with \ref moveAbove).
  
  positive and negative bars are separated per stack (positive are stacked above baseValue upwards,
  negative are stacked below baseValue downwards). This can be indicated with \a positive. So if the
  bar for which we need the base value is negative, set \a positive to false.
*/
double QCPBars::getStackedBaseValue(double key, bool positive) const
{
  if (mBarBelow)
  {
    double max = 0; // don't use mBaseValue here because only base value of bottom-most bar has meaning in a bar stack
    // find bars of mBarBelow that are approximately at key and find largest one:
    double epsilon = qAbs(key)*1e-6; // should be safe even when changed to use float at some point
    if (key == 0)
      epsilon = 1e-6;
    QCPBarDataMap::const_iterator it = mBarBelow.data()->mData->lowerBound(key-epsilon);
    QCPBarDataMap::const_iterator itEnd = mBarBelow.data()->mData->upperBound(key+epsilon);
    while (it != itEnd)
    {
      if ((positive && it.value().value > max) ||
          (!positive && it.value().value < max))
        max = it.value().value;
      ++it;
    }
    // recurse down the bar-stack to find the total height:
    return max + mBarBelow.data()->getStackedBaseValue(key, positive);
  } else
    return mBaseValue;
}

/*! \internal

  Connects \a below and \a above to each other via their mBarAbove/mBarBelow properties. The bar(s)
  currently above lower and below upper will become disconnected to lower/upper.
  
  If lower is zero, upper will be disconnected at the bottom.
  If upper is zero, lower will be disconnected at the top.
*/
void QCPBars::connectBars(QCPBars *lower, QCPBars *upper)
{
  if (!lower && !upper) return;
  
  if (!lower) // disconnect upper at bottom
  {
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = 0;
    upper->mBarBelow = 0;
  } else if (!upper) // disconnect lower at top
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = 0;
    lower->mBarAbove = 0;
  } else // connect lower and upper
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = 0;
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = 0;
    lower->mBarAbove = upper;
    upper->mBarBelow = lower;
  }
}

/* inherits documentation from base class */
QCPRange QCPBars::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  QCPBarDataMap::const_iterator it = mData->constBegin();
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
  // determine exact range of bars by including bar width and barsgroup offset:
  if (haveLower && mKeyAxis)
  {
    double lowerPixelWidth, upperPixelWidth, keyPixel;
    getPixelWidth(range.lower, lowerPixelWidth, upperPixelWidth);
    keyPixel = mKeyAxis.data()->coordToPixel(range.lower) + lowerPixelWidth;
    if (mBarsGroup)
      keyPixel += mBarsGroup->keyPixelOffset(this, range.lower);
    range.lower = mKeyAxis.data()->pixelToCoord(keyPixel);
  }
  if (haveUpper && mKeyAxis)
  {
    double lowerPixelWidth, upperPixelWidth, keyPixel;
    getPixelWidth(range.upper, lowerPixelWidth, upperPixelWidth);
    keyPixel = mKeyAxis.data()->coordToPixel(range.upper) + upperPixelWidth;
    if (mBarsGroup)
      keyPixel += mBarsGroup->keyPixelOffset(this, range.upper);
    range.upper = mKeyAxis.data()->pixelToCoord(keyPixel);
  }
  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPBars::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  range.lower = mBaseValue;
  range.upper = mBaseValue;
  bool haveLower = true; // set to true, because baseValue should always be visible in bar charts
  bool haveUpper = true; // set to true, because baseValue should always be visible in bar charts
  double current;
  
  QCPBarDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().value + getStackedBaseValue(it.value().key, it.value().value >= 0);
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
  
  foundRange = true; // return true because bar charts always have the 0-line visible
  return range;
}

