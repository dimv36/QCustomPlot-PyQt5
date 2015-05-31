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
/*! \file */
#ifndef QCP_PLOTTABLE_BARS_H
#define QCP_PLOTTABLE_BARS_H

#include "../global.h"
#include "../range.h"
#include "../plottable.h"

class QCPPainter;
class QCPAxis;
class QCPBars;

class QCP_LIB_DECL QCPBarsGroup : public QObject
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(SpacingType spacingType READ spacingType WRITE setSpacingType)
  Q_PROPERTY(double spacing READ spacing WRITE setSpacing)
  /// \endcond
public:
  /*!
    Defines the ways the spacing between bars in the group can be specified. Thus it defines what
    the number passed to \ref setSpacing actually means.
    
    \see setSpacingType, setSpacing
  */
  enum SpacingType { stAbsolute       ///< Bar spacing is in absolute pixels
                     ,stAxisRectRatio ///< Bar spacing is given by a fraction of the axis rect size
                     ,stPlotCoords    ///< Bar spacing is in key coordinates and thus scales with the key axis range
                 };
  QCPBarsGroup(QCustomPlot *parentPlot);
  ~QCPBarsGroup();
  
  // getters:
  SpacingType spacingType() const { return mSpacingType; }
  double spacing() const { return mSpacing; }
  
  // setters:
  void setSpacingType(SpacingType spacingType);
  void setSpacing(double spacing);
  
  // non-virtual methods:
  QList<QCPBars*> bars() const { return mBars; }
  QCPBars* bars(int index) const;
  int size() const { return mBars.size(); }
  bool isEmpty() const { return mBars.isEmpty(); }
  void clear();
  bool contains(QCPBars *bars) const { return mBars.contains(bars); }
  void append(QCPBars *bars);
  void insert(int i, QCPBars *bars);
  void remove(QCPBars *bars);
  
protected:
  // non-property members:
  QCustomPlot *mParentPlot;
  SpacingType mSpacingType;
  double mSpacing;
  QList<QCPBars*> mBars;
  
  // non-virtual methods:
  void registerBars(QCPBars *bars);
  void unregisterBars(QCPBars *bars);
  
  // virtual methods:
  double keyPixelOffset(const QCPBars *bars, double keyCoord);
  double getPixelSpacing(const QCPBars *bars, double keyCoord);
  
private:
  Q_DISABLE_COPY(QCPBarsGroup)
  
  friend class QCPBars;
};


class QCP_LIB_DECL QCPBarData
{
public:
  QCPBarData();
  QCPBarData(double key, double value);
  double key, value;
};
Q_DECLARE_TYPEINFO(QCPBarData, Q_MOVABLE_TYPE);

/*! \typedef QCPBarDataMap
  Container for storing \ref QCPBarData items in a sorted fashion. The key of the map
  is the key member of the QCPBarData instance.
  
  This is the container in which QCPBars holds its data.
  \see QCPBarData, QCPBars::setData
*/
typedef QMap<double, QCPBarData> QCPBarDataMap;
typedef QMapIterator<double, QCPBarData> QCPBarDataMapIterator;
typedef QMutableMapIterator<double, QCPBarData> QCPBarDataMutableMapIterator;


class QCP_LIB_DECL QCPBars : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(double width READ width WRITE setWidth)
  Q_PROPERTY(WidthType widthType READ widthType WRITE setWidthType)
  Q_PROPERTY(QCPBarsGroup* barsGroup READ barsGroup WRITE setBarsGroup)
  Q_PROPERTY(double baseValue READ baseValue WRITE setBaseValue)
  Q_PROPERTY(QCPBars* barBelow READ barBelow)
  Q_PROPERTY(QCPBars* barAbove READ barAbove)
  /// \endcond
public:
  /*!
    Defines the ways the width of the bar can be specified. Thus it defines what the number passed
    to \ref setWidth actually means.
    
    \see setWidthType, setWidth
  */
  enum WidthType { wtAbsolute       ///< Bar width is in absolute pixels
                   ,wtAxisRectRatio ///< Bar width is given by a fraction of the axis rect size
                   ,wtPlotCoords    ///< Bar width is in key coordinates and thus scales with the key axis range
                 };
   Q_ENUMS(WidthType)
  
  explicit QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPBars();
  
  // getters:
  double width() const { return mWidth; }
  WidthType widthType() const { return mWidthType; }
  QCPBarsGroup *barsGroup() const { return mBarsGroup; }
  double baseValue() const { return mBaseValue; }
  QCPBars *barBelow() const { return mBarBelow.data(); }
  QCPBars *barAbove() const { return mBarAbove.data(); }
  QCPBarDataMap *data() const { return mData; }
  
  // setters:
  void setWidth(double width);
  void setWidthType(WidthType widthType);
  void setBarsGroup(QCPBarsGroup *barsGroup);
  void setBaseValue(double baseValue);
  void setData(QCPBarDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);
  
  // non-property methods:
  void moveBelow(QCPBars *bars);
  void moveAbove(QCPBars *bars);
  void addData(const QCPBarDataMap &dataMap);
  void addData(const QCPBarData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QCPBarDataMap *mData;
  double mWidth;
  WidthType mWidthType;
  QCPBarsGroup *mBarsGroup;
  double mBaseValue;
  QPointer<QCPBars> mBarBelow, mBarAbove;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // non-virtual methods:
  void getVisibleDataBounds(QCPBarDataMap::const_iterator &lower, QCPBarDataMap::const_iterator &upperEnd) const;
  QPolygonF getBarPolygon(double key, double value) const;
  void getPixelWidth(double key, double &lower, double &upper) const;
  double getStackedBaseValue(double key, bool positive) const;
  static void connectBars(QCPBars* lower, QCPBars* upper);
  
  friend class QCustomPlot;
  friend class QCPLegend;
  friend class QCPBarsGroup;
};

#endif // QCP_PLOTTABLE_BARS_H
