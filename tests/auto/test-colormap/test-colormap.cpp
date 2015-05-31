#include "test-colormap.h"
#include <QMainWindow>

void TestColorMap::init()
{
  mPlot = new QCustomPlot(0);
  mColorMap = new QCPColorMap(mPlot->xAxis, mPlot->yAxis);
  mPlot->addPlottable(mColorMap);
}

void TestColorMap::QCPColorScale_rescaleDataRange()
{
  QCPColorScale *scale = new QCPColorScale(mPlot);
  mPlot->plotLayout()->addElement(0, 1, scale);
  
  QCPColorMap *map1 = new QCPColorMap(mPlot->xAxis, mPlot->yAxis);
  mPlot->addPlottable(map1);
  QCPColorMap *map2 = new QCPColorMap(mPlot->xAxis, mPlot->yAxis);
  mPlot->addPlottable(map2);
  map1->setColorScale(scale);
  map2->setColorScale(scale);
  map1->data()->setSize(2, 2);
  map2->data()->setSize(2, 2);
  
  // normal values:
  map1->data()->setCell(0, 0, 1);
  map1->data()->setCell(1, 0, 2);
  map1->data()->setCell(0, 1, 3);
  map1->data()->setCell(1, 1, 4);
  map1->data()->recalculateDataBounds();
  map2->data()->setCell(0, 0, 6);
  map2->data()->setCell(1, 0, 7);
  map2->data()->setCell(0, 1, 8);
  map2->data()->setCell(1, 1, 9);
  map2->data()->recalculateDataBounds();
  scale->rescaleDataRange(true);
  QCOMPARE(map1->dataRange().lower, 1.0);
  QCOMPARE(map1->dataRange().upper, 9.0);
  QCOMPARE(map2->dataRange().lower, 1.0);
  QCOMPARE(map2->dataRange().upper, 9.0);
  QCOMPARE(scale->dataRange().lower, 1.0);
  QCOMPARE(scale->dataRange().upper, 9.0);
  
  // one singular:
  map1->data()->setCell(0, 0, 2);
  map1->data()->setCell(1, 0, 2);
  map1->data()->setCell(0, 1, 2);
  map1->data()->setCell(1, 1, 2);
  map1->data()->recalculateDataBounds();
  map2->data()->setCell(0, 0, 6);
  map2->data()->setCell(1, 0, 7);
  map2->data()->setCell(0, 1, 8);
  map2->data()->setCell(1, 1, 9);
  map2->data()->recalculateDataBounds();
  scale->rescaleDataRange(true);
  QCOMPARE(map1->dataRange().lower, 2.0);
  QCOMPARE(map1->dataRange().upper, 9.0);
  QCOMPARE(map2->dataRange().lower, 2.0);
  QCOMPARE(map2->dataRange().upper, 9.0);
  QCOMPARE(scale->dataRange().lower, 2.0);
  QCOMPARE(scale->dataRange().upper, 9.0);
  
  // both singular:
  map1->data()->setCell(0, 0, 1);
  map1->data()->setCell(1, 0, 1);
  map1->data()->setCell(0, 1, 1);
  map1->data()->setCell(1, 1, 1);
  map1->data()->recalculateDataBounds();
  map2->data()->setCell(0, 0, 6);
  map2->data()->setCell(1, 0, 6);
  map2->data()->setCell(0, 1, 6);
  map2->data()->setCell(1, 1, 6);
  map2->data()->recalculateDataBounds();
  scale->rescaleDataRange(true);
  QCOMPARE(map1->dataRange().lower, 1.0);
  QCOMPARE(map1->dataRange().upper, 6.0);
  QCOMPARE(map2->dataRange().lower, 1.0);
  QCOMPARE(map2->dataRange().upper, 6.0);
  QCOMPARE(scale->dataRange().lower, 1.0);
  QCOMPARE(scale->dataRange().upper, 6.0);
  
  // both singular at same value (range should center on value):
  scale->setDataRange(QCPRange(0, 1));
  map1->data()->setCell(0, 0, 3);
  map1->data()->setCell(1, 0, 3);
  map1->data()->setCell(0, 1, 3);
  map1->data()->setCell(1, 1, 3);
  map1->data()->recalculateDataBounds();
  map2->data()->setCell(0, 0, 3);
  map2->data()->setCell(1, 0, 3);
  map2->data()->setCell(0, 1, 3);
  map2->data()->setCell(1, 1, 3);
  map2->data()->recalculateDataBounds();
  scale->rescaleDataRange(true);
  QCOMPARE(map1->dataRange().lower, 2.5);
  QCOMPARE(map1->dataRange().upper, 3.5);
  QCOMPARE(map2->dataRange().lower, 2.5);
  QCOMPARE(map2->dataRange().upper, 3.5);
  QCOMPARE(scale->dataRange().lower, 2.5);
  QCOMPARE(scale->dataRange().upper, 3.5);
}

void TestColorMap::cleanup()
{
  delete mPlot;
}
