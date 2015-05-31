#include "test-qcpgraph.h"
#include <QMainWindow>

void TestQCPGraph::init()
{
  mPlot = new QCustomPlot(0);
  mGraph = mPlot->addGraph();
}

void TestQCPGraph::cleanup()
{
  delete mPlot;
}

void TestQCPGraph::specializedGraphInterface()
{
  // for this test we don't use the graph created in init().
  mPlot->clearPlottables();
  
  // adding graphs:
  QCOMPARE(mPlot->plottableCount(), 0);
  QCOMPARE(mPlot->graphCount(), 0);
  
  QCPGraph *graph1 = mPlot->addGraph();
  QCOMPARE(mPlot->plottableCount(), 1);
  QCOMPARE(mPlot->graphCount(), 1);
  
  QCPGraph *graph2 = mPlot->addGraph();
  QCOMPARE(mPlot->plottableCount(), 2);
  QCOMPARE(mPlot->graphCount(), 2);
  
  QCPGraph *graph3 = new QCPGraph(mPlot->xAxis, mPlot->yAxis);
  mPlot->addPlottable(graph3);
  QCOMPARE(mPlot->plottableCount(), 3);
  QCOMPARE(mPlot->graphCount(), 3);
  
  // accessing graphs:
  QCOMPARE(mPlot->graph(0), graph1);
  QCOMPARE(mPlot->graph(1), graph2);
  QCOMPARE(mPlot->graph(2), graph3);
  QCOMPARE(mPlot->plottable(0), graph1);
  QCOMPARE(mPlot->plottable(1), graph2);
  QCOMPARE(mPlot->plottable(2), graph3);

  // removing graphs:
  mPlot->removeGraph(0);
  QCOMPARE(mPlot->graphCount(), 2);
  QCOMPARE(mPlot->graph(0), graph2);
  QCOMPARE(mPlot->graph(1), graph3);
  
  mPlot->removeGraph(graph2);
  QCOMPARE(mPlot->graphCount(), 1);
  QCOMPARE(mPlot->graph(0), graph3);
  
  mPlot->removeGraph(0);
  QCOMPARE(mPlot->graphCount(), 0);
  QCOMPARE(mPlot->plottableCount(), 0);
}

void TestQCPGraph::dataManipulation()
{
  QVERIFY(mGraph->data()->isEmpty());
  
  QVector<double> x, y;
  x << -1 << 1 << -2 << 2;
  y <<  1 << 2 <<  0 << 3;
  
  mGraph->setData(x, y);
  QCOMPARE(mGraph->data()->size(), 4);
  // data should be sorted by x:
  QCOMPARE((mGraph->data()->begin()+0).value().value, 0.0);
  QCOMPARE((mGraph->data()->begin()+1).value().value, 1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().value, 2.0);
  QCOMPARE((mGraph->data()->begin()+3).value().value, 3.0);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -2.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, -1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().key, 1.0);
  QCOMPARE((mGraph->data()->begin()+3).value().key, 2.0);
  // map key should be identical to data key:
  for (int i=0; i<4; ++i)
    QCOMPARE((mGraph->data()->begin()+i).key(), (mGraph->data()->begin()+i).value().key);
  
  // data removal:
  mGraph->removeDataBefore(0);
  QCOMPARE(mGraph->data()->size(), 2);
  QCOMPARE((mGraph->data()->begin()+0).value().key, 1.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, 2.0);
  
  mGraph->setData(x, y);
  mGraph->removeDataAfter(0);
  QCOMPARE(mGraph->data()->size(), 2);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -2.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, -1.0);
  
  mGraph->setData(x, y);
  mGraph->removeData(-1.1, -0.9);
  QCOMPARE(mGraph->data()->size(), 3);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -2.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, 1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().key, 2.0);
  
  mGraph->setData(x, y);
  mGraph->removeData(-2.1, -1.9);
  QCOMPARE(mGraph->data()->size(), 3);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -1.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, 1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().key, 2.0);
  
  mGraph->setData(x, y);
  mGraph->removeData(1.9, 2.1);
  QCOMPARE(mGraph->data()->size(), 3);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -2.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, -1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().key, 1.0);
  
  mGraph->setData(x, y);
  mGraph->removeData(-1.1, 1.1);
  QCOMPARE(mGraph->data()->size(), 2);
  QCOMPARE((mGraph->data()->begin()+0).value().key, -2.0);
  QCOMPARE((mGraph->data()->begin()+1).value().key, 2.0);
  
  mGraph->setData(x, y);
  mGraph->clearData();
  QCOMPARE(mGraph->data()->size(), 0);
  
  // add single data points:
  mGraph->addData(5, 6);
  QCOMPARE(mGraph->data()->size(), 1);
  mGraph->addData(4, 5);
  QCOMPARE(mGraph->data()->size(), 2);
  QCOMPARE((mGraph->data()->begin()+0).value().value, 5.0);
  QCOMPARE((mGraph->data()->begin()+1).value().value, 6.0);
  
  mGraph->setData(x, y);
  mGraph->addData(3, 4);
  QCOMPARE(mGraph->data()->size(), 5);
  QCOMPARE((mGraph->data()->begin()+0).value().value, 0.0);
  QCOMPARE((mGraph->data()->begin()+1).value().value, 1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().value, 2.0);
  QCOMPARE((mGraph->data()->begin()+3).value().value, 3.0);
  QCOMPARE((mGraph->data()->begin()+4).value().value, 4.0);
  
  mGraph->setData(x, y);
  mGraph->addData(QVector<double>() << 3 << 4 << 5, QVector<double>() << 4 << 5 << 6);
  QCOMPARE(mGraph->data()->size(), 7);
  QCOMPARE((mGraph->data()->begin()+0).value().value, 0.0);
  QCOMPARE((mGraph->data()->begin()+1).value().value, 1.0);
  QCOMPARE((mGraph->data()->begin()+2).value().value, 2.0);
  QCOMPARE((mGraph->data()->begin()+3).value().value, 3.0);
  QCOMPARE((mGraph->data()->begin()+4).value().value, 4.0);
  QCOMPARE((mGraph->data()->begin()+5).value().value, 5.0);
  QCOMPARE((mGraph->data()->begin()+6).value().value, 6.0);
}

void TestQCPGraph::channelFill()
{
  QCPGraph *otherGraph = mPlot->addGraph();
  
  mGraph->setBrush(Qt::black);
  mGraph->setChannelFillGraph(otherGraph);
  mPlot->replot();
  
  mGraph->setData(QVector<double>()<<1<<2<<3<<4, QVector<double>()<<4<<2<<2.5<<6);
  mPlot->replot();
  
  otherGraph->setData(QVector<double>()<<-2<<-1.5<<-1<<-0.5, QVector<double>()<<1<<1.2<<1.3<<1.4);
  mPlot->replot();
  
  mGraph->clearData();
  mPlot->replot();
  
  mPlot->removeGraph(otherGraph);
  QCOMPARE(mGraph->channelFillGraph(), (QCPGraph*)0);
  mPlot->replot();
}

