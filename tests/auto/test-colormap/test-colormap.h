#include <QtTest/QtTest>
#include "../../../qcustomplot.h"

class TestColorMap : public QObject
{
  Q_OBJECT
private slots:
  void init();
  void cleanup();
  
  void QCPColorScale_rescaleDataRange();
  
private:
  QCustomPlot *mPlot;
  QCPColorMap *mColorMap;
};





