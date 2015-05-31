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

#include "plottable.h"

#include "painter.h"
#include "core.h"
#include "axis.h"
#include "layoutelements/layoutelement-axisrect.h"
#include "layoutelements/layoutelement-legend.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractPlottable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractPlottable
  \brief The abstract base class for all data representing objects in a plot.

  It defines a very basic interface like name, pen, brush, visibility etc. Since this class is
  abstract, it can't be instantiated. Use one of the subclasses or create a subclass yourself to
  create new ways of displaying data (see "Creating own plottables" below).
  
  All further specifics are in the subclasses, for example:
  \li A normal graph with possibly a line, scatter points and error bars: \ref QCPGraph
  (typically created with \ref QCustomPlot::addGraph)
  \li A parametric curve: \ref QCPCurve
  \li A bar chart: \ref QCPBars
  \li A statistical box plot: \ref QCPStatisticalBox
  \li A color encoded two-dimensional map: \ref QCPColorMap
  \li An OHLC/Candlestick chart: \ref QCPFinancial
  
  \section plottables-subclassing Creating own plottables
  
  To create an own plottable, you implement a subclass of QCPAbstractPlottable. These are the pure
  virtual functions, you must implement:
  \li \ref clearData
  \li \ref selectTest
  \li \ref draw
  \li \ref drawLegendIcon
  \li \ref getKeyRange
  \li \ref getValueRange
  
  See the documentation of those functions for what they need to do.
  
  For drawing your plot, you can use the \ref coordsToPixels functions to translate a point in plot
  coordinates to pixel coordinates. This function is quite convenient, because it takes the
  orientation of the key and value axes into account for you (x and y are swapped when the key axis
  is vertical and the value axis horizontal). If you are worried about performance (i.e. you need
  to translate many points in a loop like QCPGraph), you can directly use \ref
  QCPAxis::coordToPixel. However, you must then take care about the orientation of the axis
  yourself.
  
  Here are some important members you inherit from QCPAbstractPlottable:
  <table>
  <tr>
    <td>QCustomPlot *\b mParentPlot</td>
    <td>A pointer to the parent QCustomPlot instance. The parent plot is inferred from the axes that are passed in the constructor.</td>
  </tr><tr>
    <td>QString \b mName</td>
    <td>The name of the plottable.</td>
  </tr><tr>
    <td>QPen \b mPen</td>
    <td>The generic pen of the plottable. You should use this pen for the most prominent data representing lines in the plottable (e.g QCPGraph uses this pen for its graph lines and scatters)</td>
  </tr><tr>
    <td>QPen \b mSelectedPen</td>
    <td>The generic pen that should be used when the plottable is selected (hint: \ref mainPen gives you the right pen, depending on selection state).</td>
  </tr><tr>
    <td>QBrush \b mBrush</td>
    <td>The generic brush of the plottable. You should use this brush for the most prominent fillable structures in the plottable (e.g. QCPGraph uses this brush to control filling under the graph)</td>
  </tr><tr>
    <td>QBrush \b mSelectedBrush</td>
    <td>The generic brush that should be used when the plottable is selected (hint: \ref mainBrush gives you the right brush, depending on selection state).</td>
  </tr><tr>
    <td>QPointer<QCPAxis>\b mKeyAxis, \b mValueAxis</td>
    <td>The key and value axes this plottable is attached to. Call their QCPAxis::coordToPixel functions to translate coordinates to pixels in either the key or value dimension.
        Make sure to check whether the pointer is null before using it. If one of the axes is null, don't draw the plottable.</td>
  </tr><tr>
    <td>bool \b mSelected</td>
    <td>indicates whether the plottable is selected or not.</td>
  </tr>
  </table>
*/

/* start of documentation of pure virtual functions */

/*! \fn void QCPAbstractPlottable::clearData() = 0
  Clears all data in the plottable.
*/

/*! \fn void QCPAbstractPlottable::drawLegendIcon(QCPPainter *painter, const QRect &rect) const = 0
  \internal
  
  called by QCPLegend::draw (via QCPPlottableLegendItem::draw) to create a graphical representation
  of this plottable inside \a rect, next to the plottable name.
  
  The passed \a painter has its cliprect set to \a rect, so painting outside of \a rect won't
  appear outside the legend icon border.
*/

/*! \fn QCPRange QCPAbstractPlottable::getKeyRange(bool &foundRange, SignDomain inSignDomain) const = 0
  \internal
  
  called by rescaleAxes functions to get the full data key bounds. For logarithmic plots, one can
  set \a inSignDomain to either \ref sdNegative or \ref sdPositive in order to restrict the
  returned range to that sign domain. E.g. when only negative range is wanted, set \a inSignDomain
  to \ref sdNegative and all positive points will be ignored for range calculation. For no
  restriction, just set \a inSignDomain to \ref sdBoth (default). \a foundRange is an output
  parameter that indicates whether a range could be found or not. If this is false, you shouldn't
  use the returned range (e.g. no points in data).

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero, which wouldn't count as a valid range.
  
  \see rescaleAxes, getValueRange
*/

/*! \fn QCPRange QCPAbstractPlottable::getValueRange(bool &foundRange, SignDomain inSignDomain) const = 0
  \internal
  
  called by rescaleAxes functions to get the full data value bounds. For logarithmic plots, one can
  set \a inSignDomain to either \ref sdNegative or \ref sdPositive in order to restrict the
  returned range to that sign domain. E.g. when only negative range is wanted, set \a inSignDomain
  to \ref sdNegative and all positive points will be ignored for range calculation. For no
  restriction, just set \a inSignDomain to \ref sdBoth (default). \a foundRange is an output
  parameter that indicates whether a range could be found or not. If this is false, you shouldn't
  use the returned range (e.g. no points in data).

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero, which wouldn't count as a valid range.
  
  \see rescaleAxes, getKeyRange
*/

/* end of documentation of pure virtual functions */
/* start of documentation of signals */

/*! \fn void QCPAbstractPlottable::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this plottable has changed, either by user
  interaction or by a direct call to \ref setSelected.
*/

/*! \fn void QCPAbstractPlottable::selectableChanged(bool selectable);
  
  This signal is emitted when the selectability of this plottable has changed.
  
  \see setSelectable
*/

/* end of documentation of signals */

/*!
  Constructs an abstract plottable which uses \a keyAxis as its key axis ("x") and \a valueAxis as
  its value axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance
  and have perpendicular orientations. If either of these restrictions is violated, a corresponding
  message is printed to the debug output (qDebug), the construction is not aborted, though.
  
  Since QCPAbstractPlottable is an abstract class that defines the basic interface to plottables,
  it can't be directly instantiated.
  
  You probably want one of the subclasses like \ref QCPGraph or \ref QCPCurve instead.
*/
QCPAbstractPlottable::QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPLayerable(keyAxis->parentPlot(), QString(), keyAxis->axisRect()),
  mName(),
  mAntialiasedFill(true),
  mAntialiasedScatters(true),
  mAntialiasedErrorBars(false),
  mPen(Qt::black),
  mSelectedPen(Qt::black),
  mBrush(Qt::NoBrush),
  mSelectedBrush(Qt::NoBrush),
  mKeyAxis(keyAxis),
  mValueAxis(valueAxis),
  mSelectable(true),
  mSelected(false)
{
  if (keyAxis->parentPlot() != valueAxis->parentPlot())
    qDebug() << Q_FUNC_INFO << "Parent plot of keyAxis is not the same as that of valueAxis.";
  if (keyAxis->orientation() == valueAxis->orientation())
    qDebug() << Q_FUNC_INFO << "keyAxis and valueAxis must be orthogonal to each other.";
}

/*!
   The name is the textual representation of this plottable as it is displayed in the legend
   (\ref QCPLegend). It may contain any UTF-8 characters, including newlines.
*/
void QCPAbstractPlottable::setName(const QString &name)
{
  mName = name;
}

/*!
  Sets whether fills of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedFill(bool enabled)
{
  mAntialiasedFill = enabled;
}

/*!
  Sets whether the scatter symbols of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedScatters(bool enabled)
{
  mAntialiasedScatters = enabled;
}

/*!
  Sets whether the error bars of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedErrorBars(bool enabled)
{
  mAntialiasedErrorBars = enabled;
}


/*!
  The pen is used to draw basic lines that make up the plottable representation in the
  plot.
  
  For example, the \ref QCPGraph subclass draws its graph lines with this pen.

  \see setBrush
*/
void QCPAbstractPlottable::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  When the plottable is selected, this pen is used to draw basic lines instead of the normal
  pen set via \ref setPen.

  \see setSelected, setSelectable, setSelectedBrush, selectTest
*/
void QCPAbstractPlottable::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  The brush is used to draw basic fills of the plottable representation in the
  plot. The Fill can be a color, gradient or texture, see the usage of QBrush.
  
  For example, the \ref QCPGraph subclass draws the fill under the graph with this brush, when
  it's not set to Qt::NoBrush.

  \see setPen
*/
void QCPAbstractPlottable::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  When the plottable is selected, this brush is used to draw fills instead of the normal
  brush set via \ref setBrush.

  \see setSelected, setSelectable, setSelectedPen, selectTest
*/
void QCPAbstractPlottable::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  The key axis of a plottable can be set to any axis of a QCustomPlot, as long as it is orthogonal
  to the plottable's value axis. This function performs no checks to make sure this is the case.
  The typical mathematical choice is to use the x-axis (QCustomPlot::xAxis) as key axis and the
  y-axis (QCustomPlot::yAxis) as value axis.
  
  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  QCustomPlot::addGraph when working with QCPGraphs through the dedicated graph interface).

  \see setValueAxis
*/
void QCPAbstractPlottable::setKeyAxis(QCPAxis *axis)
{
  mKeyAxis = axis;
}

/*!
  The value axis of a plottable can be set to any axis of a QCustomPlot, as long as it is
  orthogonal to the plottable's key axis. This function performs no checks to make sure this is the
  case. The typical mathematical choice is to use the x-axis (QCustomPlot::xAxis) as key axis and
  the y-axis (QCustomPlot::yAxis) as value axis.

  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  QCustomPlot::addGraph when working with QCPGraphs through the dedicated graph interface).
  
  \see setKeyAxis
*/
void QCPAbstractPlottable::setValueAxis(QCPAxis *axis)
{
  mValueAxis = axis;
}

/*!
  Sets whether the user can (de-)select this plottable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectPlottables.)
  
  However, even when \a selectable was set to false, it is possible to set the selection manually,
  by calling \ref setSelected directly.
  
  \see setSelected
*/
void QCPAbstractPlottable::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets whether this plottable is selected or not. When selected, it uses a different pen and brush
  to draw its lines and fills, see \ref setSelectedPen and \ref setSelectedBrush.

  The entire selection mechanism for plottables is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectPlottables. You only need to call this function when
  you wish to change the selection state manually.
  
  This function can change the selection state even when \ref setSelectable was set to false.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see setSelectable, selectTest
*/
void QCPAbstractPlottable::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/*!
  Rescales the key and value axes associated with this plottable to contain all displayed data, so
  the whole plottable is visible. If the scaling of an axis is logarithmic, rescaleAxes will make
  sure not to rescale to an illegal range i.e. a range containing different signs and/or zero.
  Instead it will stay in the current sign domain and ignore all parts of the plottable that lie
  outside of that domain.
  
  \a onlyEnlarge makes sure the ranges are only expanded, never reduced. So it's possible to show
  multiple plottables in their entirety by multiple calls to rescaleAxes where the first call has
  \a onlyEnlarge set to false (the default), and all subsequent set to true.
  
  \see rescaleKeyAxis, rescaleValueAxis, QCustomPlot::rescaleAxes, QCPAxis::rescale
*/
void QCPAbstractPlottable::rescaleAxes(bool onlyEnlarge) const
{
  rescaleKeyAxis(onlyEnlarge);
  rescaleValueAxis(onlyEnlarge);
}

/*!
  Rescales the key axis of the plottable so the whole plottable is visible.
  
  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleKeyAxis(bool onlyEnlarge) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  
  SignDomain signDomain = sdBoth;
  if (keyAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (keyAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getKeyRange(foundRange, signDomain);
  if (foundRange)
  {
    if (onlyEnlarge)
      newRange.expand(keyAxis->range());
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (keyAxis->scaleType() == QCPAxis::stLinear)
      {
        newRange.lower = center-keyAxis->range().size()/2.0;
        newRange.upper = center+keyAxis->range().size()/2.0;
      } else // scaleType() == stLogarithmic
      {
        newRange.lower = center/qSqrt(keyAxis->range().upper/keyAxis->range().lower);
        newRange.upper = center*qSqrt(keyAxis->range().upper/keyAxis->range().lower);
      }
    }
    keyAxis->setRange(newRange);
  }
}

/*!
  Rescales the value axis of the plottable so the whole plottable is visible.
  
  Returns true if the axis was actually scaled. This might not be the case if this plottable has an
  invalid range, e.g. because it has no data points.
  
  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleValueAxis(bool onlyEnlarge) const
{
  QCPAxis *valueAxis = mValueAxis.data();
  if (!valueAxis) { qDebug() << Q_FUNC_INFO << "invalid value axis"; return; }
  
  SignDomain signDomain = sdBoth;
  if (valueAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (valueAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getValueRange(foundRange, signDomain);
  if (foundRange)
  {
    if (onlyEnlarge)
      newRange.expand(valueAxis->range());
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (valueAxis->scaleType() == QCPAxis::stLinear)
      {
        newRange.lower = center-valueAxis->range().size()/2.0;
        newRange.upper = center+valueAxis->range().size()/2.0;
      } else // scaleType() == stLogarithmic
      {
        newRange.lower = center/qSqrt(valueAxis->range().upper/valueAxis->range().lower);
        newRange.upper = center*qSqrt(valueAxis->range().upper/valueAxis->range().lower);
      }
    }
    valueAxis->setRange(newRange);
  }
}

/*!
  Adds this plottable to the legend of the parent QCustomPlot (QCustomPlot::legend).
    
  Normally, a QCPPlottableLegendItem is created and inserted into the legend. If the plottable
  needs a more specialized representation in the legend, this function will take this into account
  and instead create the specialized subclass of QCPAbstractLegendItem.
    
  Returns true on success, i.e. when the legend exists and a legend item associated with this plottable isn't already in
  the legend.
    
  \see removeFromLegend, QCPLegend::addItem
*/
bool QCPAbstractPlottable::addToLegend()
{
  if (!mParentPlot || !mParentPlot->legend)
    return false;
  
  if (!mParentPlot->legend->hasItemWithPlottable(this))
  {
    mParentPlot->legend->addItem(new QCPPlottableLegendItem(mParentPlot->legend, this));
    return true;
  } else
    return false;
}

/*!
  Removes the plottable from the legend of the parent QCustomPlot. This means the
  QCPAbstractLegendItem (usually a QCPPlottableLegendItem) that is associated with this plottable
  is removed.
    
  Returns true on success, i.e. if the legend exists and a legend item associated with this
  plottable was found and removed.
    
  \see addToLegend, QCPLegend::removeItem
*/
bool QCPAbstractPlottable::removeFromLegend() const
{
  if (!mParentPlot->legend)
    return false;
  
  if (QCPPlottableLegendItem *lip = mParentPlot->legend->itemWithPlottable(this))
    return mParentPlot->legend->removeItem(lip);
  else
    return false;
}

/* inherits documentation from base class */
QRect QCPAbstractPlottable::clipRect() const
{
  if (mKeyAxis && mValueAxis)
    return mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect();
  else
    return QRect();
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractPlottable::selectionCategory() const
{
  return QCP::iSelectPlottables;
}

/*! \internal
  
  Convenience function for transforming a key/value pair to pixels on the QCustomPlot surface,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).
  
  \a key and \a value are transformed to the coodinates in pixels and are written to \a x and \a y.
    
  \see pixelsToCoords, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::coordsToPixels(double key, double value, double &x, double &y) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    x = keyAxis->coordToPixel(key);
    y = valueAxis->coordToPixel(value);
  } else
  {
    y = keyAxis->coordToPixel(key);
    x = valueAxis->coordToPixel(value);
  }
}

/*! \internal
  \overload
  
  Returns the input as pixel coordinates in a QPointF.
*/
const QPointF QCPAbstractPlottable::coordsToPixels(double key, double value) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPointF(); }
  
  if (keyAxis->orientation() == Qt::Horizontal)
    return QPointF(keyAxis->coordToPixel(key), valueAxis->coordToPixel(value));
  else
    return QPointF(valueAxis->coordToPixel(value), keyAxis->coordToPixel(key));
}

/*! \internal
  
  Convenience function for transforming a x/y pixel pair on the QCustomPlot surface to plot coordinates,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).
  
  \a x and \a y are transformed to the plot coodinates and are written to \a key and \a value.
    
  \see coordsToPixels, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::pixelsToCoords(double x, double y, double &key, double &value) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    key = keyAxis->pixelToCoord(x);
    value = valueAxis->pixelToCoord(y);
  } else
  {
    key = keyAxis->pixelToCoord(y);
    value = valueAxis->pixelToCoord(x);
  }
}

/*! \internal
  \overload

  Returns the pixel input \a pixelPos as plot coordinates \a key and \a value.
*/
void QCPAbstractPlottable::pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const
{
  pixelsToCoords(pixelPos.x(), pixelPos.y(), key, value);
}

/*! \internal

  Returns the pen that should be used for drawing lines of the plottable. Returns mPen when the
  graph is not selected and mSelectedPen when it is.
*/
QPen QCPAbstractPlottable::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the plottable. Returns mBrush when the
  graph is not selected and mSelectedBrush when it is.
*/
QBrush QCPAbstractPlottable::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyScattersAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aePlottables);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable fills.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyDefaultAntialiasingHint, applyScattersAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyFillAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedFill, QCP::aeFills);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable scatter points.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyDefaultAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyScattersAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedScatters, QCP::aeScatters);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable error bars.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyScattersAntialiasingHint, applyDefaultAntialiasingHint
*/
void QCPAbstractPlottable::applyErrorBarsAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedErrorBars, QCP::aeErrorBars);
}

/*! \internal

  Finds the shortest squared distance of \a point to the line segment defined by \a start and \a
  end.
  
  This function may be used to help with the implementation of the \ref selectTest function for
  specific plottables.
  
  \note This function is identical to QCPAbstractItem::distSqrToLine
*/
double QCPAbstractPlottable::distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const
{
  QVector2D a(start);
  QVector2D b(end);
  QVector2D p(point);
  QVector2D v(b-a);
  
  double vLengthSqr = v.lengthSquared();
  if (!qFuzzyIsNull(vLengthSqr))
  {
    double mu = QVector2D::dotProduct(p-a, v)/vLengthSqr;
    if (mu < 0)
      return (a-p).lengthSquared();
    else if (mu > 1)
      return (b-p).lengthSquared();
    else
      return ((a + mu*v)-p).lengthSquared();
  } else
    return (a-p).lengthSquared();
}

/* inherits documentation from base class */
void QCPAbstractPlottable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAbstractPlottable::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}
