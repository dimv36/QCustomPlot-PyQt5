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

#ifndef QCP_PLOTTABLE_COLORMAP_H
#define QCP_PLOTTABLE_COLORMAP_H

#include "../global.h"
#include "../range.h"
#include "../plottable.h"
#include "../colorgradient.h"
#include "../layoutelements/layoutelement-colorscale.h"

class QCPPainter;
class QCPAxis;
class QCPColorMap;

class QCP_LIB_DECL QCPColorMapData
{
public:
  QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange);
  ~QCPColorMapData();
  QCPColorMapData(const QCPColorMapData &other);
  QCPColorMapData &operator=(const QCPColorMapData &other);
  
  // getters:
  int keySize() const { return mKeySize; }
  int valueSize() const { return mValueSize; }
  QCPRange keyRange() const { return mKeyRange; }
  QCPRange valueRange() const { return mValueRange; }
  QCPRange dataBounds() const { return mDataBounds; }
  double data(double key, double value);
  double cell(int keyIndex, int valueIndex);
  
  // setters:
  void setSize(int keySize, int valueSize);
  void setKeySize(int keySize);
  void setValueSize(int valueSize);
  void setRange(const QCPRange &keyRange, const QCPRange &valueRange);
  void setKeyRange(const QCPRange &keyRange);
  void setValueRange(const QCPRange &valueRange);
  void setData(double key, double value, double z);
  void setCell(int keyIndex, int valueIndex, double z);
  
  // non-property methods:
  void recalculateDataBounds();
  void clear();
  void fill(double z);
  bool isEmpty() const { return mIsEmpty; }
  void coordToCell(double key, double value, int *keyIndex, int *valueIndex) const;
  void cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const;
  
protected:
  // property members:
  int mKeySize, mValueSize;
  QCPRange mKeyRange, mValueRange;
  bool mIsEmpty;
  // non-property members:
  double *mData;
  QCPRange mDataBounds;
  bool mDataModified;
  
  friend class QCPColorMap;
};


class QCP_LIB_DECL QCPColorMap : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
  Q_PROPERTY(QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
  Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
  Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate)
  Q_PROPERTY(bool tightBoundary READ tightBoundary WRITE setTightBoundary)
  Q_PROPERTY(QCPColorScale* colorScale READ colorScale WRITE setColorScale)
  /// \endcond
public:
  explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPColorMap();
  
  // getters:
  QCPColorMapData *data() const { return mMapData; }
  QCPRange dataRange() const { return mDataRange; }
  QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }
  bool interpolate() const { return mInterpolate; }
  bool tightBoundary() const { return mTightBoundary; }
  QCPColorGradient gradient() const { return mGradient; }
  QCPColorScale *colorScale() const { return mColorScale.data(); }
  
  // setters:
  void setData(QCPColorMapData *data, bool copy=false);
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  void setInterpolate(bool enabled);
  void setTightBoundary(bool enabled);
  void setColorScale(QCPColorScale *colorScale);
  
  // non-property methods:
  void rescaleDataRange(bool recalculateDataBounds=false);
  Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode=Qt::SmoothTransformation, const QSize &thumbSize=QSize(32, 18));
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  void dataRangeChanged(QCPRange newRange);
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  void gradientChanged(QCPColorGradient newGradient);
  
protected:
  // property members:
  QCPRange mDataRange;
  QCPAxis::ScaleType mDataScaleType;
  QCPColorMapData *mMapData;
  QCPColorGradient mGradient;
  bool mInterpolate;
  bool mTightBoundary;
  QPointer<QCPColorScale> mColorScale;
  // non-property members:
  QImage mMapImage, mUndersampledMapImage;
  QPixmap mLegendIcon;
  bool mMapImageInvalidated;
  
  // introduced virtual methods:
  virtual void updateMapImage();
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};

#endif // QCP_PLOTTABLE_COLORMAP_H
 
