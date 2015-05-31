#include "test-qcplayout.h"

void TestQCPLayout::init()
{
  mPlot = new QCustomPlot(0);
  mPlot->show();
  QTest::qWait(150);
}


void TestQCPLayout::cleanup()
{
  delete mPlot;
}

void TestQCPLayout::layoutGridElementManagement()
{
  QCPLayoutGrid *mainLayout = mPlot->plotLayout();
  QVERIFY(mainLayout);
  QCOMPARE(mainLayout->elementCount(), 1);
  QCOMPARE(mainLayout->elementAt(0), mainLayout->element(0, 0));
  QCOMPARE(mainLayout->columnCount(), 1);
  QCOMPARE(mainLayout->rowCount(), 1);
  
  // test row/column expansion:
  QCPAxisRect *r1 = new QCPAxisRect(mPlot);
  QCPAxisRect *r2 = new QCPAxisRect(mPlot);
  QCPAxisRect *r3 = new QCPAxisRect(mPlot);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(int, int, QCPLayoutElement*) There is already an element in the specified row/column: 0 0 ");
  QVERIFY(!mainLayout->addElement(0, 0, r1));
  QVERIFY(mainLayout->addElement(0, 1, r1));
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 1);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(int, int, QCPLayoutElement*) There is already an element in the specified row/column: 0 1 ");
  QVERIFY(!mainLayout->addElement(0, 1, r2));
  QVERIFY(mainLayout->addElement(1, 0, r2));
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 2);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(int, int, QCPLayoutElement*) There is already an element in the specified row/column: 1 0 ");
  QVERIFY(!mainLayout->addElement(1, 0, r3));
  QVERIFY(mainLayout->addElement(2, 4, r3));
  QCOMPARE(mainLayout->columnCount(), 5);
  QCOMPARE(mainLayout->rowCount(), 3);
  /*
   def  r1   0    0    0
   r2   0    0    0    0
   0    0    0    0    r3
  */
  
  // simplify should remove columns 2 and 3
  mainLayout->simplify();
  /*
   def  r1   0
   r2   0    0
   0    0    r3
  */
  
  QCOMPARE(mainLayout->columnCount(), 3);
  QCOMPARE(mainLayout->rowCount(), 3);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 2 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 1 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 2 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 2 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 2 Column: 1 ");
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect());
  QCOMPARE(mainLayout->element(0, 1), r1);
  QCOMPARE(mainLayout->element(0, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), r2);
  QCOMPARE(mainLayout->element(1, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 2), r3);
      
  // remove outer right axis rect:
  QVERIFY(mainLayout->take(r3));
  delete r3;
  // now simplify should collapse to 2x2:
  mainLayout->simplify();
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 2);
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect());
  QCOMPARE(mainLayout->element(0, 1), r1);
  QCOMPARE(mainLayout->element(1, 0), r2);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 1 ");
  QCOMPARE(mainLayout->element(1, 1), (QCPLayoutElement*)0);
  
  // remove r1 axis rect:
  QCOMPARE(mainLayout->takeAt(1), r1);
  delete r1;
  // now simplify should collapse to 2x1:
  mainLayout->simplify();
  QCOMPARE(mainLayout->columnCount(), 1);
  QCOMPARE(mainLayout->rowCount(), 2);
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect(0));
  QCOMPARE(mainLayout->element(1, 0), r2);
  
  // remove last axis rects:
  QCPAxisRect *mainAxisRect = mPlot->axisRect(0);
  QCOMPARE(mainLayout->takeAt(0), mainAxisRect);
  QCOMPARE(mainLayout->takeAt(1), r2);
  delete r2;
  delete mainAxisRect;
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 0 ");
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), (QCPLayoutElement*)0);
  // now simplify should collapse to 0x0:
  mainLayout->simplify();
  QVERIFY(!mainLayout->hasElement(0, 0));
  QCOMPARE(mainLayout->columnCount(), 0);
  QCOMPARE(mainLayout->rowCount(), 0);
  QTest::ignoreMessage(QtDebugMsg, "QCPAxisRect* QCustomPlot::axisRect(int) const invalid axis rect index 0 ");
  QVERIFY(!(bool)mPlot->axisRect());
  // repopulate:
  QCPAxisRect *r4 = new QCPAxisRect(mPlot);
  QVERIFY(mainLayout->addElement(0, 0, r4));
  QCOMPARE(mPlot->axisRect(0), r4);
}

void TestQCPLayout::layoutGridInsertion()
{
  QCPLayoutGrid *mainLayout = mPlot->plotLayout();
  
  // test inserting rows/columns in front/end of initial axis rect:
  mainLayout->setColumnStretchFactor(0, 2);
  mainLayout->setRowStretchFactor(0, 3);
  mainLayout->insertColumn(0);
  QCOMPARE(mainLayout->columnCount(), 2);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 0 ");
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 1), mPlot->axisRect());
  QCOMPARE(mainLayout->columnStretchFactors().at(0), 1.0);
  QCOMPARE(mainLayout->columnStretchFactors().at(1), 2.0);
  
  mainLayout->insertColumn(2);
  QCOMPARE(mainLayout->columnCount(), 3);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 2 ");
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 1), mPlot->axisRect());
  QCOMPARE(mainLayout->element(0, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->columnStretchFactors().at(0), 1.0);
  QCOMPARE(mainLayout->columnStretchFactors().at(1), 2.0);
  QCOMPARE(mainLayout->columnStretchFactors().at(2), 1.0);
  
  mainLayout->insertRow(0);
  QCOMPARE(mainLayout->rowCount(), 2);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 1 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 2 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 2 ");
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 1), mPlot->axisRect());
  QCOMPARE(mainLayout->element(1, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->rowStretchFactors().at(0), 1.0);
  QCOMPARE(mainLayout->rowStretchFactors().at(1), 3.0);
  
  mainLayout->insertRow(2);
  QCOMPARE(mainLayout->rowCount(), 3);
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 1 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 0 Column: 2 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 1 Column: 2 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 2 Column: 0 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 2 Column: 1 ");
  QTest::ignoreMessage(QtDebugMsg, "QCPLayoutElement* QCPLayoutGrid::element(int, int) const Requested cell is empty. Row: 2 Column: 2 ");
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(0, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 1), mPlot->axisRect());
  QCOMPARE(mainLayout->element(1, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->rowStretchFactors().at(0), 1.0);
  QCOMPARE(mainLayout->rowStretchFactors().at(1), 3.0);
  QCOMPARE(mainLayout->rowStretchFactors().at(2), 1.0);
  
  // clear grid:
  mainLayout->clear();
  QCOMPARE(mainLayout->rowCount(), 0);
  QCOMPARE(mainLayout->columnCount(), 0);
  QCOMPARE(mainLayout->columnStretchFactors().size(), 0);
  QCOMPARE(mainLayout->rowStretchFactors().size(), 0);
  
  // test repopulation of completely empty grid by insertColumn:
  mainLayout->insertColumn(0);
  QCOMPARE(mainLayout->columnStretchFactors().size(), 1);
  QCOMPARE(mainLayout->rowStretchFactors().size(), 1);
  QCOMPARE(mainLayout->rowCount(), 1);
  QCOMPARE(mainLayout->columnCount(), 1);
  mainLayout->addElement(0, 0, new QCPAxisRect(mPlot));
  
  // clear grid:
  mainLayout->clear();
  
  // test repopulation of completely empty grid by insertRow:
  mainLayout->insertRow(0);
  QCOMPARE(mainLayout->columnStretchFactors().size(), 1);
  QCOMPARE(mainLayout->rowStretchFactors().size(), 1);
  QCOMPARE(mainLayout->rowCount(), 1);
  QCOMPARE(mainLayout->columnCount(), 1);
  mainLayout->addElement(0, 0, new QCPAxisRect(mPlot));
}

void TestQCPLayout::layoutGridLayout()
{
  mPlot->setGeometry(50, 50, 500, 500);
  QCPLayoutGrid *mainLayout = qobject_cast<QCPLayoutGrid*>(mPlot->plotLayout());
  mainLayout->removeAt(0); // remove initial axis rect
  // create 3x3 grid:
  for (int i=0; i<3*3; ++i)
    mainLayout->addElement(i/3, i%3, new QCPAxisRect(mPlot));
  QList<QCPAxisRect*> rlist;
  for (int i=0; i<mainLayout->elementCount(); ++i)
  {
    rlist << qobject_cast<QCPAxisRect*>(mainLayout->elementAt(i));
    rlist.last()->addAxes(QCPAxis::atLeft|QCPAxis::atRight|QCPAxis::atTop|QCPAxis::atBottom);
  }
  
  // disable spacing for testing of layout functions:
  mainLayout->setRowSpacing(0);
  mainLayout->setColumnSpacing(0);
  mPlot->replot();
  foreach (QCPAxisRect *r, rlist)
  {
    QCOMPARE(r->outerRect().width(), qRound(500/3.0));
    QCOMPARE(r->outerRect().height(), qRound(500/3.0));
  }
  
  // test stretch factors:
  mainLayout->setColumnStretchFactors(QList<double>() << 1 << 2 << 1);
  mainLayout->setRowStretchFactors(QList<double>() << 1 << 2 << 3);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(0, 1)->outerRect().width(), qRound(500/4.0*2));
  QCOMPARE(mainLayout->element(0, 2)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(0, 0)->outerRect().height(), qRound(500/6.0*1));
  QCOMPARE(mainLayout->element(1, 0)->outerRect().height(), qRound(500/6.0*2));
  QCOMPARE(mainLayout->element(2, 0)->outerRect().height(), qRound(500/6.0*3));
  QCOMPARE(mainLayout->element(2, 2)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(2, 2)->outerRect().height(), qRound(500/6.0*3));
  
  // test maximum size:
  mainLayout->element(0, 0)->setMaximumSize(100, QWIDGETSIZE_MAX);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), 100);
  QCOMPARE(mainLayout->element(0, 1)->outerRect().width(), qRound(400/3.0*2));
  QCOMPARE(mainLayout->element(0, 2)->outerRect().width(), qRound(400/3.0*1));
  
  // test minimum size:
  mainLayout->element(0, 0)->setMinimumSize(100, 200);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), 100);
  QCOMPARE(mainLayout->element(0, 0)->outerRect().height(), 200);
  QCOMPARE(mainLayout->element(1, 0)->outerRect().height(), qRound(300/5.0*2));
  QCOMPARE(mainLayout->element(2, 0)->outerRect().height(), qRound(300/5.0*3));
  
  // test minimum size hint on parent layout:
  QCOMPARE(mainLayout->minimumSizeHint(), QSize(100+50+50, 200+50+50));
  
  // add spacing:
  mainLayout->setRowSpacing(10);
  mainLayout->setColumnSpacing(15);
  // test minimum size hint on parent layout with spacing:
  QCOMPARE(mainLayout->minimumSizeHint(), QSize(100+15+50+15+50, 200+10+50+10+50));
}

void TestQCPLayout::marginGroup()
{
  mPlot->setGeometry(50, 50, 500, 500);
  
  // setup 2x2 grid layout with 4 axes on each axisrect:
  mPlot->plotLayout()->addElement(0, 1, new QCPAxisRect(mPlot));
  mPlot->plotLayout()->addElement(1, 0, new QCPAxisRect(mPlot));
  mPlot->plotLayout()->addElement(1, 1, new QCPAxisRect(mPlot));
  
  QCPAxisRect *ar0 = mPlot->axisRect(0);
  QCPAxisRect *ar2 = mPlot->axisRect(2);
  
  // remove axis tick labels so axisrect margin depends on simpler values for test:
  for (int i=0; i<4; ++i)
  {
    QList<QCPAxis*> axes = mPlot->axisRect(i)->axes();
    for (int k=0; k<axes.size(); ++k)
      axes.at(k)->setTickLabels(false);
  }
  
  // test margin propagation when both margins are auto margin and no minimum margins are set:
  ar0->setMinimumMargins(QMargins(0, 0, 0, 0));
  ar2->setMinimumMargins(QMargins(0, 0, 0, 0));
  QCPMarginGroup *leftGroup = new QCPMarginGroup(mPlot);
  QVERIFY(leftGroup->isEmpty());
  
  ar0->setMarginGroup(QCP::msLeft, leftGroup);
  ar2->setMarginGroup(QCP::msLeft, leftGroup);
  QCOMPARE(leftGroup->elements(QCP::msLeft).at(0), ar0);
  QCOMPARE(leftGroup->elements(QCP::msLeft).at(1), ar2);
  
  ar0->axis(QCPAxis::atLeft)->setPadding(10);
  ar2->axis(QCPAxis::atLeft)->setPadding(5);
  mPlot->replot();
  QCOMPARE(ar0->margins().left(), 10);
  QCOMPARE(ar2->margins().left(), 10);
  ar2->axis(QCPAxis::atLeft)->setPadding(11);
  mPlot->replot();
  QCOMPARE(ar0->margins().left(), 11);
  QCOMPARE(ar2->margins().left(), 11);
  
  // test effect of minimum margin:
  ar0->setMinimumMargins(QMargins(12, 0, 0, 0));
  mPlot->replot();
  QCOMPARE(ar0->margins().left(), 12);
  QCOMPARE(ar2->margins().left(), 12);
  
  // disable auto margin on ar2:
  ar2->setAutoMargins(QCP::msNone);
  ar2->setMargins(QMargins(9, 0, 0, 0));
  mPlot->replot();
  QCOMPARE(ar0->margins().left(), 12);
  QCOMPARE(ar2->margins().left(), 9);
  // make sure it also doesn't affect ar0 now:
  ar2->setMargins(QMargins(13, 0, 0, 0));
  mPlot->replot();
  QCOMPARE(ar0->margins().left(), 12);
}


















