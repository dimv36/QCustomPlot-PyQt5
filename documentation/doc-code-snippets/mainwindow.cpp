#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(300, 300, 500, 500);
  
  // invoke all methods of MainWindow that start with "snippet" or "website":
  for (int i=this->metaObject()->methodOffset(); i<this->metaObject()->methodCount(); ++i)
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if (QString::fromLatin1(this->metaObject()->method(i).signature()).startsWith("snippet") ||
        QString::fromLatin1(this->metaObject()->method(i).signature()).startsWith("website"))
#else
    if (this->metaObject()->method(i).methodSignature().startsWith("snippet") ||
        this->metaObject()->method(i).methodSignature().startsWith("website"))
#endif
    {
      qDebug() << "executing" << this->metaObject()->method(i).name() << "...";
      resetPlot();
      if (!this->metaObject()->method(i).invoke(this))
        qDebug() << "Failed to invoke doc-code-snippet method" << i;
    }
  }
  QTimer::singleShot(0, qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::snippetQCPColorGradient()
{
  QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(colorMap);
  
  //! [qcpcolorgradient-setgradient]
  colorMap->setGradient(QCPColorGradient::gpHot);
  //! [qcpcolorgradient-setgradient]
}

void MainWindow::snippetQCPColorScale()
{
  //! [qcpcolorscale-creation]
  QCPColorScale *colorScale = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 1, colorScale);
  colorScale->setLabel("Some Label Text");
  //! [qcpcolorscale-creation]
  
  //! [qcpcolorscale-autotickcount]
  colorScale->axis()->setAutoTickCount(3);
  //! [qcpcolorscale-autotickcount]
  
  //! [qcpcolorscale-margingroup]
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  customPlot->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  //! [qcpcolorscale-margingroup]
}

void MainWindow::snippetQCPColorMap()
{
  //! [qcpcolormap-creation-1]
  QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(colorMap);
  //! [qcpcolormap-creation-1]

  //! [qcpcolormap-creation-2]
  colorMap->data()->setSize(50, 50);
  colorMap->data()->setRange(QCPRange(0, 2), QCPRange(0, 2));
  for (int x=0; x<50; ++x)
    for (int y=0; y<50; ++y)
      colorMap->data()->setCell(x, y, qCos(x/10.0)+qSin(y/10.0));
  colorMap->setGradient(QCPColorGradient::gpPolar);
  colorMap->rescaleDataRange(true);
  customPlot->rescaleAxes();
  customPlot->replot();
  //! [qcpcolormap-creation-2]
}

void MainWindow::snippetQCPCurve()
{
  QVector<double> tData, xData, yData;
  
  //! [qcpcurve-creation-1]
  QCPCurve *newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(newCurve);
  //! [qcpcurve-creation-1]
  
  //! [qcpcurve-creation-2]
  newCurve->setName("Fermat's Spiral");
  newCurve->setData(tData, xData, yData);
  //! [qcpcurve-creation-2]
}

void MainWindow::snippetQCPItemLine()
{
  //! [qcpitemline-creation-1]
  QCPItemLine *line = new QCPItemLine(customPlot);
  customPlot->addItem(line);
  //! [qcpitemline-creation-1]
  
  //! [qcpitemline-creation-2]
  line->start->setCoords(-0.1, 0.8);
  line->end->setCoords(1.1, 0.2);
  //! [qcpitemline-creation-2]
  
  //! [qcpitemline-creation-3]
  line->start->setType(QCPItemPosition::ptAbsolute);
  line->end->setType(QCPItemPosition::ptAbsolute);
  //! [qcpitemline-creation-3]
  
  //! [qcpitemline-creation-4]
  line->start->setCoords(100, 200);
  line->end->setCoords(450, 320);
  //! [qcpitemline-creation-4]
  
  //! [qcpitemline-creation-5]
  line->setClipToAxisRect(false);
  //! [qcpitemline-creation-5]
}

void MainWindow::snippetQCPMarginGroup()
{
  QCPAxisRect *ar = new QCPAxisRect(customPlot);
  customPlot->plotLayout()->addElement(0, 1, ar);
  
  //! [qcpmargingroup-creation-1]
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  //! [qcpmargingroup-creation-1]
  
  //! [qcpmargingroup-creation-2]
  customPlot->axisRect(0)->setMarginGroup(QCP::msLeft|QCP::msRight, group);
  customPlot->axisRect(1)->setMarginGroup(QCP::msLeft|QCP::msRight, group);
  //! [qcpmargingroup-creation-2]
}

void MainWindow::snippetQCPPlotTitle()
{
  //! [qcpplottitle-creation]
  customPlot->plotLayout()->insertRow(0); // inserts an empty row above the default axis rect
  customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Your Plot Title"));
  //! [qcpplottitle-creation]
}

void MainWindow::snippetQCPLineEnding()
{
  QCPItemLine *myItemLine = new QCPItemLine(customPlot);
  //! [qcplineending-sethead]
  myItemLine->setHead(QCPLineEnding::esSpikeArrow);
  //! [qcplineending-sethead]
}

void MainWindow::snippetQCPScatterStyle()
{
  customPlot->addGraph();
  
  //! [qcpscatterstyle-creation-1]
  QCPScatterStyle myScatter;
  myScatter.setShape(QCPScatterStyle::ssCircle);
  myScatter.setPen(QPen(Qt::blue));
  myScatter.setBrush(Qt::white);
  myScatter.setSize(5);
  customPlot->graph(0)->setScatterStyle(myScatter);
  //! [qcpscatterstyle-creation-1]
  
  //! [qcpscatterstyle-creation-2]
  customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 5));
  //! [qcpscatterstyle-creation-2]
  
  //! [qcpscatterstyle-shortcreation]
  customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssPlus);
  //! [qcpscatterstyle-shortcreation]
}

void MainWindow::snippetQCPBarsGroup()
{
  QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *bars2 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(bars1);
  customPlot->addPlottable(bars2);
  
  //! [qcpbarsgroup-creation]
  QCPBarsGroup *group = new QCPBarsGroup(customPlot);
  group->append(bars1);
  group->append(bars2);
  //! [qcpbarsgroup-creation]
}

void MainWindow::snippetQCPBars()
{
  QVector<double> xData, yData;
  
  //! [qcpbars-creation-1]
  QCPBars *newBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  //! [qcpbars-creation-1]
  
  //! [qcpbars-creation-2]
  customPlot->addPlottable(newBars);
  //! [qcpbars-creation-2]
  
  //! [qcpbars-creation-3]
  newBars->setName("Country population");
  newBars->setData(xData, yData);
  //! [qcpbars-creation-3]
}

void MainWindow::snippetQCPStatisticalBox()
{
  //! [qcpstatisticalbox-creation-1]
  QCPStatisticalBox *newBox = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(newBox);
  //! [qcpstatisticalbox-creation-1]
  
  //! [qcpstatisticalbox-creation-2]
  newBox->setName("Measurement Series 1");
  newBox->setData(1000, 1, 3, 4, 5, 7);
  newBox->setOutliers(QVector<double>() << 0.5 << 0.64 << 7.2 << 7.42);
  //! [qcpstatisticalbox-creation-2]
}

void MainWindow::websiteBasicPlottingBars()
{
  QCPBars *myBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(myBars);
  // now we can modify properties of myBars:
  myBars->setName("Bars Series 1");
  QVector<double> keyData;
  QVector<double> valueData;
  keyData << 1 << 2 << 3;
  valueData << 2 << 4 << 8;
  myBars->setData(keyData, valueData);
  customPlot->rescaleAxes();
  customPlot->replot();
}




void MainWindow::resetPlot()
{
  customPlot = new QCustomPlot(this);
  setCentralWidget(customPlot);
}
