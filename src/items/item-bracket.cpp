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

#include "item-bracket.h"

#include "../painter.h"
#include "../core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemBracket
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemBracket
  \brief A bracket for referencing/highlighting certain parts in the plot.

  \image html QCPItemBracket.png "Bracket example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a left and \a right, which define the span of the bracket. If \a left is
  actually farther to the left than \a right, the bracket is opened to the bottom, as shown in the
  example image.
  
  The bracket supports multiple styles via \ref setStyle. The length, i.e. how far the bracket
  stretches away from the embraced span, can be controlled with \ref setLength.
  
  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for styles \ref
  bsCalligraphic and \ref bsSquare. Anchors and positions are displayed for reference.</center>
  
  It provides an anchor \a center, to allow connection of other items, e.g. an arrow (QCPItemLine
  or QCPItemCurve) or a text label (QCPItemText), to the bracket.
*/

/*!
  Creates a bracket item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemBracket::QCPItemBracket(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  left(createPosition(QLatin1String("left"))),
  right(createPosition(QLatin1String("right"))),
  center(createAnchor(QLatin1String("center"), aiCenter))
{
  left->setCoords(0, 0);
  right->setCoords(1, 1);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setLength(8);
  setStyle(bsCalligraphic);
}

QCPItemBracket::~QCPItemBracket()
{
}

/*!
  Sets the pen that will be used to draw the bracket.
  
  Note that when the style is \ref bsCalligraphic, only the color will be taken from the pen, the
  stroke and width are ignored. To change the apparent stroke width of a calligraphic bracket, use
  \ref setLength, which has a similar effect.
  
  \see setSelectedPen
*/
void QCPItemBracket::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the bracket when selected
  
  \see setPen, setSelected
*/
void QCPItemBracket::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the \a length in pixels how far the bracket extends in the direction towards the embraced
  span of the bracket (i.e. perpendicular to the <i>left</i>-<i>right</i>-direction)
  
  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for styles \ref
  bsCalligraphic and \ref bsSquare. Anchors and positions are displayed for reference.</center>
*/
void QCPItemBracket::setLength(double length)
{
  mLength = length;
}

/*!
  Sets the style of the bracket, i.e. the shape/visual appearance.
  
  \see setPen
*/
void QCPItemBracket::setStyle(QCPItemBracket::BracketStyle style)
{
  mStyle = style;
}

/* inherits documentation from base class */
double QCPItemBracket::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return -1;
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;
  
  switch (mStyle)
  {
    case QCPItemBracket::bsSquare:
    case QCPItemBracket::bsRound:
    {
      double a = distSqrToLine((centerVec-widthVec).toPointF(), (centerVec+widthVec).toPointF(), pos);
      double b = distSqrToLine((centerVec-widthVec+lengthVec).toPointF(), (centerVec-widthVec).toPointF(), pos);
      double c = distSqrToLine((centerVec+widthVec+lengthVec).toPointF(), (centerVec+widthVec).toPointF(), pos);
      return qSqrt(qMin(qMin(a, b), c));
    }
    case QCPItemBracket::bsCurly:
    case QCPItemBracket::bsCalligraphic:
    {
      double a = distSqrToLine((centerVec-widthVec*0.75f+lengthVec*0.15f).toPointF(), (centerVec+lengthVec*0.3f).toPointF(), pos);
      double b = distSqrToLine((centerVec-widthVec+lengthVec*0.7f).toPointF(), (centerVec-widthVec*0.75f+lengthVec*0.15f).toPointF(), pos);
      double c = distSqrToLine((centerVec+widthVec*0.75f+lengthVec*0.15f).toPointF(), (centerVec+lengthVec*0.3f).toPointF(), pos);
      double d = distSqrToLine((centerVec+widthVec+lengthVec*0.7f).toPointF(), (centerVec+widthVec*0.75f+lengthVec*0.15f).toPointF(), pos);
      return qSqrt(qMin(qMin(a, b), qMin(c, d)));
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPItemBracket::draw(QCPPainter *painter)
{
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return;
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;

  QPolygon boundingPoly;
  boundingPoly << leftVec.toPoint() << rightVec.toPoint()
               << (rightVec-lengthVec).toPoint() << (leftVec-lengthVec).toPoint();
  QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
  if (clip.intersects(boundingPoly.boundingRect()))
  {
    painter->setPen(mainPen());
    switch (mStyle)
    {
      case bsSquare:
      {
        painter->drawLine((centerVec+widthVec).toPointF(), (centerVec-widthVec).toPointF());
        painter->drawLine((centerVec+widthVec).toPointF(), (centerVec+widthVec+lengthVec).toPointF());
        painter->drawLine((centerVec-widthVec).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        break;
      }
      case bsRound:
      {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        path.cubicTo((centerVec+widthVec).toPointF(), (centerVec+widthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-widthVec).toPointF(), (centerVec-widthVec).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCurly:
      {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        path.cubicTo((centerVec+widthVec-lengthVec*0.8f).toPointF(), (centerVec+0.4f*widthVec+lengthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-0.4f*widthVec+lengthVec).toPointF(), (centerVec-widthVec-lengthVec*0.8f).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCalligraphic:
      {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(mainPen().color()));
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        
        path.cubicTo((centerVec+widthVec-lengthVec*0.8f).toPointF(), (centerVec+0.4f*widthVec+0.8f*lengthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-0.4f*widthVec+0.8f*lengthVec).toPointF(), (centerVec-widthVec-lengthVec*0.8f).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        
        path.cubicTo((centerVec-widthVec-lengthVec*0.5f).toPointF(), (centerVec-0.2f*widthVec+1.2f*lengthVec).toPointF(), (centerVec+lengthVec*0.2f).toPointF());
        path.cubicTo((centerVec+0.2f*widthVec+1.2f*lengthVec).toPointF(), (centerVec+widthVec-lengthVec*0.5f).toPointF(), (centerVec+widthVec+lengthVec).toPointF());
        
        painter->drawPath(path);
        break;
      }
    }
  }
}

/* inherits documentation from base class */
QPointF QCPItemBracket::anchorPixelPoint(int anchorId) const
{
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return leftVec.toPointF();
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;
  
  switch (anchorId)
  {
    case aiCenter:
      return centerVec.toPointF();
  }
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemBracket::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}
