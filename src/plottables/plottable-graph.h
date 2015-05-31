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
#ifndef QCP_PLOTTABLE_GRAPH_H
#define QCP_PLOTTABLE_GRAPH_H

#include "../global.h"
#include "../range.h"
#include "../plottable.h"
#include "../painter.h"

class QCPPainter;
class QCPAxis;

class QCP_LIB_DECL QCPData
{
public:
  QCPData();
  QCPData(double key, double value);
  double key, value;
  double keyErrorPlus, keyErrorMinus;
  double valueErrorPlus, valueErrorMinus;
};
Q_DECLARE_TYPEINFO(QCPData, Q_MOVABLE_TYPE);

/*! \typedef QCPDataMap
  Container for storing \ref QCPData items in a sorted fashion. The key of the map
  is the key member of the QCPData instance.
  
  This is the container in which QCPGraph holds its data.
  \see QCPData, QCPGraph::setData
*/
typedef QMap<double, QCPData> QCPDataMap;
typedef QMapIterator<double, QCPData> QCPDataMapIterator;
typedef QMutableMapIterator<double, QCPData> QCPDataMutableMapIterator;


class QCP_LIB_DECL QCPGraph : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
  Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
  Q_PROPERTY(ErrorType errorType READ errorType WRITE setErrorType)
  Q_PROPERTY(QPen errorPen READ errorPen WRITE setErrorPen)
  Q_PROPERTY(double errorBarSize READ errorBarSize WRITE setErrorBarSize)
  Q_PROPERTY(bool errorBarSkipSymbol READ errorBarSkipSymbol WRITE setErrorBarSkipSymbol)
  Q_PROPERTY(QCPGraph* channelFillGraph READ channelFillGraph WRITE setChannelFillGraph)
  Q_PROPERTY(bool adaptiveSampling READ adaptiveSampling WRITE setAdaptiveSampling)
  /// \endcond
public:
  /*!
    Defines how the graph's line is represented visually in the plot. The line is drawn with the
    current pen of the graph (\ref setPen).
    \see setLineStyle
  */
  enum LineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
                                 ///< with symbols according to the scatter style, see \ref setScatterStyle)
                   ,lsLine       ///< data points are connected by a straight line
                   ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
                   ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
                   ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
                   ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
                 };
  Q_ENUMS(LineStyle)
  /*!
    Defines what kind of error bars are drawn for each data point
  */
  enum ErrorType { etNone   ///< No error bars are shown
                   ,etKey   ///< Error bars for the key dimension of the data point are shown
                   ,etValue ///< Error bars for the value dimension of the data point are shown
                   ,etBoth  ///< Error bars for both key and value dimensions of the data point are shown
                 };
  Q_ENUMS(ErrorType)
  
  explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPGraph();
  
  // getters:
  QCPDataMap *data() const { return mData; }
  LineStyle lineStyle() const { return mLineStyle; }
  QCPScatterStyle scatterStyle() const { return mScatterStyle; }
  ErrorType errorType() const { return mErrorType; }
  QPen errorPen() const { return mErrorPen; }
  double errorBarSize() const { return mErrorBarSize; }
  bool errorBarSkipSymbol() const { return mErrorBarSkipSymbol; }
  QCPGraph *channelFillGraph() const { return mChannelFillGraph.data(); }
  bool adaptiveSampling() const { return mAdaptiveSampling; }
  
  // setters:
  void setData(QCPDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueError);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError, const QVector<double> &valueError);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setLineStyle(LineStyle ls);
  void setScatterStyle(const QCPScatterStyle &style);
  void setErrorType(ErrorType errorType);
  void setErrorPen(const QPen &pen);
  void setErrorBarSize(double size);
  void setErrorBarSkipSymbol(bool enabled);
  void setChannelFillGraph(QCPGraph *targetGraph);
  void setAdaptiveSampling(bool enabled);
  
  // non-property methods:
  void addData(const QCPDataMap &dataMap);
  void addData(const QCPData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  using QCPAbstractPlottable::rescaleAxes;
  using QCPAbstractPlottable::rescaleKeyAxis;
  using QCPAbstractPlottable::rescaleValueAxis;
  void rescaleAxes(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleKeyAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleValueAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  
protected:
  // property members:
  QCPDataMap *mData;
  QPen mErrorPen;
  LineStyle mLineStyle;
  QCPScatterStyle mScatterStyle;
  ErrorType mErrorType;
  double mErrorBarSize;
  bool mErrorBarSkipSymbol;
  QPointer<QCPGraph> mChannelFillGraph;
  bool mAdaptiveSampling;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  
  // introduced virtual methods:
  virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const;
  virtual void drawLinePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawImpulsePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  
  // non-virtual methods:
  void getPreparedData(QVector<QCPData> *lineData, QVector<QCPData> *scatterData) const;
  void getPlotData(QVector<QPointF> *lineData, QVector<QCPData> *scatterData) const;
  void getScatterPlotData(QVector<QCPData> *scatterData) const;
  void getLinePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepLeftPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepRightPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepCenterPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getImpulsePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void drawError(QCPPainter *painter, double x, double y, const QCPData &data) const;
  void getVisibleDataBounds(QCPDataMap::const_iterator &lower, QCPDataMap::const_iterator &upper) const;
  int countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const;
  void addFillBasePoints(QVector<QPointF> *lineData) const;
  void removeFillBasePoints(QVector<QPointF> *lineData) const;
  QPointF lowerFillBasePoint(double lowerKey) const;
  QPointF upperFillBasePoint(double upperKey) const;
  const QPolygonF getChannelFillPolygon(const QVector<QPointF> *lineData) const;
  int findIndexBelowX(const QVector<QPointF> *data, double x) const;
  int findIndexAboveX(const QVector<QPointF> *data, double x) const;
  int findIndexBelowY(const QVector<QPointF> *data, double y) const;
  int findIndexAboveY(const QVector<QPointF> *data, double y) const;
  double pointDistance(const QPointF &pixelPoint) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};

#endif // QCP_PLOTTABLE_GRAPH_H
