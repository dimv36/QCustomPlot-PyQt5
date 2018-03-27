/** PyQt5 binding for QCustomPlot v2.0.0
 *  
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *  
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPLayer : public QObject
{
%TypeHeaderCode
#include <QCustomPlot/src/layer.h>
%End
public:
  enum LayerMode { lmLogical   ///< Layer is used only for rendering order, and shares paint buffer with all other adjacent logical layers.
                   ,lmBuffered ///< Layer has its own paint buffer and may be replotted individually (see \ref replot).
                 };
  
  QCPLayer(QCustomPlot* parentPlot /TransferThis/, const QString &layerName);
  virtual ~QCPLayer();
  
  // getters:
  QCustomPlot *parentPlot() const;
  QString name() const;
  int index() const;
  QList<QCPLayerable*> children() const;
  bool visible() const; 
  LayerMode mode() const;
  
  // setters:
  void setVisible(bool visible);
  void setMode(LayerMode mode);
  
  // non-virtual methods:
  void replot();
};

class QCPLayerable : public QObject /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/layer.h>
%End
public:
  QCPLayerable(QCustomPlot* plot /TransferThis/, QString targetLayer=QString(), QCPLayerable *parentLayerable=0);
  virtual ~QCPLayerable();
  
  // getters:
  bool visible() const; 
  QCustomPlot *parentPlot() const;
  QCPLayerable *parentLayerable() const;
  QCPLayer *layer() const;
  bool antialiased() const;
  
  // setters:
  void setVisible(bool on);
  Q_SLOT bool setLayer(QCPLayer *layer);
  bool setLayer(const QString &layerName);
  void setAntialiased(bool enabled);
  
  // introduced virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-property methods:
  bool realVisibility() const;
  
signals:
  void layerChanged(QCPLayer *newLayer);
};
 