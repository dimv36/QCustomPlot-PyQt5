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

#include "axis.h"

#include "painter.h"
#include "core.h"
#include "plottable.h"
#include "plottables/plottable-graph.h"
#include "item.h"
#include "layoutelements/layoutelement-axisrect.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGrid
  \brief Responsible for drawing the grid of a QCPAxis.
  
  This class is tightly bound to QCPAxis. Every axis owns a grid instance and uses it to draw the
  grid lines, sub grid lines and zero-line. You can interact with the grid of an axis via \ref
  QCPAxis::grid. Normally, you don't need to create an instance of QCPGrid yourself.
  
  The axis and grid drawing was split into two classes to allow them to be placed on different
  layers (both QCPAxis and QCPGrid inherit from QCPLayerable). Thus it is possible to have the grid
  in the background and the axes in the foreground, and any plottables/items in between. This
  described situation is the default setup, see the QCPLayer documentation.
*/

/*!
  Creates a QCPGrid instance and sets default values.
  
  You shouldn't instantiate grids on their own, since every QCPAxis brings its own QCPGrid.
*/
QCPGrid::QCPGrid(QCPAxis *parentAxis) :
  QCPLayerable(parentAxis->parentPlot(), QString(), parentAxis),
  mParentAxis(parentAxis)
{
  // warning: this is called in QCPAxis constructor, so parentAxis members should not be accessed/called
  setParent(parentAxis);
  setPen(QPen(QColor(200,200,200), 0, Qt::DotLine));
  setSubGridPen(QPen(QColor(220,220,220), 0, Qt::DotLine));
  setZeroLinePen(QPen(QColor(200,200,200), 0, Qt::SolidLine));
  setSubGridVisible(false);
  setAntialiased(false);
  setAntialiasedSubGrid(false);
  setAntialiasedZeroLine(false);
}

/*!
  Sets whether grid lines at sub tick marks are drawn.
  
  \see setSubGridPen
*/
void QCPGrid::setSubGridVisible(bool visible)
{
  mSubGridVisible = visible;
}

/*!
  Sets whether sub grid lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedSubGrid(bool enabled)
{
  mAntialiasedSubGrid = enabled;
}

/*!
  Sets whether zero lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedZeroLine(bool enabled)
{
  mAntialiasedZeroLine = enabled;
}

/*!
  Sets the pen with which (major) grid lines are drawn.
*/
void QCPGrid::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen with which sub grid lines are drawn.
*/
void QCPGrid::setSubGridPen(const QPen &pen)
{
  mSubGridPen = pen;
}

/*!
  Sets the pen with which zero lines are drawn.
  
  Zero lines are lines at value coordinate 0 which may be drawn with a different pen than other grid
  lines. To disable zero lines and just draw normal grid lines at zero, set \a pen to Qt::NoPen.
*/
void QCPGrid::setZeroLinePen(const QPen &pen)
{
  mZeroLinePen = pen;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing the major grid lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPGrid::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeGrid);
}

/*! \internal
  
  Draws grid lines and sub grid lines at the positions of (sub) ticks of the parent axis, spanning
  over the complete axis rect. Also draws the zero line, if appropriate (\ref setZeroLinePen).
*/
void QCPGrid::draw(QCPPainter *painter)
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  if (mSubGridVisible)
    drawSubGridLines(painter);
  drawGridLines(painter);
}

/*! \internal
  
  Draws the main grid lines and possibly a zero line with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawGridLines(QCPPainter *painter) const
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  int lowTick = mParentAxis->mLowestVisibleTick;
  int highTick = mParentAxis->mHighestVisibleTick;
  double t; // helper variable, result of coordinate-to-pixel transforms
  if (mParentAxis->orientation() == Qt::Horizontal)
  {
    // draw zeroline:
    int zeroLineIndex = -1;
    if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
    {
      applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
      painter->setPen(mZeroLinePen);
      double epsilon = mParentAxis->range().size()*1E-6; // for comparing double to zero
      for (int i=lowTick; i <= highTick; ++i)
      {
        if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
        {
          zeroLineIndex = i;
          t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
          painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
          break;
        }
      }
    }
    // draw grid lines:
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    for (int i=lowTick; i <= highTick; ++i)
    {
      if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
      t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
      painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
    }
  } else
  {
    // draw zeroline:
    int zeroLineIndex = -1;
    if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
    {
      applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
      painter->setPen(mZeroLinePen);
      double epsilon = mParentAxis->mRange.size()*1E-6; // for comparing double to zero
      for (int i=lowTick; i <= highTick; ++i)
      {
        if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
        {
          zeroLineIndex = i;
          t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
          painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
          break;
        }
      }
    }
    // draw grid lines:
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    for (int i=lowTick; i <= highTick; ++i)
    {
      if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
      t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
      painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
    }
  }
}

/*! \internal
  
  Draws the sub grid lines with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawSubGridLines(QCPPainter *painter) const
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  applyAntialiasingHint(painter, mAntialiasedSubGrid, QCP::aeSubGrid);
  double t; // helper variable, result of coordinate-to-pixel transforms
  painter->setPen(mSubGridPen);
  if (mParentAxis->orientation() == Qt::Horizontal)
  {
    for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
    {
      t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // x
      painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
    }
  } else
  {
    for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
    {
      t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // y
      painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxis
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxis
  \brief Manages a single axis inside a QCustomPlot.

  Usually doesn't need to be instantiated externally. Access %QCustomPlot's default four axes via
  QCustomPlot::xAxis (bottom), QCustomPlot::yAxis (left), QCustomPlot::xAxis2 (top) and
  QCustomPlot::yAxis2 (right).
  
  Axes are always part of an axis rect, see QCPAxisRect.
  \image html AxisNamesOverview.png
  <center>Naming convention of axis parts</center>
  \n
    
  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed gray line
  on the left represents the QCustomPlot widget border.</center>

*/

/* start of documentation of inline functions */

/*! \fn Qt::Orientation QCPAxis::orientation() const
  
  Returns the orientation of this axis. The axis orientation (horizontal or vertical) is deduced
  from the axis type (left, top, right or bottom).
  
  \see orientation(AxisType type)
*/

/*! \fn QCPGrid *QCPAxis::grid() const
  
  Returns the \ref QCPGrid instance belonging to this axis. Access it to set details about the way the
  grid is displayed.
*/

/*! \fn static Qt::Orientation QCPAxis::orientation(AxisType type)
  
  Returns the orientation of the specified axis type
  
  \see orientation()
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCPAxis::ticksRequest()
  
  This signal is emitted when \ref setAutoTicks is false and the axis is about to generate tick
  labels for a replot.
  
  Modifying the tick positions can be done with \ref setTickVector. If you also want to control the
  tick labels, set \ref setAutoTickLabels to false and also provide the labels with \ref
  setTickVectorLabels.
  
  If you only want static ticks you probably don't need this signal, since you can just set the
  tick vector (and possibly tick label vector) once. However, if you want to provide ticks (and
  maybe labels) dynamically, e.g. depending on the current axis range, connect a slot to this
  signal and set the vector/vectors there.
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange)

  This signal is emitted when the range of this axis has changed. You can connect it to the \ref
  setRange slot of another axis to communicate the new range to the other axis, in order for it to
  be synchronized.
  
  You may also manipulate/correct the range with \ref setRange in a slot connected to this signal.
  This is useful if for example a maximum range span shall not be exceeded, or if the lower/upper
  range shouldn't go beyond certain values. For example, the following slot would limit the x axis
  to only positive ranges:
  \code
  if (newRange.lower < 0)
    plot->xAxis->setRange(0, newRange.size());
  \endcode
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange, const QCPRange &oldRange)
  \overload
  
  Additionally to the new range, this signal also provides the previous range held by the axis as
  \a oldRange.
*/

/*! \fn void QCPAxis::scaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the scale type changes, by calls to \ref setScaleType
*/

/*! \fn void QCPAxis::selectionChanged(QCPAxis::SelectableParts selection)
  
  This signal is emitted when the selection state of this axis has changed, either by user interaction
  or by a direct call to \ref setSelectedParts.
*/

/*! \fn void QCPAxis::selectableChanged(const QCPAxis::SelectableParts &parts);
  
  This signal is emitted when the selectability changes, by calls to \ref setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs an Axis instance of Type \a type for the axis rect \a parent.
  
  Usually it isn't necessary to instantiate axes directly, because you can let QCustomPlot create
  them for you with \ref QCPAxisRect::addAxis. If you want to use own QCPAxis-subclasses however,
  create them manually and then inject them also via \ref QCPAxisRect::addAxis.
*/
QCPAxis::QCPAxis(QCPAxisRect *parent, AxisType type) :
  QCPLayerable(parent->parentPlot(), QString(), parent),
  // axis base:
  mAxisType(type),
  mAxisRect(parent),
  mPadding(5),
  mOrientation(orientation(type)),
  mSelectableParts(spAxis | spTickLabels | spAxisLabel),
  mSelectedParts(spNone),
  mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedBasePen(QPen(Qt::blue, 2)),
  // axis label:
  mLabel(),
  mLabelFont(mParentPlot->font()),
  mSelectedLabelFont(QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
  mLabelColor(Qt::black),
  mSelectedLabelColor(Qt::blue),
  // tick labels:
  mTickLabels(true),
  mAutoTickLabels(true),
  mTickLabelType(ltNumber),
  mTickLabelFont(mParentPlot->font()),
  mSelectedTickLabelFont(QFont(mTickLabelFont.family(), mTickLabelFont.pointSize(), QFont::Bold)),
  mTickLabelColor(Qt::black),
  mSelectedTickLabelColor(Qt::blue),
  mDateTimeFormat(QLatin1String("hh:mm:ss\ndd.MM.yy")),
  mDateTimeSpec(Qt::LocalTime),
  mNumberPrecision(6),
  mNumberFormatChar('g'),
  mNumberBeautifulPowers(true),
  // ticks and subticks:
  mTicks(true),
  mTickStep(1),
  mSubTickCount(4),
  mAutoTickCount(6),
  mAutoTicks(true),
  mAutoTickStep(true),
  mAutoSubTicks(true),
  mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedTickPen(QPen(Qt::blue, 2)),
  mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedSubTickPen(QPen(Qt::blue, 2)),
  // scale and range:
  mRange(0, 5),
  mRangeReversed(false),
  mScaleType(stLinear),
  mScaleLogBase(10),
  mScaleLogBaseLogInv(1.0/qLn(mScaleLogBase)),
  // internal members:
  mGrid(new QCPGrid(this)),
  mAxisPainter(new QCPAxisPainterPrivate(parent->parentPlot())),
  mLowestVisibleTick(0),
  mHighestVisibleTick(-1),
  mCachedMarginValid(false),
  mCachedMargin(0)
{
  mGrid->setVisible(false);
  setAntialiased(false);
  setLayer(mParentPlot->currentLayer()); // it's actually on that layer already, but we want it in front of the grid, so we place it on there again
  
  if (type == atTop)
  {
    setTickLabelPadding(3);
    setLabelPadding(6);
  } else if (type == atRight)
  {
    setTickLabelPadding(7);
    setLabelPadding(12);
  } else if (type == atBottom)
  {
    setTickLabelPadding(3);
    setLabelPadding(3);
  } else if (type == atLeft)
  {
    setTickLabelPadding(5);
    setLabelPadding(10);
  }
}

QCPAxis::~QCPAxis()
{
  delete mAxisPainter;
  delete mGrid; // delete grid here instead of via parent ~QObject for better defined deletion order
}

/* No documentation as it is a property getter */
int QCPAxis::tickLabelPadding() const
{
  return mAxisPainter->tickLabelPadding;
}

/* No documentation as it is a property getter */
double QCPAxis::tickLabelRotation() const
{
  return mAxisPainter->tickLabelRotation;
}

/* No documentation as it is a property getter */
QCPAxis::LabelSide QCPAxis::tickLabelSide() const
{
  return mAxisPainter->tickLabelSide;
}

/* No documentation as it is a property getter */
QString QCPAxis::numberFormat() const
{
  QString result;
  result.append(mNumberFormatChar);
  if (mNumberBeautifulPowers)
  {
    result.append(QLatin1Char('b'));
    if (mAxisPainter->numberMultiplyCross)
      result.append(QLatin1Char('c'));
  }
  return result;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthIn() const
{
  return mAxisPainter->tickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthOut() const
{
  return mAxisPainter->tickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthIn() const
{
  return mAxisPainter->subTickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthOut() const
{
  return mAxisPainter->subTickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::labelPadding() const
{
  return mAxisPainter->labelPadding;
}

/* No documentation as it is a property getter */
int QCPAxis::offset() const
{
  return mAxisPainter->offset;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::lowerEnding() const
{
  return mAxisPainter->lowerEnding;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::upperEnding() const
{
  return mAxisPainter->upperEnding;
}

/*!
  Sets whether the axis uses a linear scale or a logarithmic scale. If \a type is set to \ref
  stLogarithmic, the logarithm base can be set with \ref setScaleLogBase. In logarithmic axis
  scaling, major tick marks appear at all powers of the logarithm base. Properties like tick step
  (\ref setTickStep) don't apply in logarithmic scaling. If you wish a decimal base but less major
  ticks, consider choosing a logarithm base of 100, 1000 or even higher.
  
  If \a type is \ref stLogarithmic and the number format (\ref setNumberFormat) uses the 'b' option
  (beautifully typeset decimal powers), the display usually is "1 [multiplication sign] 10
  [superscript] n", which looks unnatural for logarithmic scaling (the "1 [multiplication sign]"
  part). To only display the decimal power, set the number precision to zero with
  \ref setNumberPrecision.
*/
void QCPAxis::setScaleType(QCPAxis::ScaleType type)
{
  if (mScaleType != type)
  {
    mScaleType = type;
    if (mScaleType == stLogarithmic)
      setRange(mRange.sanitizedForLogScale());
    mCachedMarginValid = false;
    emit scaleTypeChanged(mScaleType);
  }
}

/*!
  If \ref setScaleType is set to \ref stLogarithmic, \a base will be the logarithm base of the
  scaling. In logarithmic axis scaling, major tick marks appear at all powers of \a base.
  
  Properties like tick step (\ref setTickStep) don't apply in logarithmic scaling. If you wish a decimal base but
  less major ticks, consider choosing \a base 100, 1000 or even higher.
*/
void QCPAxis::setScaleLogBase(double base)
{
  if (base > 1)
  {
    mScaleLogBase = base;
    mScaleLogBaseLogInv = 1.0/qLn(mScaleLogBase); // buffer for faster baseLog() calculation
    mCachedMarginValid = false;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid logarithmic scale base (must be greater 1):" << base;
}

/*!
  Sets the range of the axis.
  
  This slot may be connected with the \ref rangeChanged signal of another axis so this axis
  is always synchronized with the other axis range, when it changes.
  
  To invert the direction of an axis, use \ref setRangeReversed.
*/
void QCPAxis::setRange(const QCPRange &range)
{
  if (range.lower == mRange.lower && range.upper == mRange.upper)
    return;
  
  if (!QCPRange::validRange(range)) return;
  QCPRange oldRange = mRange;
  if (mScaleType == stLogarithmic)
  {
    mRange = range.sanitizedForLogScale();
  } else
  {
    mRange = range.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectAxes.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.
  
  \see SelectablePart, setSelectedParts
*/
void QCPAxis::setSelectableParts(const SelectableParts &selectable)
{
  if (mSelectableParts != selectable)
  {
    mSelectableParts = selectable;
    emit selectableChanged(mSelectableParts);
  }
}

/*!
  Sets the selected state of the respective axis parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font.
  
  The entire selection mechanism for axes is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectAxes. You only need to call this function when you
  wish to change the selection state manually.
  
  This function can change the selection state of a part, independent of the \ref setSelectableParts setting.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen, setSelectedTickPen, setSelectedSubTickPen,
  setSelectedTickLabelFont, setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
void QCPAxis::setSelectedParts(const SelectableParts &selected)
{
  if (mSelectedParts != selected)
  {
    mSelectedParts = selected;
    emit selectionChanged(mSelectedParts);
  }
}

/*!
  \overload
  
  Sets the lower and upper bound of the axis range.
  
  To invert the direction of an axis, use \ref setRangeReversed.
  
  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
void QCPAxis::setRange(double lower, double upper)
{
  if (lower == mRange.lower && upper == mRange.upper)
    return;
  
  if (!QCPRange::validRange(lower, upper)) return;
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  \overload
  
  Sets the range of the axis.
  
  The \a position coordinate indicates together with the \a alignment parameter, where the new
  range will be positioned. \a size defines the size of the new axis range. \a alignment may be
  Qt::AlignLeft, Qt::AlignRight or Qt::AlignCenter. This will cause the left border, right border,
  or center of the range to be aligned with \a position. Any other values of \a alignment will
  default to Qt::AlignCenter.
*/
void QCPAxis::setRange(double position, double size, Qt::AlignmentFlag alignment)
{
  if (alignment == Qt::AlignLeft)
    setRange(position, position+size);
  else if (alignment == Qt::AlignRight)
    setRange(position-size, position);
  else // alignment == Qt::AlignCenter
    setRange(position-size/2.0, position+size/2.0);
}

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeLower(double lower)
{
  if (mRange.lower == lower)
    return;
  
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeUpper(double upper)
{
  if (mRange.upper == upper)
    return;
  
  QCPRange oldRange = mRange;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the values on horizontal
  axes increase left to right, on vertical axes bottom to top. When \a reversed is set to true, the
  direction of increasing values is inverted.

  Note that the range and data interface stays the same for reversed axes, e.g. the \a lower part
  of the \ref setRange interface will still reference the mathematically smaller number than the \a
  upper part.
*/
void QCPAxis::setRangeReversed(bool reversed)
{
  if (mRangeReversed != reversed)
  {
    mRangeReversed = reversed;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick positions should be calculated automatically (either from an automatically
  generated tick step or a tick step provided manually via \ref setTickStep, see \ref setAutoTickStep).
  
  If \a on is set to false, you must provide the tick positions manually via \ref setTickVector.
  For these manual ticks you may let QCPAxis generate the appropriate labels automatically by
  leaving \ref setAutoTickLabels set to true. If you also wish to control the displayed labels
  manually, set \ref setAutoTickLabels to false and provide the label strings with \ref
  setTickVectorLabels.
  
  If you need dynamically calculated tick vectors (and possibly tick label vectors), set the
  vectors in a slot connected to the \ref ticksRequest signal.
  
  \see setAutoTickLabels, setAutoSubTicks, setAutoTickCount, setAutoTickStep
*/
void QCPAxis::setAutoTicks(bool on)
{
  if (mAutoTicks != on)
  {
    mAutoTicks = on;
    mCachedMarginValid = false;
  }
}

/*!
  When \ref setAutoTickStep is true, \a approximateCount determines how many ticks should be
  generated in the visible range, approximately.
  
  It's not guaranteed that this number of ticks is met exactly, but approximately within a
  tolerance of about two.
  
  Only values greater than zero are accepted as \a approximateCount.
  
  \see setAutoTickStep, setAutoTicks, setAutoSubTicks
*/
void QCPAxis::setAutoTickCount(int approximateCount)
{
  if (mAutoTickCount != approximateCount)
  {
    if (approximateCount > 0)
    {
      mAutoTickCount = approximateCount;
      mCachedMarginValid = false;
    } else
      qDebug() << Q_FUNC_INFO << "approximateCount must be greater than zero:" << approximateCount;
  }
}

/*!
  Sets whether the tick labels are generated automatically. Depending on the tick label type (\ref
  ltNumber or \ref ltDateTime), the labels will either show the coordinate as floating point
  number (\ref setNumberFormat), or a date/time formatted according to \ref setDateTimeFormat.
  
  If \a on is set to false, you should provide the tick labels via \ref setTickVectorLabels. This
  is usually used in a combination with \ref setAutoTicks set to false for complete control over
  tick positions and labels, e.g. when the ticks should be at multiples of pi and show "2pi", "3pi"
  etc. as tick labels.
  
  If you need dynamically calculated tick vectors (and possibly tick label vectors), set the
  vectors in a slot connected to the \ref ticksRequest signal.
  
  \see setAutoTicks
*/
void QCPAxis::setAutoTickLabels(bool on)
{
  if (mAutoTickLabels != on)
  {
    mAutoTickLabels = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick step, i.e. the interval between two (major) ticks, is calculated
  automatically. If \a on is set to true, the axis finds a tick step that is reasonable for human
  readable plots.

  The number of ticks the algorithm aims for within the visible range can be specified with \ref
  setAutoTickCount.
  
  If \a on is set to false, you may set the tick step manually with \ref setTickStep.
  
  \see setAutoTicks, setAutoSubTicks, setAutoTickCount
*/
void QCPAxis::setAutoTickStep(bool on)
{
  if (mAutoTickStep != on)
  {
    mAutoTickStep = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the number of sub ticks in one tick interval is determined automatically. This
  works, as long as the tick step mantissa is a multiple of 0.5. When \ref setAutoTickStep is
  enabled, this is always the case.
  
  When \a on is set to false, you may set the sub tick count with \ref setSubTickCount manually.
  
  \see setAutoTickCount, setAutoTicks, setAutoTickStep
*/
void QCPAxis::setAutoSubTicks(bool on)
{
  if (mAutoSubTicks != on)
  {
    mAutoSubTicks = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are invisible, too. To achieve
  that, see \ref setTickLabels.
*/
void QCPAxis::setTicks(bool show)
{
  if (mTicks != show)
  {
    mTicks = show;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next to tick marks.
*/
void QCPAxis::setTickLabels(bool show)
{
  if (mTickLabels != show)
  {
    mTickLabels = show;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the axis base line (including any outward ticks) and the tick labels.
  \see setLabelPadding, setPadding
*/
void QCPAxis::setTickLabelPadding(int padding)
{
  if (mAxisPainter->tickLabelPadding != padding)
  {
    mAxisPainter->tickLabelPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick labels display numbers or dates/times.
  
  If \a type is set to \ref ltNumber, the format specifications of \ref setNumberFormat apply.
  
  If \a type is set to \ref ltDateTime, the format specifications of \ref setDateTimeFormat apply.
  
  In QCustomPlot, date/time coordinates are <tt>double</tt> numbers representing the seconds since
  1970-01-01T00:00:00 UTC. This format can be retrieved from QDateTime objects with the
  QDateTime::toTime_t() function. Since this only gives a resolution of one second, there is also
  the QDateTime::toMSecsSinceEpoch() function which returns the timespan described above in
  milliseconds. Divide its return value by 1000.0 to get a value with the format needed for
  date/time plotting, with a resolution of one millisecond.
  
  Using the toMSecsSinceEpoch function allows dates that go back to 2nd January 4713 B.C.
  (represented by a negative number), unlike the toTime_t function, which works with unsigned
  integers and thus only goes back to 1st January 1970. So both for range and accuracy, use of
  toMSecsSinceEpoch()/1000.0 should be preferred as key coordinate for date/time axes.
  
  \see setTickLabels
*/
void QCPAxis::setTickLabelType(LabelType type)
{
  if (mTickLabelType != type)
  {
    mTickLabelType = type;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the font of the tick labels.
  
  \see setTickLabels, setTickLabelColor
*/
void QCPAxis::setTickLabelFont(const QFont &font)
{
  if (font != mTickLabelFont)
  {
    mTickLabelFont = font;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the tick labels.
  
  \see setTickLabels, setTickLabelFont
*/
void QCPAxis::setTickLabelColor(const QColor &color)
{
  if (color != mTickLabelColor)
  {
    mTickLabelColor = color;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are drawn normally. Else,
  the tick labels are drawn rotated by \a degrees clockwise. The specified angle is bound to values
  from -90 to 90 degrees.
  
  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the tick coordinate. For
  other angles, the label is drawn with an offset such that it seems to point toward or away from
  the tick mark.
*/
void QCPAxis::setTickLabelRotation(double degrees)
{
  if (!qFuzzyIsNull(degrees-mAxisPainter->tickLabelRotation))
  {
    mAxisPainter->tickLabelRotation = qBound(-90.0, degrees, 90.0);
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick labels (numbers) shall appear inside or outside the axis rect.
  
  The usual and default setting is \ref lsOutside. Very compact plots sometimes require tick labels
  to be inside the axis rect, to save space. If \a side is set to \ref lsInside, the tick labels
  appear on the inside are additionally clipped to the axis rect.
*/
void QCPAxis::setTickLabelSide(LabelSide side)
{
  mAxisPainter->tickLabelSide = side;
  mCachedMarginValid = false;
}

/*!
  Sets the format in which dates and times are displayed as tick labels, if \ref setTickLabelType is \ref ltDateTime.
  for details about the \a format string, see the documentation of QDateTime::toString().
  
  Newlines can be inserted with "\n".
  
  \see setDateTimeSpec
*/
void QCPAxis::setDateTimeFormat(const QString &format)
{
  if (mDateTimeFormat != format)
  {
    mDateTimeFormat = format;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the time spec that is used for the date time values when \ref setTickLabelType is \ref
  ltDateTime.

  The default value of QDateTime objects (and also QCustomPlot) is <tt>Qt::LocalTime</tt>. However,
  if the date time values passed to QCustomPlot are given in the UTC spec, set \a
  timeSpec to <tt>Qt::UTC</tt> to get the correct axis labels.
  
  \see setDateTimeFormat
*/
void QCPAxis::setDateTimeSpec(const Qt::TimeSpec &timeSpec)
{
  mDateTimeSpec = timeSpec;
}

/*!
  Sets the number format for the numbers drawn as tick labels (if tick label type is \ref
  ltNumber). This \a formatCode is an extended version of the format code used e.g. by
  QString::number() and QLocale::toString(). For reference about that, see the "Argument Formats"
  section in the detailed description of the QString class. \a formatCode is a string of one, two
  or three characters. The first character is identical to the normal format code used by Qt. In
  short, this means: 'e'/'E' scientific format, 'f' fixed format, 'g'/'G' scientific or fixed,
  whichever is shorter.
  
  The second and third characters are optional and specific to QCustomPlot:\n
  If the first char was 'e' or 'g', numbers are/might be displayed in the scientific format, e.g.
  "5.5e9", which is ugly in a plot. So when the second char of \a formatCode is set to 'b' (for
  "beautiful"), those exponential numbers are formatted in a more natural way, i.e. "5.5
  [multiplication sign] 10 [superscript] 9". By default, the multiplication sign is a centered dot.
  If instead a cross should be shown (as is usual in the USA), the third char of \a formatCode can
  be set to 'c'. The inserted multiplication signs are the UTF-8 characters 215 (0xD7) for the
  cross and 183 (0xB7) for the dot.
  
  If the scale type (\ref setScaleType) is \ref stLogarithmic and the \a formatCode uses the 'b'
  option (beautifully typeset decimal powers), the display usually is "1 [multiplication sign] 10
  [superscript] n", which looks unnatural for logarithmic scaling (the "1 [multiplication sign]"
  part). To only display the decimal power, set the number precision to zero with \ref
  setNumberPrecision.
  
  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is used, if number is large,
  normal scientific format is used
  \li \c gb If number is small, fixed format is used, if number is large, scientific format is used with
  beautifully typeset decimal powers and a dot as multiplication sign
  \li \c ebc All numbers are in scientific format with beautifully typeset decimal power and a cross as
  multiplication sign
  \li \c fb illegal format code, since fixed format doesn't support (or need) beautifully typeset decimal
  powers. Format code will be reduced to 'f'.
  \li \c hello illegal format code, since first char is not 'e', 'E', 'f', 'g' or 'G'. Current format
  code will not be changed.
*/
void QCPAxis::setNumberFormat(const QString &formatCode)
{
  if (formatCode.isEmpty())
  {
    qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
    return;
  }
  mCachedMarginValid = false;
  
  // interpret first char as number format char:
  QString allowedFormatChars(QLatin1String("eEfgG"));
  if (allowedFormatChars.contains(formatCode.at(0)))
  {
    mNumberFormatChar = QLatin1Char(formatCode.at(0).toLatin1());
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (first char not in 'eEfgG'):" << formatCode;
    return;
  }
  if (formatCode.length() < 2)
  {
    mNumberBeautifulPowers = false;
    mAxisPainter->numberMultiplyCross = false;
    return;
  }
  
  // interpret second char as indicator for beautiful decimal powers:
  if (formatCode.at(1) == QLatin1Char('b') && (mNumberFormatChar == QLatin1Char('e') || mNumberFormatChar == QLatin1Char('g')))
  {
    mNumberBeautifulPowers = true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (second char not 'b' or first char neither 'e' nor 'g'):" << formatCode;
    return;
  }
  if (formatCode.length() < 3)
  {
    mAxisPainter->numberMultiplyCross = false;
    return;
  }
  
  // interpret third char as indicator for dot or cross multiplication symbol:
  if (formatCode.at(2) == QLatin1Char('c'))
  {
    mAxisPainter->numberMultiplyCross = true;
  } else if (formatCode.at(2) == QLatin1Char('d'))
  {
    mAxisPainter->numberMultiplyCross = false;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (third char neither 'c' nor 'd'):" << formatCode;
    return;
  }
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i, char f, int prec)
  for details. The effect of precisions are most notably for number Formats starting with 'e', see
  \ref setNumberFormat

  If the scale type (\ref setScaleType) is \ref stLogarithmic and the number format (\ref
  setNumberFormat) uses the 'b' format code (beautifully typeset decimal powers), the display
  usually is "1 [multiplication sign] 10 [superscript] n", which looks unnatural for logarithmic
  scaling (the redundant "1 [multiplication sign]" part). To only display the decimal power "10
  [superscript] n", set \a precision to zero.
*/
void QCPAxis::setNumberPrecision(int precision)
{
  if (mNumberPrecision != precision)
  {
    mNumberPrecision = precision;
    mCachedMarginValid = false;
  }
}

/*!
  If \ref setAutoTickStep is set to false, use this function to set the tick step manually.
  The tick step is the interval between (major) ticks, in plot coordinates.
  \see setSubTickCount
*/
void QCPAxis::setTickStep(double step)
{
  if (mTickStep != step)
  {
    mTickStep = step;
    mCachedMarginValid = false;
  }
}

/*!
  If you want full control over what ticks (and possibly labels) the axes show, this function is
  used to set the coordinates at which ticks will appear.\ref setAutoTicks must be disabled, else
  the provided tick vector will be overwritten with automatically generated tick coordinates upon
  replot. The labels of the ticks can be generated automatically when \ref setAutoTickLabels is
  left enabled. If it is disabled, you can set the labels manually with \ref setTickVectorLabels.
  
  \a vec is a vector containing the positions of the ticks, in plot coordinates.
  
  \warning \a vec must be sorted in ascending order, no additional checks are made to ensure this.

  \see setTickVectorLabels
*/
void QCPAxis::setTickVector(const QVector<double> &vec)
{
  // don't check whether mTickVector != vec here, because it takes longer than we would save
  mTickVector = vec;
  mCachedMarginValid = false;
}

/*!
  If you want full control over what ticks and labels the axes show, this function is used to set a
  number of QStrings that will be displayed at the tick positions which you need to provide with
  \ref setTickVector. These two vectors should have the same size. (Note that you need to disable
  \ref setAutoTicks and \ref setAutoTickLabels first.)
  
  \a vec is a vector containing the labels of the ticks. The entries correspond to the respective
  indices in the tick vector, passed via \ref setTickVector.
  
  \see setTickVector
*/
void QCPAxis::setTickVectorLabels(const QVector<QString> &vec)
{
  // don't check whether mTickVectorLabels != vec here, because it takes longer than we would save
  mTickVectorLabels = vec;
  mCachedMarginValid = false;
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will reach inside the
  plot and \a outside is the length they will reach outside the plot. If \a outside is greater than
  zero, the tick labels and axis label will increase their distance to the axis accordingly, so
  they won't collide with the ticks.
  
  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPAxis::setTickLength(int inside, int outside)
{
  setTickLengthIn(inside);
  setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the ticks will reach
  inside the plot.
  
  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthIn(int inside)
{
  if (mAxisPainter->tickLengthIn != inside)
  {
    mAxisPainter->tickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the ticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthOut(int outside)
{
  if (mAxisPainter->tickLengthOut != outside)
  {
    mAxisPainter->tickLengthOut = outside;
    mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the number of sub ticks in one (major) tick step. A sub tick count of three for example,
  divides the tick intervals in four sub intervals.
  
  By default, the number of sub ticks is chosen automatically in a reasonable manner as long as the
  mantissa of the tick step is a multiple of 0.5. When \ref setAutoTickStep is enabled, this is
  always the case.

  If you want to disable automatic sub tick count and use this function to set the count manually,
  see \ref setAutoSubTicks.
*/
void QCPAxis::setSubTickCount(int count)
{
  mSubTickCount = count;
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the subticks will reach inside
  the plot and \a outside is the length they will reach outside the plot. If \a outside is greater
  than zero, the tick labels and axis label will increase their distance to the axis accordingly,
  so they won't collide with the ticks.
  
  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPAxis::setSubTickLength(int inside, int outside)
{
  setSubTickLengthIn(inside);
  setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the subticks will reach inside
  the plot.
  
  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthIn(int inside)
{
  if (mAxisPainter->subTickLengthIn != inside)
  {
    mAxisPainter->subTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length the subticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels will increase their
  distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthOut(int outside)
{
  if (mAxisPainter->subTickLengthOut != outside)
  {
    mAxisPainter->subTickLengthOut = outside;
    mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the pen, the axis base line is drawn with.
  
  \see setTickPen, setSubTickPen
*/
void QCPAxis::setBasePen(const QPen &pen)
{
  mBasePen = pen;
}

/*!
  Sets the pen, tick marks will be drawn with.
  
  \see setTickLength, setBasePen
*/
void QCPAxis::setTickPen(const QPen &pen)
{
  mTickPen = pen;
}

/*!
  Sets the pen, subtick marks will be drawn with.
  
  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPAxis::setSubTickPen(const QPen &pen)
{
  mSubTickPen = pen;
}

/*!
  Sets the font of the axis label.
  
  \see setLabelColor
*/
void QCPAxis::setLabelFont(const QFont &font)
{
  if (mLabelFont != font)
  {
    mLabelFont = font;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the axis label.
  
  \see setLabelFont
*/
void QCPAxis::setLabelColor(const QColor &color)
{
  mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the axis, depending on
  its orientation. To disable axis labels, pass an empty string as \a str.
*/
void QCPAxis::setLabel(const QString &str)
{
  if (mLabel != str)
  {
    mLabel = str;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the tick labels and the axis label.
  
  \see setTickLabelPadding, setPadding
*/
void QCPAxis::setLabelPadding(int padding)
{
  if (mAxisPainter->labelPadding != padding)
  {
    mAxisPainter->labelPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the padding of the axis.

  When \ref QCPAxisRect::setAutoMargins is enabled, the padding is the additional outer most space,
  that is left blank.
  
  The axis padding has no meaning if \ref QCPAxisRect::setAutoMargins is disabled.
  
  \see setLabelPadding, setTickLabelPadding
*/
void QCPAxis::setPadding(int padding)
{
  if (mPadding != padding)
  {
    mPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the offset the axis has to its axis rect side.
  
  If an axis rect side has multiple axes and automatic margin calculation is enabled for that side,
  only the offset of the inner most axis has meaning (even if it is set to be invisible). The
  offset of the other, outer axes is controlled automatically, to place them at appropriate
  positions.
*/
void QCPAxis::setOffset(int offset)
{
  mAxisPainter->offset = offset;
}

/*!
  Sets the font that is used for tick labels when they are selected.
  
  \see setTickLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelFont(const QFont &font)
{
  if (font != mSelectedTickLabelFont)
  {
    mSelectedTickLabelFont = font;
    // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
  }
}

/*!
  Sets the font that is used for the axis label when it is selected.
  
  \see setLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelFont(const QFont &font)
{
  mSelectedLabelFont = font;
  // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.
  
  \see setTickLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelColor(const QColor &color)
{
  if (color != mSelectedTickLabelColor)
  {
    mSelectedTickLabelColor = color;
  }
}

/*!
  Sets the color that is used for the axis label when it is selected.
  
  \see setLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelColor(const QColor &color)
{
  mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.
  
  \see setBasePen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedBasePen(const QPen &pen)
{
  mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.
  
  \see setTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickPen(const QPen &pen)
{
  mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.
  
  \see setSubTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedSubTickPen(const QPen &pen)
{
  mSelectedSubTickPen = pen;
}

/*!
  Sets the style for the lower axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the left ending, for vertical axes the bottom ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setUpperEnding
*/
void QCPAxis::setLowerEnding(const QCPLineEnding &ending)
{
  mAxisPainter->lowerEnding = ending;
}

/*!
  Sets the style for the upper axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the right ending, for vertical axes the top ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setLowerEnding
*/
void QCPAxis::setUpperEnding(const QCPLineEnding &ending)
{
  mAxisPainter->upperEnding = ending;
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to the lower and upper
  bounds of the range. The range is simply moved by \a diff.
  
  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a diff. This
  corresponds to an apparent "linear" move in logarithmic scaling by a distance of log(diff).
*/
void QCPAxis::moveRange(double diff)
{
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear)
  {
    mRange.lower += diff;
    mRange.upper += diff;
  } else // mScaleType == stLogarithmic
  {
    mRange.lower *= diff;
    mRange.upper *= diff;
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the coordinate \a center. For example, if \a
  factor is 2.0, \a center is 1.0, then the axis range will double its size, and the point at
  coordinate 1.0 won't have changed its position in the QCustomPlot widget (i.e. coordinates
  around 1.0 will have moved symmetrically closer to 1.0).
*/
void QCPAxis::scaleRange(double factor, double center)
{
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear)
  {
    QCPRange newRange;
    newRange.lower = (mRange.lower-center)*factor + center;
    newRange.upper = (mRange.upper-center)*factor + center;
    if (QCPRange::validRange(newRange))
      mRange = newRange.sanitizedForLinScale();
  } else // mScaleType == stLogarithmic
  {
    if ((mRange.upper < 0 && center < 0) || (mRange.upper > 0 && center > 0)) // make sure center has same sign as range
    {
      QCPRange newRange;
      newRange.lower = qPow(mRange.lower/center, factor)*center;
      newRange.upper = qPow(mRange.upper/center, factor)*center;
      if (QCPRange::validRange(newRange))
        mRange = newRange.sanitizedForLogScale();
    } else
      qDebug() << Q_FUNC_INFO << "Center of scaling operation doesn't lie in same logarithmic sign domain as range:" << center;
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis to have a certain scale \a ratio to \a otherAxis. The scaling will
  be done around the center of the current axis range.

  For example, if \a ratio is 1, this axis is the \a yAxis and \a otherAxis is \a xAxis, graphs
  plotted with those axes will appear in a 1:1 aspect ratio, independent of the aspect ratio the
  axis rect has.

  This is an operation that changes the range of this axis once, it doesn't fix the scale ratio
  indefinitely. Note that calling this function in the constructor of the QCustomPlot's parent
  won't have the desired effect, since the widget dimensions aren't defined yet, and a resizeEvent
  will follow.
*/
void QCPAxis::setScaleRatio(const QCPAxis *otherAxis, double ratio)
{
  int otherPixelSize, ownPixelSize;
  
  if (otherAxis->orientation() == Qt::Horizontal)
    otherPixelSize = otherAxis->axisRect()->width();
  else
    otherPixelSize = otherAxis->axisRect()->height();
  
  if (orientation() == Qt::Horizontal)
    ownPixelSize = axisRect()->width();
  else
    ownPixelSize = axisRect()->height();
  
  double newRangeSize = ratio*otherAxis->range().size()*ownPixelSize/(double)otherPixelSize;
  setRange(range().center(), newRangeSize, Qt::AlignCenter);
}

/*!
  Changes the axis range such that all plottables associated with this axis are fully visible in
  that dimension.
  
  \see QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPAxis::rescale(bool onlyVisiblePlottables)
{
  QList<QCPAbstractPlottable*> p = plottables();
  QCPRange newRange;
  bool haveRange = false;
  for (int i=0; i<p.size(); ++i)
  {
    if (!p.at(i)->realVisibility() && onlyVisiblePlottables)
      continue;
    QCPRange plottableRange;
    bool currentFoundRange;
    QCPAbstractPlottable::SignDomain signDomain = QCPAbstractPlottable::sdBoth;
    if (mScaleType == stLogarithmic)
      signDomain = (mRange.upper < 0 ? QCPAbstractPlottable::sdNegative : QCPAbstractPlottable::sdPositive);
    if (p.at(i)->keyAxis() == this)
      plottableRange = p.at(i)->getKeyRange(currentFoundRange, signDomain);
    else
      plottableRange = p.at(i)->getValueRange(currentFoundRange, signDomain);
    if (currentFoundRange)
    {
      if (!haveRange)
        newRange = plottableRange;
      else
        newRange.expand(plottableRange);
      haveRange = true;
    }
  }
  if (haveRange)
  {
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (mScaleType == stLinear)
      {
        newRange.lower = center-mRange.size()/2.0;
        newRange.upper = center+mRange.size()/2.0;
      } else // mScaleType == stLogarithmic
      {
        newRange.lower = center/qSqrt(mRange.upper/mRange.lower);
        newRange.upper = center*qSqrt(mRange.upper/mRange.lower);
      }
    }
    setRange(newRange);
  }
}

/*!
  Transforms \a value, in pixel coordinates of the QCustomPlot widget, to axis coordinates.
*/
double QCPAxis::pixelToCoord(double value) const
{
  if (orientation() == Qt::Horizontal)
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.lower;
      else
        return -(value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.upper;
    } else // mScaleType == stLogarithmic
    {
      if (!mRangeReversed)
        return qPow(mRange.upper/mRange.lower, (value-mAxisRect->left())/(double)mAxisRect->width())*mRange.lower;
      else
        return qPow(mRange.upper/mRange.lower, (mAxisRect->left()-value)/(double)mAxisRect->width())*mRange.upper;
    }
  } else // orientation() == Qt::Vertical
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.lower;
      else
        return -(mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.upper;
    } else // mScaleType == stLogarithmic
    {
      if (!mRangeReversed)
        return qPow(mRange.upper/mRange.lower, (mAxisRect->bottom()-value)/(double)mAxisRect->height())*mRange.lower;
      else
        return qPow(mRange.upper/mRange.lower, (value-mAxisRect->bottom())/(double)mAxisRect->height())*mRange.upper;
    }
  }
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the QCustomPlot widget.
*/
double QCPAxis::coordToPixel(double value) const
{
  if (orientation() == Qt::Horizontal)
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (value-mRange.lower)/mRange.size()*mAxisRect->width()+mAxisRect->left();
      else
        return (mRange.upper-value)/mRange.size()*mAxisRect->width()+mAxisRect->left();
    } else // mScaleType == stLogarithmic
    {
      if (value >= 0 && mRange.upper < 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->right()+200 : mAxisRect->left()-200;
      else if (value <= 0 && mRange.upper > 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->left()-200 : mAxisRect->right()+200;
      else
      {
        if (!mRangeReversed)
          return baseLog(value/mRange.lower)/baseLog(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
        else
          return baseLog(mRange.upper/value)/baseLog(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
      }
    }
  } else // orientation() == Qt::Vertical
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return mAxisRect->bottom()-(value-mRange.lower)/mRange.size()*mAxisRect->height();
      else
        return mAxisRect->bottom()-(mRange.upper-value)/mRange.size()*mAxisRect->height();
    } else // mScaleType == stLogarithmic
    {
      if (value >= 0 && mRange.upper < 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->top()-200 : mAxisRect->bottom()+200;
      else if (value <= 0 && mRange.upper > 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->bottom()+200 : mAxisRect->top()-200;
      else
      {
        if (!mRangeReversed)
          return mAxisRect->bottom()-baseLog(value/mRange.lower)/baseLog(mRange.upper/mRange.lower)*mAxisRect->height();
        else
          return mAxisRect->bottom()-baseLog(mRange.upper/value)/baseLog(mRange.upper/mRange.lower)*mAxisRect->height();
      }
    }
  }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return value of this function
  is independent of the user-selectable parts defined with \ref setSelectableParts. Further, this
  function does not change the current selection state of the axis.
  
  If the axis is not visible (\ref setVisible), this function always returns \ref spNone.
  
  \see setSelectedParts, setSelectableParts, QCustomPlot::setInteractions
*/
QCPAxis::SelectablePart QCPAxis::getPartAt(const QPointF &pos) const
{
  if (!mVisible)
    return spNone;
  
  if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
    return spAxis;
  else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
    return spTickLabels;
  else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
    return spAxisLabel;
  else
    return spNone;
}

/* inherits documentation from base class */
double QCPAxis::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  if (!mParentPlot) return -1;
  SelectablePart part = getPartAt(pos);
  if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
    return -1;
  
  if (details)
    details->setValue(part);
  return mParentPlot->selectionTolerance()*0.99;
}

/*!
  Returns a list of all the plottables that have this axis as key or value axis.
  
  If you are only interested in plottables of type QCPGraph, see \ref graphs.
  
  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxis::plottables() const
{
  QList<QCPAbstractPlottable*> result;
  if (!mParentPlot) return result;
  
  for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
  {
    if (mParentPlot->mPlottables.at(i)->keyAxis() == this ||mParentPlot->mPlottables.at(i)->valueAxis() == this)
      result.append(mParentPlot->mPlottables.at(i));
  }
  return result;
}

/*!
  Returns a list of all the graphs that have this axis as key or value axis.
  
  \see plottables, items
*/
QList<QCPGraph*> QCPAxis::graphs() const
{
  QList<QCPGraph*> result;
  if (!mParentPlot) return result;
  
  for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
  {
    if (mParentPlot->mGraphs.at(i)->keyAxis() == this || mParentPlot->mGraphs.at(i)->valueAxis() == this)
      result.append(mParentPlot->mGraphs.at(i));
  }
  return result;
}

/*!
  Returns a list of all the items that are associated with this axis. An item is considered
  associated with an axis if at least one of its positions uses the axis as key or value axis.
  
  \see plottables, graphs
*/
QList<QCPAbstractItem*> QCPAxis::items() const
{
  QList<QCPAbstractItem*> result;
  if (!mParentPlot) return result;
  
  for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
  {
    QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
    for (int posId=0; posId<positions.size(); ++posId)
    {
      if (positions.at(posId)->keyAxis() == this || positions.at(posId)->valueAxis() == this)
      {
        result.append(mParentPlot->mItems.at(itemId));
        break;
      }
    }
  }
  return result;
}

/*!
  Transforms a margin side to the logically corresponding axis type. (QCP::msLeft to
  QCPAxis::atLeft, QCP::msRight to QCPAxis::atRight, etc.)
*/
QCPAxis::AxisType QCPAxis::marginSideToAxisType(QCP::MarginSide side)
{
  switch (side)
  {
    case QCP::msLeft: return atLeft;
    case QCP::msRight: return atRight;
    case QCP::msTop: return atTop;
    case QCP::msBottom: return atBottom;
    default: break;
  }
  qDebug() << Q_FUNC_INFO << "Invalid margin side passed:" << (int)side;
  return atLeft;
}

/*!
  Returns the axis type that describes the opposite axis of an axis with the specified \a type.
*/
QCPAxis::AxisType QCPAxis::opposite(QCPAxis::AxisType type)
{
  switch (type)
  {
    case atLeft: return atRight; break;
    case atRight: return atLeft; break;
    case atBottom: return atTop; break;
    case atTop: return atBottom; break;
    default: qDebug() << Q_FUNC_INFO << "invalid axis type"; return atLeft; break;
  }
}

/*! \internal
  
  This function is called to prepare the tick vector, sub tick vector and tick label vector. If
  \ref setAutoTicks is set to true, appropriate tick values are determined automatically via \ref
  generateAutoTicks. If it's set to false, the signal ticksRequest is emitted, which can be used to
  provide external tick positions. Then the sub tick vectors and tick label vectors are created.
*/
void QCPAxis::setupTickVectors()
{
  if (!mParentPlot) return;
  if ((!mTicks && !mTickLabels && !mGrid->visible()) || mRange.size() <= 0) return;
  
  // fill tick vectors, either by auto generating or by notifying user to fill the vectors himself
  if (mAutoTicks)
  {
    generateAutoTicks();
  } else
  {
    emit ticksRequest();
  }
  
  visibleTickBounds(mLowestVisibleTick, mHighestVisibleTick);
  if (mTickVector.isEmpty())
  {
    mSubTickVector.clear();
    return;
  }
  
  // generate subticks between ticks:
  mSubTickVector.resize((mTickVector.size()-1)*mSubTickCount);
  if (mSubTickCount > 0)
  {
    double subTickStep = 0;
    double subTickPosition = 0;
    int subTickIndex = 0;
    bool done = false;
    int lowTick = mLowestVisibleTick > 0 ? mLowestVisibleTick-1 : mLowestVisibleTick;
    int highTick = mHighestVisibleTick < mTickVector.size()-1 ? mHighestVisibleTick+1 : mHighestVisibleTick;
    for (int i=lowTick+1; i<=highTick; ++i)
    {
      subTickStep = (mTickVector.at(i)-mTickVector.at(i-1))/(double)(mSubTickCount+1);
      for (int k=1; k<=mSubTickCount; ++k)
      {
        subTickPosition = mTickVector.at(i-1) + k*subTickStep;
        if (subTickPosition < mRange.lower)
          continue;
        if (subTickPosition > mRange.upper)
        {
          done = true;
          break;
        }
        mSubTickVector[subTickIndex] = subTickPosition;
        subTickIndex++;
      }
      if (done) break;
    }
    mSubTickVector.resize(subTickIndex);
  }

  // generate tick labels according to tick positions:
  if (mAutoTickLabels)
  {
    int vecsize = mTickVector.size();
    mTickVectorLabels.resize(vecsize);
    if (mTickLabelType == ltNumber)
    {
      for (int i=mLowestVisibleTick; i<=mHighestVisibleTick; ++i)
        mTickVectorLabels[i] = mParentPlot->locale().toString(mTickVector.at(i), mNumberFormatChar.toLatin1(), mNumberPrecision);
    } else if (mTickLabelType == ltDateTime)
    {
      for (int i=mLowestVisibleTick; i<=mHighestVisibleTick; ++i)
      {
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0) // use fromMSecsSinceEpoch function if available, to gain sub-second accuracy on tick labels (e.g. for format "hh:mm:ss:zzz")
        mTickVectorLabels[i] = mParentPlot->locale().toString(QDateTime::fromTime_t(mTickVector.at(i)).toTimeSpec(mDateTimeSpec), mDateTimeFormat);
#else
        mTickVectorLabels[i] = mParentPlot->locale().toString(QDateTime::fromMSecsSinceEpoch(mTickVector.at(i)*1000).toTimeSpec(mDateTimeSpec), mDateTimeFormat);
#endif
      }
    }
  } else // mAutoTickLabels == false
  {
    if (mAutoTicks) // ticks generated automatically, but not ticklabels, so emit ticksRequest here for labels
    {
      emit ticksRequest();
    }
    // make sure provided tick label vector has correct (minimal) length:
    if (mTickVectorLabels.size() < mTickVector.size())
      mTickVectorLabels.resize(mTickVector.size());
  }
}

/*! \internal
  
  If \ref setAutoTicks is set to true, this function is called by \ref setupTickVectors to
  generate reasonable tick positions (and subtick count). The algorithm tries to create
  approximately <tt>mAutoTickCount</tt> ticks (set via \ref setAutoTickCount).
 
  If the scale is logarithmic, \ref setAutoTickCount is ignored, and one tick is generated at every
  power of the current logarithm base, set via \ref setScaleLogBase.
*/
void QCPAxis::generateAutoTicks()
{
  if (mScaleType == stLinear)
  {
    if (mAutoTickStep)
    {
      // Generate tick positions according to linear scaling:
      mTickStep = mRange.size()/(double)(mAutoTickCount+1e-10); // mAutoTickCount ticks on average, the small addition is to prevent jitter on exact integers
      double magnitudeFactor = qPow(10.0, qFloor(qLn(mTickStep)/qLn(10.0))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
      double tickStepMantissa = mTickStep/magnitudeFactor;
      if (tickStepMantissa < 5)
      {
        // round digit after decimal point to 0.5
        mTickStep = (int)(tickStepMantissa*2)/2.0*magnitudeFactor;
      } else
      {
        // round to first digit in multiples of 2
        mTickStep = (int)(tickStepMantissa/2.0)*2.0*magnitudeFactor;
      }
    }
    if (mAutoSubTicks)
      mSubTickCount = calculateAutoSubTickCount(mTickStep);
    // Generate tick positions according to mTickStep:
    qint64 firstStep = floor(mRange.lower/mTickStep); // do not use qFloor here, or we'll lose 64 bit precision
    qint64 lastStep = ceil(mRange.upper/mTickStep); // do not use qCeil here, or we'll lose 64 bit precision
    int tickcount = lastStep-firstStep+1;
    if (tickcount < 0) tickcount = 0;
    mTickVector.resize(tickcount);
    for (int i=0; i<tickcount; ++i)
      mTickVector[i] = (firstStep+i)*mTickStep;
  } else // mScaleType == stLogarithmic
  {
    // Generate tick positions according to logbase scaling:
    if (mRange.lower > 0 && mRange.upper > 0) // positive range
    {
      double lowerMag = basePow(qFloor(baseLog(mRange.lower)));
      double currentMag = lowerMag;
      mTickVector.clear();
      mTickVector.append(currentMag);
      while (currentMag < mRange.upper && currentMag > 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
      {
        currentMag *= mScaleLogBase;
        mTickVector.append(currentMag);
      }
    } else if (mRange.lower < 0 && mRange.upper < 0) // negative range
    {
      double lowerMag = -basePow(qCeil(baseLog(-mRange.lower)));
      double currentMag = lowerMag;
      mTickVector.clear();
      mTickVector.append(currentMag);
      while (currentMag < mRange.upper && currentMag < 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
      {
        currentMag /= mScaleLogBase;
        mTickVector.append(currentMag);
      }
    } else // invalid range for logarithmic scale, because lower and upper have different sign
    {
      mTickVector.clear();
      qDebug() << Q_FUNC_INFO << "Invalid range for logarithmic plot: " << mRange.lower << "-" << mRange.upper;
    }
  }
}

/*! \internal
  
  Called by generateAutoTicks when \ref setAutoSubTicks is set to true. Depending on the \a
  tickStep between two major ticks on the axis, a different number of sub ticks is appropriate. For
  Example taking 4 sub ticks for a \a tickStep of 1 makes more sense than taking 5 sub ticks,
  because this corresponds to a sub tick step of 0.2, instead of the less intuitive 0.16667. Note
  that a subtick count of 4 means dividing the major tick step into 5 sections.
  
  This is implemented by a hand made lookup for integer tick steps as well as fractional tick steps
  with a fractional part of (approximately) 0.5. If a tick step is different (i.e. has no
  fractional part close to 0.5), the currently set sub tick count (\ref setSubTickCount) is
  returned.
*/
int QCPAxis::calculateAutoSubTickCount(double tickStep) const
{
  int result = mSubTickCount; // default to current setting, if no proper value can be found
  
  // get mantissa of tickstep:
  double magnitudeFactor = qPow(10.0, qFloor(qLn(tickStep)/qLn(10.0))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
  double tickStepMantissa = tickStep/magnitudeFactor;
  
  // separate integer and fractional part of mantissa:
  double epsilon = 0.01;
  double intPartf;
  int intPart;
  double fracPart = modf(tickStepMantissa, &intPartf);
  intPart = intPartf;
  
  // handle cases with (almost) integer mantissa:
  if (fracPart < epsilon || 1.0-fracPart < epsilon)
  {
    if (1.0-fracPart < epsilon)
      ++intPart;
    switch (intPart)
    {
      case 1: result = 4; break; // 1.0 -> 0.2 substep
      case 2: result = 3; break; // 2.0 -> 0.5 substep
      case 3: result = 2; break; // 3.0 -> 1.0 substep
      case 4: result = 3; break; // 4.0 -> 1.0 substep
      case 5: result = 4; break; // 5.0 -> 1.0 substep
      case 6: result = 2; break; // 6.0 -> 2.0 substep
      case 7: result = 6; break; // 7.0 -> 1.0 substep
      case 8: result = 3; break; // 8.0 -> 2.0 substep
      case 9: result = 2; break; // 9.0 -> 3.0 substep
    }
  } else
  {
    // handle cases with significantly fractional mantissa:
    if (qAbs(fracPart-0.5) < epsilon) // *.5 mantissa
    {
      switch (intPart)
      {
        case 1: result = 2; break; // 1.5 -> 0.5 substep
        case 2: result = 4; break; // 2.5 -> 0.5 substep
        case 3: result = 4; break; // 3.5 -> 0.7 substep
        case 4: result = 2; break; // 4.5 -> 1.5 substep
        case 5: result = 4; break; // 5.5 -> 1.1 substep (won't occur with autoTickStep from here on)
        case 6: result = 4; break; // 6.5 -> 1.3 substep
        case 7: result = 2; break; // 7.5 -> 2.5 substep
        case 8: result = 4; break; // 8.5 -> 1.7 substep
        case 9: result = 4; break; // 9.5 -> 1.9 substep
      }
    }
    // if mantissa fraction isnt 0.0 or 0.5, don't bother finding good sub tick marks, leave default
  }
  
  return result;
}

/* inherits documentation from base class */
void QCPAxis::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  SelectablePart part = details.value<SelectablePart>();
  if (mSelectableParts.testFlag(part))
  {
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(additive ? mSelectedParts^part : part);
    if (selectionStateChanged)
      *selectionStateChanged = mSelectedParts != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAxis::deselectEvent(bool *selectionStateChanged)
{
  SelectableParts selBefore = mSelectedParts;
  setSelectedParts(mSelectedParts & ~mSelectableParts);
  if (selectionStateChanged)
    *selectionStateChanged = mSelectedParts != selBefore;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing axis lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPAxis::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/*! \internal
  
  Draws the axis with the specified \a painter, using the internal QCPAxisPainterPrivate instance.

*/
void QCPAxis::draw(QCPPainter *painter)
{
  const int lowTick = mLowestVisibleTick;
  const int highTick = mHighestVisibleTick;
  QVector<double> subTickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
  tickPositions.reserve(highTick-lowTick+1);
  tickLabels.reserve(highTick-lowTick+1);
  subTickPositions.reserve(mSubTickVector.size());
  
  if (mTicks)
  {
    for (int i=lowTick; i<=highTick; ++i)
    {
      tickPositions.append(coordToPixel(mTickVector.at(i)));
      if (mTickLabels)
        tickLabels.append(mTickVectorLabels.at(i));
    }
    
    if (mSubTickCount > 0)
    {
      const int subTickCount = mSubTickVector.size();
      for (int i=0; i<subTickCount; ++i) // no need to check bounds because subticks are always only created inside current mRange
        subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
    }
  }
  // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to draw the axis.
  // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
  mAxisPainter->type = mAxisType;
  mAxisPainter->basePen = getBasePen();
  mAxisPainter->labelFont = getLabelFont();
  mAxisPainter->labelColor = getLabelColor();
  mAxisPainter->label = mLabel;
  mAxisPainter->substituteExponent = mAutoTickLabels && mNumberBeautifulPowers && mTickLabelType == ltNumber;
  mAxisPainter->tickPen = getTickPen();
  mAxisPainter->subTickPen = getSubTickPen();
  mAxisPainter->tickLabelFont = getTickLabelFont();
  mAxisPainter->tickLabelColor = getTickLabelColor();
  mAxisPainter->axisRect = mAxisRect->rect();
  mAxisPainter->viewportRect = mParentPlot->viewport();
  mAxisPainter->abbreviateDecimalPowers = mScaleType == stLogarithmic;
  mAxisPainter->reversedEndings = mRangeReversed;
  mAxisPainter->tickPositions = tickPositions;
  mAxisPainter->tickLabels = tickLabels;
  mAxisPainter->subTickPositions = subTickPositions;
  mAxisPainter->draw(painter);
}

/*! \internal
  
  Returns via \a lowIndex and \a highIndex, which ticks in the current tick vector are visible in
  the current range. The return values are indices of the tick vector, not the positions of the
  ticks themselves.
  
  The actual use of this function is when an external tick vector is provided, since it might
  exceed far beyond the currently displayed range, and would cause unnecessary calculations e.g. of
  subticks.
  
  If all ticks are outside the axis range, an inverted range is returned, i.e. highIndex will be
  smaller than lowIndex. There is one case, where this function returns indices that are not really
  visible in the current axis range: When the tick spacing is larger than the axis range size and
  one tick is below the axis range and the next tick is already above the axis range. Because in
  such cases it is usually desirable to know the tick pair, to draw proper subticks.
*/
void QCPAxis::visibleTickBounds(int &lowIndex, int &highIndex) const
{
  bool lowFound = false;
  bool highFound = false;
  lowIndex = 0;
  highIndex = -1;
  
  for (int i=0; i < mTickVector.size(); ++i)
  {
    if (mTickVector.at(i) >= mRange.lower)
    {
      lowFound = true;
      lowIndex = i;
      break;
    }
  }
  for (int i=mTickVector.size()-1; i >= 0; --i)
  {
    if (mTickVector.at(i) <= mRange.upper)
    {
      highFound = true;
      highIndex = i;
      break;
    }
  }
  
  if (!lowFound && highFound)
    lowIndex = highIndex+1;
  else if (lowFound && !highFound)
    highIndex = lowIndex-1;
}

/*! \internal
  
  A log function with the base mScaleLogBase, used mostly for coordinate transforms in logarithmic
  scales with arbitrary log base. Uses the buffered mScaleLogBaseLogInv for faster calculation.
  This is set to <tt>1.0/qLn(mScaleLogBase)</tt> in \ref setScaleLogBase.
  
  \see basePow, setScaleLogBase, setScaleType
*/
double QCPAxis::baseLog(double value) const
{
  return qLn(value)*mScaleLogBaseLogInv;
}

/*! \internal
  
  A power function with the base mScaleLogBase, used mostly for coordinate transforms in
  logarithmic scales with arbitrary log base.
  
  \see baseLog, setScaleLogBase, setScaleType
*/
double QCPAxis::basePow(double value) const
{
  return qPow(mScaleLogBase, value);
}

/*! \internal
  
  Returns the pen that is used to draw the axis base line. Depending on the selection state, this
  is either mSelectedBasePen or mBasePen.
*/
QPen QCPAxis::getBasePen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal
  
  Returns the pen that is used to draw the (major) ticks. Depending on the selection state, this
  is either mSelectedTickPen or mTickPen.
*/
QPen QCPAxis::getTickPen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal
  
  Returns the pen that is used to draw the subticks. Depending on the selection state, this
  is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPAxis::getSubTickPen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal
  
  Returns the font that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPAxis::getTickLabelFont() const
{
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont : mTickLabelFont;
}

/*! \internal
  
  Returns the font that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPAxis::getLabelFont() const
{
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal
  
  Returns the color that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPAxis::getTickLabelColor() const
{
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor : mTickLabelColor;
}

/*! \internal
  
  Returns the color that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPAxis::getLabelColor() const
{
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor : mLabelColor;
}

/*! \internal
  
  Returns the appropriate outward margin for this axis. It is needed if \ref
  QCPAxisRect::setAutoMargins is set to true on the parent axis rect. An axis with axis type \ref
  atLeft will return an appropriate left margin, \ref atBottom will return an appropriate bottom
  margin and so forth. For the calculation, this function goes through similar steps as \ref draw,
  so changing one function likely requires the modification of the other one as well.
  
  The margin consists of the outward tick length, tick label padding, tick label size, label
  padding, label size, and padding.
  
  The margin is cached internally, so repeated calls while leaving the axis range, fonts, etc.
  unchanged are very fast.
*/
int QCPAxis::calculateMargin()
{
  if (!mVisible) // if not visible, directly return 0, don't cache 0 because we can't react to setVisible in QCPAxis
    return 0;
  
  if (mCachedMarginValid)
    return mCachedMargin;
  
  // run through similar steps as QCPAxis::draw, and caluclate margin needed to fit axis and its labels
  int margin = 0;
  
  int lowTick, highTick;
  visibleTickBounds(lowTick, highTick);
  QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
  tickPositions.reserve(highTick-lowTick+1);
  tickLabels.reserve(highTick-lowTick+1);
  if (mTicks)
  {
    for (int i=lowTick; i<=highTick; ++i)
    {
      tickPositions.append(coordToPixel(mTickVector.at(i)));
      if (mTickLabels)
        tickLabels.append(mTickVectorLabels.at(i));
    }
  }
  // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to calculate the size.
  // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
  mAxisPainter->type = mAxisType;
  mAxisPainter->labelFont = getLabelFont();
  mAxisPainter->label = mLabel;
  mAxisPainter->tickLabelFont = mTickLabelFont;
  mAxisPainter->axisRect = mAxisRect->rect();
  mAxisPainter->viewportRect = mParentPlot->viewport();
  mAxisPainter->tickPositions = tickPositions;
  mAxisPainter->tickLabels = tickLabels;
  margin += mAxisPainter->size();
  margin += mPadding;

  mCachedMargin = margin;
  mCachedMarginValid = true;
  return margin;
}

/* inherits documentation from base class */
QCP::Interaction QCPAxis::selectionCategory() const
{
  return QCP::iSelectAxes;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisPainterPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxisPainterPrivate

  \internal
  \brief (Private)
  
  This is a private class and not part of the public QCustomPlot interface.
  
  It is used by QCPAxis to do the low-level drawing of axis backbone, tick marks, tick labels and
  axis label. It also buffers the labels to reduce replot times. The parameters are configured by
  directly accessing the public member variables.
*/

/*!
  Constructs a QCPAxisPainterPrivate instance. Make sure to not create a new instance on every
  redraw, to utilize the caching mechanisms.
*/
QCPAxisPainterPrivate::QCPAxisPainterPrivate(QCustomPlot *parentPlot) :
  type(QCPAxis::atLeft),
  basePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  lowerEnding(QCPLineEnding::esNone),
  upperEnding(QCPLineEnding::esNone),
  labelPadding(0),
  tickLabelPadding(0),
  tickLabelRotation(0),
  tickLabelSide(QCPAxis::lsOutside),
  substituteExponent(true),
  numberMultiplyCross(false),
  tickLengthIn(5),
  tickLengthOut(0),
  subTickLengthIn(2),
  subTickLengthOut(0),
  tickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  subTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  offset(0),
  abbreviateDecimalPowers(false),
  reversedEndings(false),
  mParentPlot(parentPlot),
  mLabelCache(16) // cache at most 16 (tick) labels
{
}

QCPAxisPainterPrivate::~QCPAxisPainterPrivate()
{
}

/*! \internal
  
  Draws the axis with the specified \a painter.
  
  The selection boxes (mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox) are set
  here, too.
*/
void QCPAxisPainterPrivate::draw(QCPPainter *painter)
{
  QByteArray newHash = generateLabelParameterHash();
  if (newHash != mLabelParameterHash)
  {
    mLabelCache.clear();
    mLabelParameterHash = newHash;
  }
  
  QPoint origin;
  switch (type)
  {
    case QCPAxis::atLeft:   origin = axisRect.bottomLeft() +QPoint(-offset, 0); break;
    case QCPAxis::atRight:  origin = axisRect.bottomRight()+QPoint(+offset, 0); break;
    case QCPAxis::atTop:    origin = axisRect.topLeft()    +QPoint(0, -offset); break;
    case QCPAxis::atBottom: origin = axisRect.bottomLeft() +QPoint(0, +offset); break;
  }

  double xCor = 0, yCor = 0; // paint system correction, for pixel exact matches (affects baselines and ticks of top/right axes)
  switch (type)
  {
    case QCPAxis::atTop: yCor = -1; break;
    case QCPAxis::atRight: xCor = 1; break;
    default: break;
  }
  int margin = 0;
  // draw baseline:
  QLineF baseLine;
  painter->setPen(basePen);
  if (QCPAxis::orientation(type) == Qt::Horizontal)
    baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(axisRect.width()+xCor, yCor));
  else
    baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(xCor, -axisRect.height()+yCor));
  if (reversedEndings)
    baseLine = QLineF(baseLine.p2(), baseLine.p1()); // won't make a difference for line itself, but for line endings later
  painter->drawLine(baseLine);
  
  // draw ticks:
  if (!tickPositions.isEmpty())
  {
    painter->setPen(tickPen);
    int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1; // direction of ticks ("inward" is right for left axis and left for right axis)
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      for (int i=0; i<tickPositions.size(); ++i)
        painter->drawLine(QLineF(tickPositions.at(i)+xCor, origin.y()-tickLengthOut*tickDir+yCor, tickPositions.at(i)+xCor, origin.y()+tickLengthIn*tickDir+yCor));
    } else
    {
      for (int i=0; i<tickPositions.size(); ++i)
        painter->drawLine(QLineF(origin.x()-tickLengthOut*tickDir+xCor, tickPositions.at(i)+yCor, origin.x()+tickLengthIn*tickDir+xCor, tickPositions.at(i)+yCor));
    }
  }
  
  // draw subticks:
  if (!subTickPositions.isEmpty())
  {
    painter->setPen(subTickPen);
    // direction of ticks ("inward" is right for left axis and left for right axis)
    int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      for (int i=0; i<subTickPositions.size(); ++i)
        painter->drawLine(QLineF(subTickPositions.at(i)+xCor, origin.y()-subTickLengthOut*tickDir+yCor, subTickPositions.at(i)+xCor, origin.y()+subTickLengthIn*tickDir+yCor));
    } else
    {
      for (int i=0; i<subTickPositions.size(); ++i)
        painter->drawLine(QLineF(origin.x()-subTickLengthOut*tickDir+xCor, subTickPositions.at(i)+yCor, origin.x()+subTickLengthIn*tickDir+xCor, subTickPositions.at(i)+yCor));
    }
  }
  margin += qMax(0, qMax(tickLengthOut, subTickLengthOut));
  
  // draw axis base endings:
  bool antialiasingBackup = painter->antialiasing();
  painter->setAntialiasing(true); // always want endings to be antialiased, even if base and ticks themselves aren't
  painter->setBrush(QBrush(basePen.color()));
  QVector2D baseLineVector(baseLine.dx(), baseLine.dy());
  if (lowerEnding.style() != QCPLineEnding::esNone)
    lowerEnding.draw(painter, QVector2D(baseLine.p1())-baseLineVector.normalized()*lowerEnding.realLength()*(lowerEnding.inverted()?-1:1), -baseLineVector);
  if (upperEnding.style() != QCPLineEnding::esNone)
    upperEnding.draw(painter, QVector2D(baseLine.p2())+baseLineVector.normalized()*upperEnding.realLength()*(upperEnding.inverted()?-1:1), baseLineVector);
  painter->setAntialiasing(antialiasingBackup);
  
  // tick labels:
  QRect oldClipRect;
  if (tickLabelSide == QCPAxis::lsInside) // if using inside labels, clip them to the axis rect
  {
    oldClipRect = painter->clipRegion().boundingRect();
    painter->setClipRect(axisRect);
  }
  QSize tickLabelsSize(0, 0); // size of largest tick label, for offset calculation of axis label
  if (!tickLabels.isEmpty())
  {
    if (tickLabelSide == QCPAxis::lsOutside)
      margin += tickLabelPadding;
    painter->setFont(tickLabelFont);
    painter->setPen(QPen(tickLabelColor));
    const int maxLabelIndex = qMin(tickPositions.size(), tickLabels.size());
    int distanceToAxis = margin;
    if (tickLabelSide == QCPAxis::lsInside)
      distanceToAxis = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
    for (int i=0; i<maxLabelIndex; ++i)
      placeTickLabel(painter, tickPositions.at(i), distanceToAxis, tickLabels.at(i), &tickLabelsSize);
    if (tickLabelSide == QCPAxis::lsOutside)
      margin += (QCPAxis::orientation(type) == Qt::Horizontal) ? tickLabelsSize.height() : tickLabelsSize.width();
  }
  if (tickLabelSide == QCPAxis::lsInside)
    painter->setClipRect(oldClipRect);
  
  // axis label:
  QRect labelBounds;
  if (!label.isEmpty())
  {
    margin += labelPadding;
    painter->setFont(labelFont);
    painter->setPen(QPen(labelColor));
    labelBounds = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip, label);
    if (type == QCPAxis::atLeft)
    {
      QTransform oldTransform = painter->transform();
      painter->translate((origin.x()-margin-labelBounds.height()), origin.y());
      painter->rotate(-90);
      painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
      painter->setTransform(oldTransform);
    }
    else if (type == QCPAxis::atRight)
    {
      QTransform oldTransform = painter->transform();
      painter->translate((origin.x()+margin+labelBounds.height()), origin.y()-axisRect.height());
      painter->rotate(90);
      painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
      painter->setTransform(oldTransform);
    }
    else if (type == QCPAxis::atTop)
      painter->drawText(origin.x(), origin.y()-margin-labelBounds.height(), axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
    else if (type == QCPAxis::atBottom)
      painter->drawText(origin.x(), origin.y()+margin, axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
  }
  
  // set selection boxes:
  int selectionTolerance = 0;
  if (mParentPlot)
    selectionTolerance = mParentPlot->selectionTolerance();
  else
    qDebug() << Q_FUNC_INFO << "mParentPlot is null";
  int selAxisOutSize = qMax(qMax(tickLengthOut, subTickLengthOut), selectionTolerance);
  int selAxisInSize = selectionTolerance;
  int selTickLabelSize;
  int selTickLabelOffset;
  if (tickLabelSide == QCPAxis::lsOutside)
  {
    selTickLabelSize = (QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
    selTickLabelOffset = qMax(tickLengthOut, subTickLengthOut)+tickLabelPadding;
  } else
  {
    selTickLabelSize = -(QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
    selTickLabelOffset = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
  }
  int selLabelSize = labelBounds.height();
  int selLabelOffset = qMax(tickLengthOut, subTickLengthOut)+(!tickLabels.isEmpty() && tickLabelSide == QCPAxis::lsOutside ? tickLabelPadding+selTickLabelSize : 0)+labelPadding;
  if (type == QCPAxis::atLeft)
  {
    mAxisSelectionBox.setCoords(origin.x()-selAxisOutSize, axisRect.top(), origin.x()+selAxisInSize, axisRect.bottom());
    mTickLabelsSelectionBox.setCoords(origin.x()-selTickLabelOffset-selTickLabelSize, axisRect.top(), origin.x()-selTickLabelOffset, axisRect.bottom());
    mLabelSelectionBox.setCoords(origin.x()-selLabelOffset-selLabelSize, axisRect.top(), origin.x()-selLabelOffset, axisRect.bottom());
  } else if (type == QCPAxis::atRight)
  {
    mAxisSelectionBox.setCoords(origin.x()-selAxisInSize, axisRect.top(), origin.x()+selAxisOutSize, axisRect.bottom());
    mTickLabelsSelectionBox.setCoords(origin.x()+selTickLabelOffset+selTickLabelSize, axisRect.top(), origin.x()+selTickLabelOffset, axisRect.bottom());
    mLabelSelectionBox.setCoords(origin.x()+selLabelOffset+selLabelSize, axisRect.top(), origin.x()+selLabelOffset, axisRect.bottom());
  } else if (type == QCPAxis::atTop)
  {
    mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisOutSize, axisRect.right(), origin.y()+selAxisInSize);
    mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()-selTickLabelOffset-selTickLabelSize, axisRect.right(), origin.y()-selTickLabelOffset);
    mLabelSelectionBox.setCoords(axisRect.left(), origin.y()-selLabelOffset-selLabelSize, axisRect.right(), origin.y()-selLabelOffset);
  } else if (type == QCPAxis::atBottom)
  {
    mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisInSize, axisRect.right(), origin.y()+selAxisOutSize);
    mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()+selTickLabelOffset+selTickLabelSize, axisRect.right(), origin.y()+selTickLabelOffset);
    mLabelSelectionBox.setCoords(axisRect.left(), origin.y()+selLabelOffset+selLabelSize, axisRect.right(), origin.y()+selLabelOffset);
  }
  mAxisSelectionBox = mAxisSelectionBox.normalized();
  mTickLabelsSelectionBox = mTickLabelsSelectionBox.normalized();
  mLabelSelectionBox = mLabelSelectionBox.normalized();
  // draw hitboxes for debug purposes:
  //painter->setBrush(Qt::NoBrush);
  //painter->drawRects(QVector<QRect>() << mAxisSelectionBox << mTickLabelsSelectionBox << mLabelSelectionBox);
}

/*! \internal
  
  Returns the size ("margin" in QCPAxisRect context, so measured perpendicular to the axis backbone
  direction) needed to fit the axis.
*/
int QCPAxisPainterPrivate::size() const
{
  int result = 0;
  
  // get length of tick marks pointing outwards:
  if (!tickPositions.isEmpty())
    result += qMax(0, qMax(tickLengthOut, subTickLengthOut));
  
  // calculate size of tick labels:
  if (tickLabelSide == QCPAxis::lsOutside)
  {
    QSize tickLabelsSize(0, 0);
    if (!tickLabels.isEmpty())
    {
      for (int i=0; i<tickLabels.size(); ++i)
        getMaxTickLabelSize(tickLabelFont, tickLabels.at(i), &tickLabelsSize);
      result += QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width();
    result += tickLabelPadding;
    }
  }
  
  // calculate size of axis label (only height needed, because left/right labels are rotated by 90 degrees):
  if (!label.isEmpty())
  {
    QFontMetrics fontMetrics(labelFont);
    QRect bounds;
    bounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter | Qt::AlignVCenter, label);
    result += bounds.height() + labelPadding;
  }
  
  return result;
}

/*! \internal
  
  Clears the internal label cache. Upon the next \ref draw, all labels will be created new. This
  method is called automatically in \ref draw, if any parameters have changed that invalidate the
  cached labels, such as font, color, etc.
*/
void QCPAxisPainterPrivate::clearCache()
{
  mLabelCache.clear();
}

/*! \internal
  
  Returns a hash that allows uniquely identifying whether the label parameters have changed such
  that the cached labels must be refreshed (\ref clearCache). It is used in \ref draw. If the
  return value of this method hasn't changed since the last redraw, the respective label parameters
  haven't changed and cached labels may be used.
*/
QByteArray QCPAxisPainterPrivate::generateLabelParameterHash() const
{
  QByteArray result;
  result.append(QByteArray::number(tickLabelRotation));
  result.append(QByteArray::number((int)tickLabelSide));
  result.append(QByteArray::number((int)substituteExponent));
  result.append(QByteArray::number((int)numberMultiplyCross));
  result.append(tickLabelColor.name().toLatin1()+QByteArray::number(tickLabelColor.alpha(), 16));
  result.append(tickLabelFont.toString().toLatin1());
  return result;
}

/*! \internal
  
  Draws a single tick label with the provided \a painter, utilizing the internal label cache to
  significantly speed up drawing of labels that were drawn in previous calls. The tick label is
  always bound to an axis, the distance to the axis is controllable via \a distanceToAxis in
  pixels. The pixel position in the axis direction is passed in the \a position parameter. Hence
  for the bottom axis, \a position would indicate the horizontal pixel position (not coordinate),
  at which the label should be drawn.
  
  In order to later draw the axis label in a place that doesn't overlap with the tick labels, the
  largest tick label size is needed. This is acquired by passing a \a tickLabelsSize to the \ref
  drawTickLabel calls during the process of drawing all tick labels of one axis. In every call, \a
  tickLabelsSize is expanded, if the drawn label exceeds the value \a tickLabelsSize currently
  holds.
  
  The label is drawn with the font and pen that are currently set on the \a painter. To draw
  superscripted powers, the font is temporarily made smaller by a fixed factor (see \ref
  getTickLabelData).
*/
void QCPAxisPainterPrivate::placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize)
{
  // warning: if you change anything here, also adapt getMaxTickLabelSize() accordingly!
  if (text.isEmpty()) return;
  QSize finalSize;
  QPointF labelAnchor;
  switch (type)
  {
    case QCPAxis::atLeft:   labelAnchor = QPointF(axisRect.left()-distanceToAxis-offset, position); break;
    case QCPAxis::atRight:  labelAnchor = QPointF(axisRect.right()+distanceToAxis+offset, position); break;
    case QCPAxis::atTop:    labelAnchor = QPointF(position, axisRect.top()-distanceToAxis-offset); break;
    case QCPAxis::atBottom: labelAnchor = QPointF(position, axisRect.bottom()+distanceToAxis+offset); break;
  }
  if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && !painter->modes().testFlag(QCPPainter::pmNoCaching)) // label caching enabled
  {
    CachedLabel *cachedLabel = mLabelCache.take(text); // attempt to get label from cache
    if (!cachedLabel)  // no cached label existed, create it
    {
      cachedLabel = new CachedLabel;
      TickLabelData labelData = getTickLabelData(painter->font(), text);
      cachedLabel->offset = getTickLabelDrawOffset(labelData)+labelData.rotatedTotalBounds.topLeft();
      cachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size());
      cachedLabel->pixmap.fill(Qt::transparent);
      QCPPainter cachePainter(&cachedLabel->pixmap);
      cachePainter.setPen(painter->pen());
      drawTickLabel(&cachePainter, -labelData.rotatedTotalBounds.topLeft().x(), -labelData.rotatedTotalBounds.topLeft().y(), labelData);
    }
    // if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
    bool labelClippedByBorder = false;
    if (tickLabelSide == QCPAxis::lsOutside)
    {
      if (QCPAxis::orientation(type) == Qt::Horizontal)
        labelClippedByBorder = labelAnchor.x()+cachedLabel->offset.x()+cachedLabel->pixmap.width() > viewportRect.right() || labelAnchor.x()+cachedLabel->offset.x() < viewportRect.left();
      else
        labelClippedByBorder = labelAnchor.y()+cachedLabel->offset.y()+cachedLabel->pixmap.height() > viewportRect.bottom() || labelAnchor.y()+cachedLabel->offset.y() < viewportRect.top();
    }
    if (!labelClippedByBorder)
    {
      painter->drawPixmap(labelAnchor+cachedLabel->offset, cachedLabel->pixmap);
      finalSize = cachedLabel->pixmap.size();
    }
    mLabelCache.insert(text, cachedLabel); // return label to cache or insert for the first time if newly created
  } else // label caching disabled, draw text directly on surface:
  {
    TickLabelData labelData = getTickLabelData(painter->font(), text);
    QPointF finalPosition = labelAnchor + getTickLabelDrawOffset(labelData);
    // if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
     bool labelClippedByBorder = false;
    if (tickLabelSide == QCPAxis::lsOutside)
    {
      if (QCPAxis::orientation(type) == Qt::Horizontal)
        labelClippedByBorder = finalPosition.x()+(labelData.rotatedTotalBounds.width()+labelData.rotatedTotalBounds.left()) > viewportRect.right() || finalPosition.x()+labelData.rotatedTotalBounds.left() < viewportRect.left();
      else
        labelClippedByBorder = finalPosition.y()+(labelData.rotatedTotalBounds.height()+labelData.rotatedTotalBounds.top()) > viewportRect.bottom() || finalPosition.y()+labelData.rotatedTotalBounds.top() < viewportRect.top();
    }
    if (!labelClippedByBorder)
    {
      drawTickLabel(painter, finalPosition.x(), finalPosition.y(), labelData);
      finalSize = labelData.rotatedTotalBounds.size();
    }
  }
  
  // expand passed tickLabelsSize if current tick label is larger:
  if (finalSize.width() > tickLabelsSize->width())
    tickLabelsSize->setWidth(finalSize.width());
  if (finalSize.height() > tickLabelsSize->height())
    tickLabelsSize->setHeight(finalSize.height());
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Draws the tick label specified in \a labelData with \a painter at the pixel positions \a x and \a
  y. This function is used by \ref placeTickLabel to create new tick labels for the cache, or to
  directly draw the labels on the QCustomPlot surface when label caching is disabled, i.e. when
  QCP::phCacheLabels plotting hint is not set.
*/
void QCPAxisPainterPrivate::drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const
{
  // backup painter settings that we're about to change:
  QTransform oldTransform = painter->transform();
  QFont oldFont = painter->font();
  
  // transform painter to position/rotation:
  painter->translate(x, y);
  if (!qFuzzyIsNull(tickLabelRotation))
    painter->rotate(tickLabelRotation);
  
  // draw text:
  if (!labelData.expPart.isEmpty()) // indicator that beautiful powers must be used
  {
    painter->setFont(labelData.baseFont);
    painter->drawText(0, 0, 0, 0, Qt::TextDontClip, labelData.basePart);
    painter->setFont(labelData.expFont);
    painter->drawText(labelData.baseBounds.width()+1, 0, labelData.expBounds.width(), labelData.expBounds.height(), Qt::TextDontClip,  labelData.expPart);
  } else
  {
    painter->setFont(labelData.baseFont);
    painter->drawText(0, 0, labelData.totalBounds.width(), labelData.totalBounds.height(), Qt::TextDontClip | Qt::AlignHCenter, labelData.basePart);
  }
  
  // reset painter settings to what it was before:
  painter->setTransform(oldTransform);
  painter->setFont(oldFont);
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Transforms the passed \a text and \a font to a tickLabelData structure that can then be further
  processed by \ref getTickLabelDrawOffset and \ref drawTickLabel. It splits the text into base and
  exponent if necessary (member substituteExponent) and calculates appropriate bounding boxes.
*/
QCPAxisPainterPrivate::TickLabelData QCPAxisPainterPrivate::getTickLabelData(const QFont &font, const QString &text) const
{
  TickLabelData result;
  
  // determine whether beautiful decimal powers should be used
  bool useBeautifulPowers = false;
  int ePos = -1;
  if (substituteExponent)
  {
    ePos = text.indexOf(QLatin1Char('e'));
    if (ePos > -1)
      useBeautifulPowers = true;
  }
  
  // calculate text bounding rects and do string preparation for beautiful decimal powers:
  result.baseFont = font;
  if (result.baseFont.pointSizeF() > 0) // might return -1 if specified with setPixelSize, in that case we can't do correction in next line
    result.baseFont.setPointSizeF(result.baseFont.pointSizeF()+0.05); // QFontMetrics.boundingRect has a bug for exact point sizes that make the results oscillate due to internal rounding
  if (useBeautifulPowers)
  {
    // split text into parts of number/symbol that will be drawn normally and part that will be drawn as exponent:
    result.basePart = text.left(ePos);
    // in log scaling, we want to turn "1*10^n" into "10^n", else add multiplication sign and decimal base:
    if (abbreviateDecimalPowers && result.basePart == QLatin1String("1"))
      result.basePart = QLatin1String("10");
    else
      result.basePart += (numberMultiplyCross ? QString(QChar(215)) : QString(QChar(183))) + QLatin1String("10");
    result.expPart = text.mid(ePos+1);
    // clip "+" and leading zeros off expPart:
    while (result.expPart.length() > 2 && result.expPart.at(1) == QLatin1Char('0')) // length > 2 so we leave one zero when numberFormatChar is 'e'
      result.expPart.remove(1, 1);
    if (!result.expPart.isEmpty() && result.expPart.at(0) == QLatin1Char('+'))
      result.expPart.remove(0, 1);
    // prepare smaller font for exponent:
    result.expFont = font;
    if (result.expFont.pointSize() > 0)
      result.expFont.setPointSize(result.expFont.pointSize()*0.75);
    else
      result.expFont.setPixelSize(result.expFont.pixelSize()*0.75);
    // calculate bounding rects of base part, exponent part and total one:
    result.baseBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.basePart);
    result.expBounds = QFontMetrics(result.expFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.expPart);
    result.totalBounds = result.baseBounds.adjusted(0, 0, result.expBounds.width()+2, 0); // +2 consists of the 1 pixel spacing between base and exponent (see drawTickLabel) and an extra pixel to include AA
  } else // useBeautifulPowers == false
  {
    result.basePart = text;
    result.totalBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter, result.basePart);
  }
  result.totalBounds.moveTopLeft(QPoint(0, 0)); // want bounding box aligned top left at origin, independent of how it was created, to make further processing simpler
  
  // calculate possibly different bounding rect after rotation:
  result.rotatedTotalBounds = result.totalBounds;
  if (!qFuzzyIsNull(tickLabelRotation))
  {
    QTransform transform;
    transform.rotate(tickLabelRotation);
    result.rotatedTotalBounds = transform.mapRect(result.rotatedTotalBounds);
  }
  
  return result;
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Calculates the offset at which the top left corner of the specified tick label shall be drawn.
  The offset is relative to a point right next to the tick the label belongs to.
  
  This function is thus responsible for e.g. centering tick labels under ticks and positioning them
  appropriately when they are rotated.
*/
QPointF QCPAxisPainterPrivate::getTickLabelDrawOffset(const TickLabelData &labelData) const
{
  /*
    calculate label offset from base point at tick (non-trivial, for best visual appearance): short
    explanation for bottom axis: The anchor, i.e. the point in the label that is placed
    horizontally under the corresponding tick is always on the label side that is closer to the
    axis (e.g. the left side of the text when we're rotating clockwise). On that side, the height
    is halved and the resulting point is defined the anchor. This way, a 90 degree rotated text
    will be centered under the tick (i.e. displaced horizontally by half its height). At the same
    time, a 45 degree rotated text will "point toward" its tick, as is typical for rotated tick
    labels.
  */
  bool doRotation = !qFuzzyIsNull(tickLabelRotation);
  bool flip = qFuzzyCompare(qAbs(tickLabelRotation), 90.0); // perfect +/-90 degree flip. Indicates vertical label centering on vertical axes.
  double radians = tickLabelRotation/180.0*M_PI;
  int x=0, y=0;
  if ((type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsInside)) // Anchor at right side of tick label
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = -qCos(radians)*labelData.totalBounds.width();
        y = flip ? -labelData.totalBounds.width()/2.0 : -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height()/2.0;
      } else
      {
        x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height();
        y = flip ? +labelData.totalBounds.width()/2.0 : +qSin(-radians)*labelData.totalBounds.width()-qCos(-radians)*labelData.totalBounds.height()/2.0;
      }
    } else
    {
      x = -labelData.totalBounds.width();
      y = -labelData.totalBounds.height()/2.0;
    }
  } else if ((type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsInside)) // Anchor at left side of tick label
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = +qSin(radians)*labelData.totalBounds.height();
        y = flip ? -labelData.totalBounds.width()/2.0 : -qCos(radians)*labelData.totalBounds.height()/2.0;
      } else
      {
        x = 0;
        y = flip ? +labelData.totalBounds.width()/2.0 : -qCos(-radians)*labelData.totalBounds.height()/2.0;
      }
    } else
    {
      x = 0;
      y = -labelData.totalBounds.height()/2.0;
    }
  } else if ((type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsInside)) // Anchor at bottom side of tick label
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = -qCos(radians)*labelData.totalBounds.width()+qSin(radians)*labelData.totalBounds.height()/2.0;
        y = -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height();
      } else
      {
        x = -qSin(-radians)*labelData.totalBounds.height()/2.0;
        y = -qCos(-radians)*labelData.totalBounds.height();
      }
    } else
    {
      x = -labelData.totalBounds.width()/2.0;
      y = -labelData.totalBounds.height();
    }
  } else if ((type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsInside)) // Anchor at top side of tick label
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = +qSin(radians)*labelData.totalBounds.height()/2.0;
        y = 0;
      } else
      {
        x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height()/2.0;
        y = +qSin(-radians)*labelData.totalBounds.width();
      }
    } else
    {
      x = -labelData.totalBounds.width()/2.0;
      y = 0;
    }
  }
  
  return QPointF(x, y);
}

/*! \internal
  
  Simulates the steps done by \ref placeTickLabel by calculating bounding boxes of the text label
  to be drawn, depending on number format etc. Since only the largest tick label is wanted for the
  margin calculation, the passed \a tickLabelsSize is only expanded, if it's currently set to a
  smaller width/height.
*/
void QCPAxisPainterPrivate::getMaxTickLabelSize(const QFont &font, const QString &text,  QSize *tickLabelsSize) const
{
  // note: this function must return the same tick label sizes as the placeTickLabel function.
  QSize finalSize;
  if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && mLabelCache.contains(text)) // label caching enabled and have cached label
  {
    const CachedLabel *cachedLabel = mLabelCache.object(text);
    finalSize = cachedLabel->pixmap.size();
  } else // label caching disabled or no label with this text cached:
  {
    TickLabelData labelData = getTickLabelData(font, text);
    finalSize = labelData.rotatedTotalBounds.size();
  }
  
  // expand passed tickLabelsSize if current tick label is larger:
  if (finalSize.width() > tickLabelsSize->width())
    tickLabelsSize->setWidth(finalSize.width());
  if (finalSize.height() > tickLabelsSize->height())
    tickLabelsSize->setHeight(finalSize.height());
}
