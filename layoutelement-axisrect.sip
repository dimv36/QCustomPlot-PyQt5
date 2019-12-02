/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPAxisRect : public QCPLayoutElement
{
%TypeHeaderCode
#include <QCustomPlot/src/layoutelements/layoutelement-axisrect.h>
%End
public:
  explicit QCPAxisRect(QCustomPlot *parentPlot /TransferThis/, bool setupDefaultAxes=true);
  virtual ~QCPAxisRect();

  // getters:
  QPixmap background() const;
  QBrush backgroundBrush() const;
  bool backgroundScaled() const;
  Qt::AspectRatioMode backgroundScaledMode() const;
  Qt::Orientations rangeDrag() const;
  Qt::Orientations rangeZoom() const;
  QCPAxis *rangeDragAxis(Qt::Orientation orientation);
  QCPAxis *rangeZoomAxis(Qt::Orientation orientation);
  QList<QCPAxis*> rangeDragAxes(Qt::Orientation orientation);
  QList<QCPAxis*> rangeZoomAxes(Qt::Orientation orientation);
  double rangeZoomFactor(Qt::Orientation orientation);

  // setters:
  void setBackground(const QPixmap &pm);
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  void setBackground(const QBrush &brush);
  void setBackgroundScaled(bool scaled);
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  void setRangeDrag(Qt::Orientations orientations);
  void setRangeZoom(Qt::Orientations orientations);
  void setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeDragAxes(QList<QCPAxis*> axes);
  void setRangeDragAxes(QList<QCPAxis*> horizontal, QList<QCPAxis*> vertical);
  void setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeZoomAxes(QList<QCPAxis*> axes);
  void setRangeZoomAxes(QList<QCPAxis*> horizontal, QList<QCPAxis*> vertical);
  void setRangeZoomFactor(double horizontalFactor, double verticalFactor);
  void setRangeZoomFactor(double factor);

  // non-property methods:
  int axisCount(QCPAxis::AxisType type) const;
  QCPAxis *axis(QCPAxis::AxisType type, int index=0) const;
  QList<QCPAxis*> axes(QCPAxis::AxisTypes types) const;
  QList<QCPAxis*> axes() const;
  QCPAxis *addAxis(QCPAxis::AxisType type, QCPAxis *axis=0);
  QList<QCPAxis*> addAxes(QCPAxis::AxisTypes types);
  bool removeAxis(QCPAxis *axis);
  QCPLayoutInset *insetLayout() const;

  void zoom(const QRectF &pixelRect);
  void zoom(const QRectF &pixelRect, const QList<QCPAxis*> &affectedAxes);
  void setupFullAxesBox(bool connectRanges=false);
  QList<QCPAbstractPlottable*> plottables() const;
  QList<QCPGraph*> graphs() const;
  QList<QCPAbstractItem*> items() const;

  // read-only interface imitating a QRect:
  int left() const;
  int right() const;
  int top() const;
  int bottom() const;
  int width() const;
  int height() const;
  QSize size() const;
  QPoint topLeft() const;
  QPoint topRight() const;
  QPoint bottomLeft() const;
  QPoint bottomRight() const;
  QPoint center() const;

  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
};
