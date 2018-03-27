/** PyQt5 binding for QCustomPlot v2.0.0
 *  
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *  
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPItemAnchor /NoDefaultCtors/
{
%TypeHeaderCode
#include <QCustomPlot/src/item.h>
%End
public:
  QCPItemAnchor(QCustomPlot *parentPlot /TransferThis/, QCPAbstractItem *parentItem, const QString &name, int anchorId=-1);
  virtual ~QCPItemAnchor();
  
  // getters:
  QString name() const;
  virtual QPointF pixelPosition() const;
};


class QCPItemPosition : public QCPItemAnchor /NoDefaultCtors/
{
%TypeHeaderCode
#include <QCustomPlot/src/item.h>
%End
public:
  enum PositionType { ptAbsolute        ///< Static positioning in pixels, starting from the top left corner of the viewport/widget.
                      ,ptViewportRatio  ///< Static positioning given by a fraction of the viewport size. For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the viewport/widget. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the viewport/widget, etc.
                      ,ptAxisRectRatio  ///< Static positioning given by a fraction of the axis rect size (see \ref setAxisRect). For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the axis rect. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the axis rect, etc. You can also go beyond the axis rect by providing negative coordinates or coordinates larger than 1.
                      ,ptPlotCoords     ///< Dynamic positioning at a plot coordinate defined by two axes (see \ref setAxes).
                    };
  
  QCPItemPosition(QCustomPlot *parentPlot /TransferThis/, QCPAbstractItem *parentItem, const QString &name);
  virtual ~QCPItemPosition();
  
  // getters:
  PositionType type() const;
  PositionType typeX() const;
  PositionType typeY() const;
  QCPItemAnchor *parentAnchor() const;
  QCPItemAnchor *parentAnchorX() const;
  QCPItemAnchor *parentAnchorY() const;
  double key() const;
  double value() const;
  QPointF coords() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  QCPAxisRect *axisRect() const;
  virtual QPointF pixelPosition() const;
  
  // setters:
  void setType(PositionType type);
  void setTypeX(PositionType type);
  void setTypeY(PositionType type);
  bool setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  void setCoords(double key, double value);
  void setCoords(const QPointF &coords);
  void setAxes(QCPAxis* keyAxis, QCPAxis* valueAxis);
  void setAxisRect(QCPAxisRect *axisRect);
  void setPixelPosition(const QPointF &pixelPosition);
};


class QCPAbstractItem : public QCPLayerable /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/item.h>
%End
public:
  explicit QCPAbstractItem(QCustomPlot *parentPlot /TransferThis/);
  virtual ~QCPAbstractItem();
  
  // getters:
  bool clipToAxisRect() const;
  QCPAxisRect *clipAxisRect() const;
  bool selectable() const;
  bool selected() const;
  
  // setters:
  void setClipToAxisRect(bool clip);
  void setClipAxisRect(QCPAxisRect *rect);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  
  // non-virtual methods:
  QList<QCPItemPosition*> positions() const;
  QList<QCPItemAnchor*> anchors() const;
  QCPItemPosition *position(const QString &name) const;
  QCPItemAnchor *anchor(const QString &name) const;
  bool hasAnchor(const QString &name) const;
  
signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
};
