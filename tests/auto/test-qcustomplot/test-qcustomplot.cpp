#include "test-qcustomplot.h"

void TestQCustomPlot::init()
{
  mPlot = new QCustomPlot(0);
  mPlot->show();
  QTest::qWait(150);
}

void TestQCustomPlot::cleanup()
{
  delete mPlot;
}

void TestQCustomPlot::rescaleAxes_GraphVisibility()
{
  mPlot->setGeometry(50, 50, 500, 500);
  
  mPlot->addGraph();
  mPlot->addGraph();
  mPlot->graph(0)->setData(QVector<double>()<<4<<5<<6, QVector<double>()<<-2<<-1<<0);
  mPlot->graph(1)->setData(QVector<double>()<<3<<5<<7, QVector<double>()<<-3<<-1<<1);
  mPlot->graph(1)->setVisible(false);
  
  // test rescaling with respect to all graphs, even invisible ones:
  mPlot->rescaleAxes(false);
  QCOMPARE(mPlot->xAxis->range().lower, 3.0);
  QCOMPARE(mPlot->xAxis->range().upper, 7.0);
  QCOMPARE(mPlot->yAxis->range().lower, -3.0);
  QCOMPARE(mPlot->yAxis->range().upper, 1.0);
  
  // test rescaling with respect to all visible graphs:
  mPlot->rescaleAxes(true);
  QCOMPARE(mPlot->xAxis->range().lower, 4.0);
  QCOMPARE(mPlot->xAxis->range().upper, 6.0);
  QCOMPARE(mPlot->yAxis->range().lower, -2.0);
  QCOMPARE(mPlot->yAxis->range().upper, 0.0);
}

void TestQCustomPlot::rescaleAxes_FlatGraph()
{
  // test rescaling if only one graph is in plot and it's flat:
  mPlot->xAxis->setRange(10, 11);
  mPlot->yAxis->setRange(10, 11);
  mPlot->addGraph();
  mPlot->graph(0)->setData(QVector<double>()<<2<<2<<2, QVector<double>()<<0<<0<<0);
  mPlot->rescaleAxes();
  QCOMPARE(mPlot->xAxis->range().lower, 1.5);
  QCOMPARE(mPlot->xAxis->range().upper, 2.5);
  QCOMPARE(mPlot->yAxis->range().lower, -0.5);
  QCOMPARE(mPlot->yAxis->range().upper, 0.5);
  
  // test rescaling when there are multiple graphs with different values but first graph is flat:
  mPlot->xAxis->setRange(10, 11);
  mPlot->yAxis->setRange(10, 11);
  mPlot->addGraph();
  mPlot->graph(1)->setData(QVector<double>()<<-1<<0<<1, QVector<double>()<<-0.1<<0<<0.1);
  mPlot->rescaleAxes();
  QCOMPARE(mPlot->xAxis->range().lower, -1.0);
  QCOMPARE(mPlot->xAxis->range().upper, 2.0);
  QCOMPARE(mPlot->yAxis->range().lower, -0.1);
  QCOMPARE(mPlot->yAxis->range().upper, 0.1);
}

void TestQCustomPlot::rescaleAxes_MultipleFlatGraphs()
{
  // test rescaling when there are multiple flat graphs:
  mPlot->addGraph();
  mPlot->addGraph();
  mPlot->graph(0)->setData(QVector<double>()<<1<<2<<3, QVector<double>()<<0<<0<<0);
  mPlot->graph(1)->setData(QVector<double>()<<-1<<0<<1, QVector<double>()<<2<<2<<2);
  
  mPlot->rescaleAxes();
  
  QCOMPARE(mPlot->xAxis->range().lower, -1.0);
  QCOMPARE(mPlot->xAxis->range().upper, 3.0);
  QCOMPARE(mPlot->yAxis->range().lower, 0.0);
  QCOMPARE(mPlot->yAxis->range().upper, 2.0);
}














