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

#include "layoutelement-plottitle.h"

#include "../painter.h"
#include "../core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlotTitle
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPlotTitle
  \brief A layout element displaying a plot title text
  
  The text may be specified with \ref setText, theformatting can be controlled with \ref setFont
  and \ref setTextColor.
  
  A plot title can be added as follows:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpplottitle-creation
  
  Since a plot title is a common requirement, QCustomPlot offers specialized selection signals for
  easy interaction with QCPPlotTitle. If a layout element of type QCPPlotTitle is clicked, the
  signal \ref QCustomPlot::titleClick is emitted. A double click emits the \ref
  QCustomPlot::titleDoubleClick signal.
*/

/* start documentation of signals */

/*! \fn void QCPPlotTitle::selectionChanged(bool selected)
  
  This signal is emitted when the selection state has changed to \a selected, either by user
  interaction or by a direct call to \ref setSelected.
  
  \see setSelected, setSelectable
*/

/* end documentation of signals */

/*!
  Creates a new QCPPlotTitle instance and sets default values. The initial text is empty (\ref setText).
  
  To set the title text in the constructor, rather use \ref QCPPlotTitle(QCustomPlot *parentPlot, const QString &text).
*/
QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot),
  mFont(QFont(QLatin1String("sans serif"), 13*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont(QLatin1String("sans serif"), 13*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  if (parentPlot)
  {
    setLayer(parentPlot->currentLayer());
    mFont = QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold);
    mSelectedFont = QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold);
  }
  setMargins(QMargins(5, 5, 5, 0));
}

/*! \overload
  
  Creates a new QCPPlotTitle instance and sets default values. The initial text is set to \a text.
*/
QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot, const QString &text) :
  QCPLayoutElement(parentPlot),
  mText(text),
  mFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  setLayer(QLatin1String("axes"));
  setMargins(QMargins(5, 5, 5, 0));
}

/*!
  Sets the text that will be displayed to \a text. Multiple lines can be created by insertion of "\n".
  
  \see setFont, setTextColor
*/
void QCPPlotTitle::setText(const QString &text)
{
  mText = text;
}

/*!
  Sets the \a font of the title text.
  
  \see setTextColor, setSelectedFont
*/
void QCPPlotTitle::setFont(const QFont &font)
{
  mFont = font;
}

/*!
  Sets the \a color of the title text.
  
  \see setFont, setSelectedTextColor
*/
void QCPPlotTitle::setTextColor(const QColor &color)
{
  mTextColor = color;
}

/*!
  Sets the \a font of the title text that will be used if the plot title is selected (\ref setSelected).
  
  \see setFont
*/
void QCPPlotTitle::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

/*!
  Sets the \a color of the title text that will be used if the plot title is selected (\ref setSelected).
  
  \see setTextColor
*/
void QCPPlotTitle::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
}

/*!
  Sets whether the user may select this plot title to \a selectable.

  Note that even when \a selectable is set to <tt>false</tt>, the selection state may be changed
  programmatically via \ref setSelected.
*/
void QCPPlotTitle::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets the selection state of this plot title to \a selected. If the selection has changed, \ref
  selectionChanged is emitted.
  
  Note that this function can change the selection state independently of the current \ref
  setSelectable state.
*/
void QCPPlotTitle::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/* inherits documentation from base class */
void QCPPlotTitle::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeNone);
}

/* inherits documentation from base class */
void QCPPlotTitle::draw(QCPPainter *painter)
{
  painter->setFont(mainFont());
  painter->setPen(QPen(mainTextColor()));
  painter->drawText(mRect, Qt::AlignCenter, mText, &mTextBoundingRect);
}

/* inherits documentation from base class */
QSize QCPPlotTitle::minimumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rwidth() += mMargins.left() + mMargins.right();
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

/* inherits documentation from base class */
QSize QCPPlotTitle::maximumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rheight() += mMargins.top() + mMargins.bottom();
  result.setWidth(QWIDGETSIZE_MAX);
  return result;
}

/* inherits documentation from base class */
void QCPPlotTitle::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
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
void QCPPlotTitle::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
double QCPPlotTitle::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  if (mTextBoundingRect.contains(pos.toPoint()))
    return mParentPlot->selectionTolerance()*0.99;
  else
    return -1;
}

/*! \internal
  
  Returns the main font to be used. This is mSelectedFont if \ref setSelected is set to
  <tt>true</tt>, else mFont is returned.
*/
QFont QCPPlotTitle::mainFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Returns the main color to be used. This is mSelectedTextColor if \ref setSelected is set to
  <tt>true</tt>, else mTextColor is returned.
*/
QColor QCPPlotTitle::mainTextColor() const
{
  return mSelected ? mSelectedTextColor : mTextColor;
}

