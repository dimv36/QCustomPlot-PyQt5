/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPSelectionRect : public QCPLayerable
{
%TypeHeaderCode
#include <QCustomPlot/src/selectionrect.h>
%End
public:
  QCPSelectionRect(QCustomPlot *parentPlot /TransferThis/);
  virtual ~QCPSelectionRect();

  // getters:
  QRect rect() const;
  QCPRange range(const QCPAxis *axis) const;
  QPen pen() const;
  QBrush brush() const;
  bool isActive() const;

  // setters:
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);

  // non-property methods:
  Q_SLOT void cancel();

signals:
  void started(QMouseEvent *event);
  void changed(const QRect &rect, QMouseEvent *event);
  void canceled(const QRect &rect, QInputEvent *event);
  void accepted(const QRect &rect, QMouseEvent *event);
};
