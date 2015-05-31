#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  defaultBrush(QColor(80, 215, 10, 70))
{
  ui->setupUi(this);
  setGeometry(300, 300, 500, 500);
  
  dir.setPath(qApp->applicationDirPath());
  dir.mkdir("images");
  if (!dir.cd("images"))
  {
    QMessageBox::critical(this, "Error", tr("Couldn't create and access image directory:\n%1").arg(dir.filePath("images")));
  } else
  {
    // invoke all methods of MainWindow that start with "gen":
    for (int i=this->metaObject()->methodOffset(); i<this->metaObject()->methodCount(); ++i)
    {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
      if (QString::fromLatin1(this->metaObject()->method(i).signature()).startsWith("gen"))
#else
      if (this->metaObject()->method(i).methodSignature().startsWith("gen"))
#endif
      {
        if (!this->metaObject()->method(i).invoke(this))
          qDebug() << "Failed to invoke doc-image-generator method" << i;
      }
    }
  }
  QTimer::singleShot(0, qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::genScatterStyles()
{
  resetPlot();
  // Different scatter styles in separate images:
  customPlot->xAxis->setRange(-1, 1);
  customPlot->yAxis->setRange(-1, 1);
  customPlot->addGraph();
  customPlot->graph()->addData(0, 0);
  customPlot->graph()->setPen(QPen(Qt::black));
  customPlot->setBackground(Qt::transparent);
  QMap<QCPScatterStyle::ScatterShape, QString> scatterShapes;
  scatterShapes.insert(QCPScatterStyle::ssDot, "ssDot");
  scatterShapes.insert(QCPScatterStyle::ssCross, "ssCross");
  scatterShapes.insert(QCPScatterStyle::ssPlus, "ssPlus");
  scatterShapes.insert(QCPScatterStyle::ssCircle, "ssCircle");
  scatterShapes.insert(QCPScatterStyle::ssDisc, "ssDisc");
  scatterShapes.insert(QCPScatterStyle::ssSquare, "ssSquare");
  scatterShapes.insert(QCPScatterStyle::ssDiamond, "ssDiamond");
  scatterShapes.insert(QCPScatterStyle::ssStar, "ssStar");
  scatterShapes.insert(QCPScatterStyle::ssTriangle, "ssTriangle");
  scatterShapes.insert(QCPScatterStyle::ssTriangleInverted, "ssTriangleInverted");
  scatterShapes.insert(QCPScatterStyle::ssCrossSquare, "ssCrossSquare");
  scatterShapes.insert(QCPScatterStyle::ssPlusSquare, "ssPlusSquare");
  scatterShapes.insert(QCPScatterStyle::ssCrossCircle, "ssCrossCircle");
  scatterShapes.insert(QCPScatterStyle::ssPlusCircle, "ssPlusCircle");
  scatterShapes.insert(QCPScatterStyle::ssPeace, "ssPeace");
  QMapIterator<QCPScatterStyle::ScatterShape, QString> scatterIt(scatterShapes);
  while (scatterIt.hasNext())
  {
    scatterIt.next();
    customPlot->graph()->setScatterStyle(QCPScatterStyle(scatterIt.key(), 8.5));
    QPixmap pm = customPlot->toPixmap(16, 16);
    pm.save(dir.filePath(scatterIt.value()+".png"));
  }
}

void MainWindow::genItemPixmap()
{
  resetPlot();
  QCPItemPixmap *pixmapItem = new QCPItemPixmap(customPlot);
  customPlot->addItem(pixmapItem);
  pixmapItem->setPixmap(QPixmap("./gnu.png"));
  pixmapItem->setScaled(true, Qt::IgnoreAspectRatio);
  pixmapItem->topLeft->setCoords(-0.2, 1);
  pixmapItem->bottomRight->setCoords(0.3, 0);
  labelItemAnchors(pixmapItem);
  QCPItemPixmap *pixmapItem2 = new QCPItemPixmap(customPlot);
  customPlot->addItem(pixmapItem2);
  pixmapItem2->setPixmap(QPixmap("./gnu.png"));
  pixmapItem2->setScaled(true, Qt::IgnoreAspectRatio);
  pixmapItem2->topLeft->setCoords(1.2, 0);
  pixmapItem2->bottomRight->setCoords(0.7, 1);
  labelItemAnchors(pixmapItem2);
  customPlot->savePng(dir.filePath("QCPItemPixmap.png"), 460, 160);
}

void MainWindow::genItemRect()
{
  resetPlot();
  QCPItemRect *rect = new QCPItemRect(customPlot);
  customPlot->addItem(rect);
  rect->setBrush(defaultBrush);
  rect->topLeft->setCoords(0, 1);
  rect->bottomRight->setCoords(1, 0);
  labelItemAnchors(rect);
  customPlot->savePng(dir.filePath("QCPItemRect.png"), 230, 160);
}

void MainWindow::genItemEllipse()
{
  resetPlot();
  QCPItemEllipse *ellipse = new QCPItemEllipse(customPlot);
  customPlot->addItem(ellipse);
  ellipse->setBrush(defaultBrush);
  ellipse->topLeft->setCoords(-0.15, 1.1);
  ellipse->bottomRight->setCoords(1.1, 0);
  labelItemAnchors(ellipse);
  customPlot->savePng(dir.filePath("QCPItemEllipse.png"), 300, 200);
}

void MainWindow::genItemLine()
{
  resetPlot();
  QCPItemLine *line = new QCPItemLine(customPlot);
  customPlot->addItem(line);
  line->start->setCoords(-0.1, 0.8);
  line->end->setCoords(1.1, 0.2);
  line->setHead(QCPLineEnding::esSpikeArrow);
  labelItemAnchors(line);
  customPlot->savePng(dir.filePath("QCPItemLine.png"), 230, 160);
}

void MainWindow::genItemStraightLIne()
{
  resetPlot();
  QCPItemStraightLine *straightLine = new QCPItemStraightLine(customPlot);
  customPlot->addItem(straightLine);
  straightLine->point1->setCoords(0, 0.4);
  straightLine->point2->setCoords(1, 0.6);
  labelItemAnchors(straightLine);
  customPlot->savePng(dir.filePath("QCPItemStraightLine.png"), 230, 160);
}

void MainWindow::genItemCurve()
{
  resetPlot();
  QCPItemCurve *curve = new QCPItemCurve(customPlot);
  customPlot->addItem(curve);
  curve->start->setCoords(0, 1);
  curve->startDir->setCoords(0.5, 1);
  curve->endDir->setCoords(0.7, 0.2);
  curve->end->setCoords(1, 0);
  curve->setHead(QCPLineEnding::esSpikeArrow);
  curve->setTail(QCPLineEnding::esLineArrow);
  labelItemAnchors(curve);
  customPlot->savePng(dir.filePath("QCPItemCurve.png"), 230, 160);
}

void MainWindow::genItemBracket()
{
  resetPlot();
  QCPItemBracket *bracket = new QCPItemBracket(customPlot);
  customPlot->addItem(bracket);
  bracket->left->setCoords(-0.2, 0.35);
  bracket->right->setCoords(1.2, 0.65);
  bracket->setLength(12);
  labelItemAnchors(bracket, 8, true, false);
  customPlot->savePng(dir.filePath("QCPItemBracket.png"), 230, 160);
  customPlot->clearItems();
  
  // generate image of multiple bracket lengths/styles:
  for (int i=0; i<4; ++i)
  {
    QCPItemBracket *bracket = new QCPItemBracket(customPlot);
    customPlot->addItem(bracket);
    bracket->setStyle(QCPItemBracket::bsCalligraphic);
    bracket->left->setCoords(-0.35+i*0.18, 0.95);
    bracket->right->setCoords(-0.15+i*0.18, 0.05);
    bracket->setLength(10+i*5);
    labelItemAnchors(bracket, 0, true, false);
    QCPItemText *label = new QCPItemText(customPlot);
    customPlot->addItem(label);
    label->setText(QString::number(bracket->length()));
    label->position->setParentAnchor(bracket->right);
    label->position->setCoords(-5, 20);
    label->setFont(QFont(font().family(), 9));
  }
  for (int i=0; i<4; ++i)
  {
    QCPItemBracket *bracket = new QCPItemBracket(customPlot);
    customPlot->addItem(bracket);
    bracket->setStyle(QCPItemBracket::bsSquare);
    bracket->left->setCoords(0.55+i*0.18, 0.95);
    bracket->right->setCoords(0.75+i*0.18, 0.05);
    bracket->setLength(10+i*5);
    labelItemAnchors(bracket, 0, true, false);
    QCPItemText *label = new QCPItemText(customPlot);
    customPlot->addItem(label);
    label->setText(QString::number(bracket->length()));
    label->position->setParentAnchor(bracket->right);
    label->position->setCoords(-5, 20);
    label->setFont(QFont(font().family(), 9));
  }
  QCPItemText *topLabel1 = new QCPItemText(customPlot);
  customPlot->addItem(topLabel1);
  topLabel1->setText("bsCalligraphic");
  topLabel1->position->setCoords(-0.05, 1.1);
  topLabel1->setFont(QFont(font().family(), 10));
  QCPItemText *topLabel2 = new QCPItemText(customPlot);
  customPlot->addItem(topLabel2);
  topLabel2->setText("bsSquare");
  topLabel2->position->setCoords(0.85, 1.1);
  topLabel2->setFont(QFont(font().family(), 10));
  customPlot->savePng(dir.filePath("QCPItemBracket-length.png"), 450, 200);
}

void MainWindow::genItemText()
{
  resetPlot();
  QCPItemText *text = new QCPItemText(customPlot);
  customPlot->addItem(text);
  text->position->setCoords(0.5, 0.5);
  text->setText("QCustomPlot\nWidget");
  text->setFont(QFont(font().family(), 24));
  text->setRotation(12);
  text->setBrush(defaultBrush);
  labelItemAnchors(text);
  customPlot->savePng(dir.filePath("QCPItemText.png"), 300, 170);
}

void MainWindow::genItemTracer()
{
  resetPlot();
  customPlot->addGraph();
  QVector<double> x(50), y(50);
  for (int i=0; i<50; ++i)
  {
    x[i] = -0.4+1.8*i/49.0;
    y[i] = qSin(x[i]*M_PI*0.9)*0.4+0.4;
  }
  customPlot->graph()->setData(x, y);
  customPlot->graph()->setPen(QPen(Qt::red));
  QCPItemTracer *tracer = new QCPItemTracer(customPlot);
  customPlot->addItem(tracer);
  tracer->setStyle(QCPItemTracer::tsCrosshair);
  tracer->setGraph(customPlot->graph());
  tracer->setGraphKey(0.8);
  labelItemAnchors(tracer, 8, true, false);
  customPlot->savePng(dir.filePath("QCPItemTracer.png"), 230, 160);
}

void MainWindow::genLineEnding()
{
  resetPlot();
  QMetaEnum endingStyleEnum = QCPLineEnding::staticMetaObject.enumerator(QCPLineEnding::staticMetaObject.indexOfEnumerator("EndingStyle"));
  double offset = -0.2;
  double step = 1.4/((double)endingStyleEnum.keyCount()-1);
  for (int i=0; i<endingStyleEnum.keyCount(); ++i)
  {
    QCPLineEnding ending(static_cast<QCPLineEnding::EndingStyle>(endingStyleEnum.value(i)));
    QString endingName(endingStyleEnum.key(i));
    
    if (ending.style() == QCPLineEnding::esSkewedBar)
      ending.setInverted(true);
    
    QCPItemLine *line = new QCPItemLine(customPlot);
    line->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap));
    customPlot->addItem(line);
    line->start->setCoords(offset+i*step-0.1, -0.2);
    line->end->setCoords(offset+i*step, 0.5);
    line->setHead(ending);
    QCPItemText *text = new QCPItemText(customPlot);
    customPlot->addItem(text);
    text->position->setParentAnchor(line->end);
    text->position->setCoords(8, -15-(i%2)*15);
    text->setFont(QFont(font().family(), 8));
    text->setText(endingName);
  }

  customPlot->savePng(dir.filePath("QCPLineEnding.png"), 500, 100);
}

void MainWindow::genMarginGroup()
{
  resetPlot();
  
  customPlot->plotLayout()->clear();
  customPlot->plotLayout()->addElement(0, 0, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(0, 1, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(1, 0, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(1, 1, new QCPAxisRect(customPlot));
  
  foreach (QCPAxisRect *r, customPlot->axisRects())
    r->axis(QCPAxis::atBottom)->setTickLabels(false);
  
  QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
  customPlot->axisRect(0)->setMarginGroup(QCP::msLeft, marginGroup);
  customPlot->axisRect(2)->setMarginGroup(QCP::msLeft, marginGroup);
  
  customPlot->axisRect(0)->axis(QCPAxis::atLeft)->setRange(0, 1300);
  customPlot->axisRect(1)->axis(QCPAxis::atLeft)->setRange(0, 1300);
  customPlot->axisRect(0)->axis(QCPAxis::atLeft)->setLabel("y");
  customPlot->axisRect(1)->axis(QCPAxis::atLeft)->setLabel("y");
  
  customPlot->plotLayout()->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
  customPlot->plotLayout()->setMargins(QMargins(0, 25, 0, 0));
  
  QFont textFont;
  textFont.setBold(true);
  QCPItemText *leftCaption = new QCPItemText(customPlot);
  customPlot->addItem(leftCaption);
  leftCaption->position->setType(QCPItemPosition::ptViewportRatio);
  leftCaption->setClipToAxisRect(false);
  leftCaption->position->setCoords(0.25, 0);
  leftCaption->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  leftCaption->setText("left sides in margin group");
  leftCaption->setFont(textFont);
  QCPItemText *rightCaption = new QCPItemText(customPlot);
  customPlot->addItem(rightCaption);
  rightCaption->position->setType(QCPItemPosition::ptViewportRatio);
  rightCaption->position->setCoords(0.75, 0);
  rightCaption->setClipToAxisRect(false);
  rightCaption->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  rightCaption->setText("no margin group");
  rightCaption->setFont(textFont);
  
  QCPItemLine *splitter = new QCPItemLine(customPlot);
  splitter->start->setType(QCPItemPosition::ptViewportRatio);
  splitter->start->setCoords(0.5, 0);
  splitter->end->setType(QCPItemPosition::ptViewportRatio);
  splitter->end->setCoords(0.5, 1);
  splitter->setClipToAxisRect(false);
  splitter->setPen(QPen(Qt::gray, 0, Qt::DashLine));
  
  customPlot->savePng(dir.filePath("QCPMarginGroup.png"), 400, 400);
}

void MainWindow::genAxisRectSpacingOverview()
{
  resetPlot();
 
  customPlot->xAxis->setRange(-0.4, 1.4);
  customPlot->yAxis->setRange(100, 900);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->xAxis->setTickLabels(false);
  customPlot->axisRect()->setAutoMargins(QCP::msNone);
  customPlot->axisRect()->setMargins(QMargins(200, 50, 20, 165));
  customPlot->axisRect()->setBackground(QColor(245, 245, 245));
  
  customPlot->yAxis->setLabel("Axis Label");
  customPlot->yAxis->setOffset(30);
  customPlot->yAxis->setTickLabelPadding(30);
  customPlot->yAxis->setLabelPadding(30);
  customPlot->yAxis->setTickLengthOut(5);
  customPlot->yAxis->setSubTickLengthOut(2);
  
  addBracket(QPointF(200-95-27-17, 30), QPointF(1, 30), "Padding (if auto margins enabled)", QPointF(-25, -5), false, Qt::AlignLeft|Qt::AlignBottom);
  addBracket(QPointF(1, 370), QPointF(200, 370), "Margin", QPointF(0, 5), false, Qt::AlignHCenter|Qt::AlignTop);
  addBracket(QPointF(200-30, 240), QPointF(200, 240), "Axis offset", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-35, 250), QPointF(200-30, 250), "Tick length out", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-65, 240), QPointF(200-35, 240), "Tick label padding", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-95-25, 240), QPointF(200-65-25, 240), "Label padding", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  
  QCPItemLine *leftBorder = new QCPItemLine(customPlot);
  customPlot->addItem(leftBorder);
  leftBorder->setClipToAxisRect(false);
  leftBorder->start->setType(QCPItemPosition::ptViewportRatio);
  leftBorder->end->setType(QCPItemPosition::ptViewportRatio);
  leftBorder->start->setCoords(0, 0);
  leftBorder->end->setCoords(0, 1);
  leftBorder->setPen(QPen(Qt::gray, 0, Qt::DashLine));
  
  QCPItemText *axisRectLabel = new QCPItemText(customPlot);
  customPlot->addItem(axisRectLabel);
  axisRectLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
  axisRectLabel->position->setCoords(0.5, 0.5);
  axisRectLabel->setFont(QFont(QFont().family(), 16));
  axisRectLabel->setText("QCPAxisRect");
  axisRectLabel->setColor(QColor(0, 0, 0, 60));
  
  customPlot->savePng(dir.filePath("AxisRectSpacingOverview.png"), 400, 400);
}

void MainWindow::genAxisNamesOverview()
{
  resetPlot();
 
  customPlot->xAxis->setRange(1, 2);
  customPlot->yAxis->setRange(-50, 150);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->xAxis->setTickLabels(false);
  customPlot->axisRect()->setAutoMargins(QCP::msNone);
  customPlot->axisRect()->setMargins(QMargins(250, 50, 20, 65));
  customPlot->yAxis->setLabel("Axis Label");
  
  addArrow(QPointF(216, 70), QPointF(150, 32), "Tick label", Qt::AlignRight|Qt::AlignVCenter);
  addArrow(QPointF(187, 110), QPointF(130, 76), "Axis label", Qt::AlignRight|Qt::AlignVCenter);
  addArrow(QPointF(260, 77), QPointF(300, 77), "Tick", Qt::AlignLeft|Qt::AlignVCenter);
  addArrow(QPointF(255, 95), QPointF(300, 95), "Sub tick", Qt::AlignLeft|Qt::AlignVCenter);
  addArrow(QPointF(297, 193), QPointF(297, 250), "Zero line", Qt::AlignHCenter|Qt::AlignTop);
  addArrow(QPointF(354, 165), QPointF(354, 266), "Grid line", Qt::AlignHCenter|Qt::AlignTop);
  addBracket(QPointF(263, 132), QPointF(263, 105), "Tick step", QPointF(8, 0), false, Qt::AlignLeft|Qt::AlignVCenter, QCPItemBracket::bsCurly);
  
  customPlot->savePng(dir.filePath("AxisNamesOverview.png"), 450, 300);
}

void MainWindow::genLayoutsystem_AddingPlotTitle()
{
  resetPlot(false);
  
  //! [plottitle-example]
  // first we create and prepare a plot title layout element:
  QCPPlotTitle *title = new QCPPlotTitle(customPlot);
  title->setText("Plot Title Example");
  title->setFont(QFont("sans", 12, QFont::Bold));
  // then we add it to the main plot layout:
  customPlot->plotLayout()->insertRow(0); // insert an empty row above the axis rect
  customPlot->plotLayout()->addElement(0, 0, title); // place the title in the empty cell we've just created
  //! [plottitle-example]
  
  customPlot->savePng(dir.filePath("layoutsystem-addingplottitle.png"), 300, 200);
}

void MainWindow::genLayoutsystem_MultipleAxisRects()
{
  resetPlot(false);
  
  //! [layoutsystem-example]
  customPlot->plotLayout()->clear(); // let's start from scratch and remove the default axis rect
  // add the first axis rect in second row (row index 1):
  QCPAxisRect *bottomAxisRect = new QCPAxisRect(customPlot);
  customPlot->plotLayout()->addElement(1, 0, bottomAxisRect);
  // create a sub layout that we'll place in first row:
  QCPLayoutGrid *subLayout = new QCPLayoutGrid;
  customPlot->plotLayout()->addElement(0, 0, subLayout);
  // add two axis rects in the sub layout next to each other:
  QCPAxisRect *leftAxisRect = new QCPAxisRect(customPlot);
  QCPAxisRect *rightAxisRect = new QCPAxisRect(customPlot);
  subLayout->addElement(0, 0, leftAxisRect);
  subLayout->addElement(0, 1, rightAxisRect);
  subLayout->setColumnStretchFactor(0, 3); // left axis rect shall have 60% of width
  subLayout->setColumnStretchFactor(1, 2); // right one only 40% (3:2 = 60:40)
  // since we've created the axis rects and axes from scratch, we need to place them on
  // according layers, if we don't want the grid to be drawn above the axes etc.
  // place the axis on "axes" layer and grids on the "grid" layer, which is below "axes":
  QList<QCPAxis*> allAxes;
  allAxes << bottomAxisRect->axes() << leftAxisRect->axes() << rightAxisRect->axes();
  foreach (QCPAxis *axis, allAxes)
  {
    axis->setLayer("axes");
    axis->grid()->setLayer("grid");
  }
  //! [layoutsystem-example]
  
  customPlot->savePng(dir.filePath("layoutsystem-multipleaxisrects.png"), 400, 300);
}

void MainWindow::genQCPGraph()
{
  // generate main doc image of plottable:
  resetPlot(true);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  
  QVector<double> x1, y1, x2, y2, err2;
  for (int i=0; i<100; ++i)
  {
    x1 << i/99.0*10;
    if (i != 50)
      y1 << qSin((x1.last()-5.0)*3)/((x1.last()-5.0)*3);
    else
      y1 << 1;
  }
  x2 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
  y2 << 1 << 1.1 << 1.5 << 1.6 << 1.4 << 1.35 << 1.3 << 1.2 << 1.15;
  err2 << 0.25 << 0.3 << 0.34 << 0.35 << 0.3 << 0.15 << 0.17 << 0.23 << 0.24;
  
  customPlot->addGraph();
  customPlot->graph()->setData(x1, y1);
  customPlot->graph()->setBrush(QColor(255, 50, 50, 25));
  
  customPlot->addGraph();
  customPlot->graph()->setDataValueError(x2, y2, err2);
  customPlot->graph()->setErrorType(QCPGraph::etValue);
  customPlot->graph()->setLineStyle(QCPGraph::lsNone);
  customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::black, QColor(0, 0, 0, 25), 6));
  
  customPlot->xAxis->setRange(-1, 11);
  customPlot->yAxis->setRange(-0.5, 2.1);
  
  customPlot->savePng(dir.filePath("QCPGraph.png"), 450, 200);
}

void MainWindow::genQCPCurve()
{
  // generate main doc image of plottable:
  resetPlot(true);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  
  QVector<double> x1, y1;
  for (int i=-20; i<70; ++i)
  {
    double t = i/99.0*2*M_PI;
    x1 << 4*qCos(t);
    y1 << qSin(t*2);
  }
  QCPCurve *curve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(curve);
  curve->setData(x1, y1);
  
  customPlot->xAxis->setRange(-5, 5);
  customPlot->yAxis->setRange(-2, 2);
  
  customPlot->savePng(dir.filePath("QCPCurve.png"), 450, 200);
}

void MainWindow::genQCPBars()
{
  // generate main doc image of plottable:
  resetPlot(true);
  
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  
  QVector<double> x1, y1, y2;
  x1 << -2 << -1 << 0 << 1 << 2;
  y1 << 0.5 << -0.4 << 0.2 << 0.8 << 1.2;
  y2 << 0.3 << -0.2 << 0.2 << 0.3 << 0.4;
  
  QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *bars2 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(bars1);
  customPlot->addPlottable(bars2);
  bars1->setData(x1, y1);
  bars2->setData(x1, y2);
  bars2->moveAbove(bars1);
  
  bars1->setAntialiased(false);
  bars2->setAntialiased(false);
  bars2->setPen(QPen(QColor(200, 50, 50)));
  bars2->setBrush(QColor(255, 50, 50, 25));
  
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(1);
  customPlot->xAxis->setRange(-3, 3);
  customPlot->yAxis->setRange(-1, 2);
  
  customPlot->savePng(dir.filePath("QCPBars.png"), 450, 200);
}

void MainWindow::genQCPStatisticalBox()
{
  // generate main doc image of plottable:
  resetPlot(true);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  
  QCPStatisticalBox *box1 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
  QCPStatisticalBox *box2 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
  QCPStatisticalBox *box3 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(box1);
  customPlot->addPlottable(box2);
  customPlot->addPlottable(box3);
  box1->setData(-1, -1.2, -0.35, 0.1, 0.4, 1.1);
  box2->setData(0, -1.4, -0.7, -0.1, 0.34, 0.9);
  box3->setData(1, -0.6, -0.2, 0.15, 0.6, 1.2);
  box1->setBrush(QColor(0, 0, 255, 20));
  box2->setBrush(QColor(0, 0, 255, 20));
  box3->setBrush(QColor(0, 0, 255, 20));
  
  box3->setOutliers(QVector<double>() << -0.9 << -1 << 1.35 << 1.4 << 1.1);
  box3->setOutlierStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, 5));
  
  customPlot->xAxis->setRange(-3, 3);
  customPlot->yAxis->setRange(-1.5, 1.5);
  
  customPlot->savePng(dir.filePath("QCPStatisticalBox.png"), 450, 200);
}

void MainWindow::genQCPColorMap()
{
  // generate main doc image of plottable:
  resetPlot(true);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  customPlot->xAxis->setAutoTickCount(6);
  customPlot->yAxis->setAutoTickCount(6);
  
  QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(colorMap);
  int nx = 200;
  int ny = 100;
  colorMap->data()->setSize(nx, ny);
  colorMap->data()->setRange(QCPRange(0, 10), QCPRange(0, 10));
  colorMap->setInterpolate(true);
  colorMap->setTightBoundary(true);
  for (int x=0; x<nx; ++x)
    for (int y=0; y<ny; ++y)
      colorMap->data()->setCell(x, y, (qExp(-qSqrt((x-nx*0.2)*(x-nx*0.2)+(y-ny*0.2)*(y-ny*0.2))/200.0)+
                                       qExp(-qSqrt((x-nx*0.75)*(x-nx*0.75)+(y-ny*0.75)*(y-ny*0.75))/80.0)-
                                       qExp(-qSqrt((x-nx*0.33)*(x-nx*0.33)+(y-ny*0.6)*(y-ny*0.6))/100.0)+0.436285)/1.53251*2-1);
  QCPColorGradient gradient = colorMap->gradient();
  gradient.loadPreset(QCPColorGradient::gpJet);
  colorMap->setGradient(gradient);
  colorMap->rescaleDataRange(true);
  customPlot->rescaleAxes();
  customPlot->xAxis->scaleRange(1.25, customPlot->xAxis->range().center());
  customPlot->yAxis->scaleRange(1.25, customPlot->yAxis->range().center());
  
  customPlot->savePng(dir.filePath("QCPColorMap.png"), 450, 200);
}

void MainWindow::genQCPFinancial()
{
  // generate main doc image of plottable:
  resetPlot(false);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->xAxis->setBasePen(Qt::NoPen);
  customPlot->yAxis->setBasePen(Qt::NoPen);
  customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  customPlot->xAxis->setAutoTickCount(6);
  customPlot->yAxis->setAutoTickCount(6);

  // generate two sets of random walk data (one for candlestick and one for ohlc chart):
  int n = 500;
  QVector<double> time(n), value1(n), value2(n);
  QDateTime start = QDateTime(QDate(2014, 6, 11));
  start.setTimeSpec(Qt::UTC);
  double startTime = start.toTime_t();
  double binSize = 3600*24;
  time[0] = startTime;
  value1[0] = 60;
  value2[0] = 60-75;
  qsrand(9);
  for (int i=1; i<n; ++i)
  {
    time[i] = startTime + 3600*i;
    value1[i] = value1[i-1] + (qrand()/(double)RAND_MAX-0.5)*10;
    qrand();
    value2[i] = value1[i]-75;
  }
  
  // create candlestick chart:
  QCPFinancial *candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(candlesticks);
  QCPFinancialDataMap data1 = QCPFinancial::timeSeriesToOhlc(time, value1, binSize, startTime);
  candlesticks->setChartStyle(QCPFinancial::csCandlestick);
  candlesticks->setData(&data1, true);
  candlesticks->setWidth(binSize*0.9);
  candlesticks->setTwoColored(true);
  candlesticks->setBrushPositive(QColor(245, 245, 245));
  candlesticks->setBrushNegative(QColor(0, 0, 0));
  candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
  candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));

  // create ohlc chart:
  QCPFinancial *ohlc = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(ohlc);
  QCPFinancialDataMap data2 = QCPFinancial::timeSeriesToOhlc(time, value2, binSize, startTime);
  ohlc->setChartStyle(QCPFinancial::csOhlc);
  ohlc->setData(&data2, true);
  ohlc->setWidth(binSize*0.75);
  ohlc->setTwoColored(true);

  customPlot->rescaleAxes();
  customPlot->xAxis->scaleRange(1.25, customPlot->xAxis->range().center());
  customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
  
  customPlot->savePng(dir.filePath("QCPFinancial.png"), 450, 250);
}

void MainWindow::genQCPColorScale()
{
  // generate main doc image of plottable:
  resetPlot(false);
  customPlot->axisRect()->setupFullAxesBox(true);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  customPlot->axisRect()->setMarginGroup(QCP::msAll, group);
  
  QCPColorScale *colorScaleV = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 1, colorScaleV);
  colorScaleV->setGradient(QCPColorGradient::gpThermal);
  colorScaleV->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  colorScaleV->setDataScaleType(QCPAxis::stLogarithmic);
  colorScaleV->setDataRange(QCPRange(1, 1000));
  colorScaleV->axis()->setSubTickCount(9);
  colorScaleV->axis()->setNumberFormat("eb");
  colorScaleV->axis()->setNumberPrecision(0);
  
  QCPColorScale *colorScaleH = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(1, 0, colorScaleH);
  QCPColorGradient gradient(QCPColorGradient::gpGrayscale);
  gradient.setLevelCount(20);
  colorScaleH->setGradient(gradient);
  colorScaleH->setMarginGroup(QCP::msLeft|QCP::msRight, group);
  colorScaleH->setType(QCPAxis::atBottom);
  colorScaleH->setMinimumMargins(QMargins());
  colorScaleH->setDataRange(QCPRange(-5, 5));
  
  customPlot->savePng(dir.filePath("QCPColorScale.png"), 450, 200);
}

void MainWindow::genQCPColorGradient()
{
  QMetaEnum presetEnum = QCPColorGradient::staticMetaObject.enumerator(QCPColorGradient::staticMetaObject.indexOfEnumerator("GradientPreset"));
  int subImageWidth = 200;
  int subImageHeight = 150;
  int imageColumns = 3;
  QPixmap collage(subImageWidth*imageColumns, subImageHeight*((presetEnum.keyCount()-1)/imageColumns+1));
  QPainter collagePainter(&collage);
  for (int i=0; i<presetEnum.keyCount(); ++i)
  {
    resetPlot(false);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(false);
    customPlot->axisRect()->setupFullAxesBox(true);
    QMargins m = customPlot->axisRect()->minimumMargins();
    m.setTop(m.top() + 10);
    m.setRight(0);
    customPlot->axisRect()->setMinimumMargins(m); // make some space for label
    QString gradientName(presetEnum.key(i));
    
    QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(colorMap);
    int nx = 400;
    int ny = 400;
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, 10), QCPRange(0, 10));
    colorMap->setInterpolate(true);
    colorMap->setTightBoundary(false);
    for (int x=0; x<nx; ++x)
      for (int y=0; y<ny; ++y)
        colorMap->data()->setCell(x, y, (qExp(-qSqrt((x-310)*(x-310)+(y-260)*(y-260))/200.0)+
                                         qExp(-qSqrt((x-200)*(x-200)+(y-290)*(y-290))/80.0)-
                                         qExp(-qSqrt((x-180)*(x-180)+(y-140)*(y-140))/200.0)+0.436285)/1.53251*2-1);
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setAutoTickStep(false);
    colorScale->axis()->setTickStep(1);
    QCPMarginGroup *group = new QCPMarginGroup(customPlot);
    colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    customPlot->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    QCPColorGradient gradient = colorMap->gradient();
    gradient.loadPreset(static_cast<QCPColorGradient::GradientPreset>(presetEnum.value(i)));
    gradient.setPeriodic(false);
    colorMap->setGradient(gradient);
    colorMap->rescaleDataRange(true);
    customPlot->rescaleAxes();
    
    QCPItemText *text = new QCPItemText(customPlot);
    customPlot->addItem(text);
    text->setClipToAxisRect(false);
    text->position->setType(QCPItemPosition::ptAxisRectRatio);
    text->position->setCoords(0.5, -0.12);
    text->setFont(QFont(font().family(), 10));
    text->setText(gradientName);
    collagePainter.drawPixmap(subImageWidth*(i%imageColumns), subImageHeight*(i/imageColumns), customPlot->toPixmap(subImageWidth, subImageHeight));
  }
  collage.save(dir.filePath("QCPColorGradient.png"));
}

void MainWindow::genQCPBarsGroup()
{
  resetPlot(false);
  //! [qcpbarsgroup-example]
  QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
  QVector<double> datay1 = QVector<double>() << 0.6 << 0.5 << 0.3 << 0.15;
  QVector<double> datay2 = QVector<double>() << 0.3 << 0.28 << 0.2 << 0.1;
  QVector<double> datay3 = QVector<double>() << 0.33 << 0.31 << 0.27 << 0.13;
  
  QCPBarsGroup *group1 = new QCPBarsGroup(customPlot);
  QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(bars);
  bars->setData(datax, datay1);
  bars->setBrush(QColor(0, 0, 255, 50));
  bars->setPen(QColor(0, 0, 255));
  bars->setWidth(0.15);
  bars->setBarsGroup(group1);
  bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(bars);
  bars->setData(datax, datay2);
  bars->setBrush(QColor(180, 00, 120, 50));
  bars->setPen(QColor(180, 00, 120));
  bars->setWidth(0.15);
  bars->setBarsGroup(group1);
  bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(bars);
  bars->setData(datax, datay3);
  bars->setBrush(QColor(255, 154, 0, 50));
  bars->setPen(QColor(255, 154, 0));
  bars->setWidth(0.15);
  bars->setBarsGroup(group1);

  customPlot->xAxis->setRange(0.1, 4.9);
  customPlot->yAxis->setRange(0, 0.7);
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(1);
  //! [qcpbarsgroup-example]
  customPlot->savePng(dir.filePath("QCPBarsGroup.png"), 450, 200);
}

void MainWindow::genQCPColorMap_Interpolate()
{
  resetPlot(false);
  QCPAxisRect *ar1 = customPlot->axisRect();
  QCPAxisRect *ar2 = new QCPAxisRect(customPlot);
  ar1->axis(QCPAxis::atLeft)->setTickLabels(false);
  ar1->axis(QCPAxis::atBottom)->setTickLabels(false);
  ar2->axis(QCPAxis::atLeft)->setTickLabels(false);
  ar2->axis(QCPAxis::atBottom)->setTickLabels(false);
  ar2->axis(QCPAxis::atLeft)->setLayer("axes");
  ar2->axis(QCPAxis::atBottom)->setLayer("axes");
  customPlot->plotLayout()->addElement(0, 1, ar2);
  QCPColorMap *cm1 = new QCPColorMap(ar1->axis(QCPAxis::atBottom), ar1->axis(QCPAxis::atLeft));
  QCPColorMap *cm2 = new QCPColorMap(ar2->axis(QCPAxis::atBottom), ar2->axis(QCPAxis::atLeft));
  cm1->data()->setSize(10, 10);
  cm1->data()->setRange(cm1->keyAxis()->range(), cm1->valueAxis()->range());
  for (int x=0; x<10; ++x)
    for (int y=0; y<10; ++y)
      cm1->data()->setCell(x, y, qCos(x/9.0*3.14)+qSin(y/9.0*3.14));
  cm2->setData(cm1->data(), true);
  cm1->setInterpolate(true);
  cm2->setInterpolate(false);
  cm1->rescaleDataRange();
  cm2->rescaleDataRange();
  customPlot->plotLayout()->setMargins(QMargins(0, 5, 0, 0));
  
  QCPItemText *t1 = new QCPItemText(customPlot);
  customPlot->addItem(t1);
  t1->setText("Interpolate true");
  t1->position->setType(QCPItemPosition::ptAxisRectRatio);
  t1->position->setAxisRect(ar1);
  t1->position->setCoords(0.5, -0.02);
  t1->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t1->setClipToAxisRect(false);
  QCPItemText *t2 = new QCPItemText(customPlot);
  customPlot->addItem(t2);
  t2->setText("Interpolate false");
  t2->position->setType(QCPItemPosition::ptAxisRectRatio);
  t2->position->setAxisRect(ar2);
  t2->position->setCoords(0.5, -0.02);
  t2->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t2->setClipToAxisRect(false);
  
  customPlot->savePng(dir.filePath("QCPColorMap-interpolate.png"), 450, 200);
}

void MainWindow::genQCPColorMap_TightBoundary()
{
  resetPlot(false);
  QCPAxisRect *ar1 = customPlot->axisRect();
  QCPAxisRect *ar2 = new QCPAxisRect(customPlot);
  foreach (QCPAxis *axis, QList<QCPAxis*>() << ar1->axes() << ar2->axes())
  {
    axis->setTickLabels(false);
    axis->grid()->setLayer("axes");
    axis->grid()->setZeroLinePen(Qt::NoPen);
    axis->setLayer("axes");
    axis->setAutoTickStep(false);
    axis->setTickStep(2);
  }
  customPlot->plotLayout()->setMargins(QMargins(0, 5, 0, 0));
  customPlot->plotLayout()->addElement(0, 1, ar2);
  QCPColorMap *cm1 = new QCPColorMap(ar1->axis(QCPAxis::atBottom), ar1->axis(QCPAxis::atLeft));
  QCPColorMap *cm2 = new QCPColorMap(ar2->axis(QCPAxis::atBottom), ar2->axis(QCPAxis::atLeft));
  cm1->data()->setSize(7, 7);
  cm1->data()->setRange(QCPRange(-2, 2), QCPRange(-2, 2));
  for (int x=0; x<7; ++x)
    for (int y=0; y<7; ++y)
      cm1->data()->setCell(x, y, qCos(x/6.0*3.14)+qSin(y/6.0*3.14));
  cm2->setData(cm1->data(), true);
  cm1->setInterpolate(false);
  cm2->setInterpolate(false);
  cm1->setTightBoundary(true);
  cm2->setTightBoundary(false);
  cm1->rescaleDataRange();
  cm2->rescaleDataRange();
  cm1->rescaleAxes();
  cm2->rescaleAxes();
  foreach (QCPAxis *axis, QList<QCPAxis*>() << ar1->axes() << ar2->axes())
    axis->setRange(-3, 3);
  
  QCPItemText *t1 = new QCPItemText(customPlot);
  customPlot->addItem(t1);
  t1->setText("TightBoundary true");
  t1->position->setType(QCPItemPosition::ptAxisRectRatio);
  t1->position->setAxisRect(ar1);
  t1->position->setCoords(0.5, -0.02);
  t1->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t1->setClipToAxisRect(false);
  QCPItemText *t2 = new QCPItemText(customPlot);
  customPlot->addItem(t2);
  t2->setText("TightBoundary false");
  t2->position->setType(QCPItemPosition::ptAxisRectRatio);
  t2->position->setAxisRect(ar2);
  t2->position->setCoords(0.5, -0.02);
  t2->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t2->setClipToAxisRect(false);
  
  customPlot->savePng(dir.filePath("QCPColorMap-tightboundary.png"), 450, 200);
}

void MainWindow::genQCPColorGradient_LevelCount()
{
  resetPlot(false);
  customPlot->plotLayout()->clear();
  
  QCPAxisRect *r1 = new QCPAxisRect(customPlot, true);
  QCPAxisRect *r2 = new QCPAxisRect(customPlot, true);
  r1->setupFullAxesBox(true);
  r2->setupFullAxesBox(true);
  r1->axis(QCPAxis::atLeft)->setTickLabels(false);
  r1->axis(QCPAxis::atBottom)->setTickLabels(false);
  r2->axis(QCPAxis::atLeft)->setTickLabels(false);
  r2->axis(QCPAxis::atBottom)->setTickLabels(false);
  QCPColorScale *scale1 = new QCPColorScale(customPlot);
  QCPColorScale *scale2 = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 0, r1);
  customPlot->plotLayout()->addElement(0, 1, scale1);
  customPlot->plotLayout()->addElement(0, 2, r2);
  customPlot->plotLayout()->addElement(0, 3, scale2);
  QCPColorGradient gradient;
  scale1->setGradient(gradient);
  gradient.setLevelCount(10);
  scale2->setGradient(gradient);
  
  QMargins m = r1->minimumMargins();
  m.setTop(m.top() + 10); // make some space for label
  m.setRight(0);
  r1->setMinimumMargins(m);
  r2->setMinimumMargins(m);

  QCPColorMap *map1 = new QCPColorMap(r1->axis(QCPAxis::atBottom), r1->axis(QCPAxis::atLeft));
  QCPColorMap *map2 = new QCPColorMap(r2->axis(QCPAxis::atBottom), r2->axis(QCPAxis::atLeft));
  customPlot->addPlottable(map1);
  customPlot->addPlottable(map2);
  int nx = 400;
  int ny = 400;
  map1->data()->setSize(nx, ny);
  map1->data()->setRange(QCPRange(0, 10), QCPRange(0, 10));
  for (int x=0; x<nx; ++x)
    for (int y=0; y<ny; ++y)
      map1->data()->setCell(x, y, (qExp(-qSqrt((x-310)*(x-310)+(y-260)*(y-260))/200.0)+
                                       qExp(-qSqrt((x-200)*(x-200)+(y-290)*(y-290))/80.0)-
                                       qExp(-qSqrt((x-180)*(x-180)+(y-140)*(y-140))/200.0)+0.436285)/1.53251*2-1);
  map2->setData(map1->data(), true);
  map1->setColorScale(scale1);
  map2->setColorScale(scale2);
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  r1->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  r2->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  scale1->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  scale2->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  
  map1->rescaleDataRange();
  map2->rescaleDataRange();
  customPlot->rescaleAxes();
  
  QCPItemText *t1 = new QCPItemText(customPlot);
  customPlot->addItem(t1);
  t1->setText("350 Levels");
  t1->position->setType(QCPItemPosition::ptAxisRectRatio);
  t1->position->setAxisRect(r1);
  t1->position->setCoords(0.5, -0.02);
  t1->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t1->setClipToAxisRect(false);
  QCPItemText *t2 = new QCPItemText(customPlot);
  customPlot->addItem(t2);
  t2->setText("10 Levels");
  t2->position->setType(QCPItemPosition::ptAxisRectRatio);
  t2->position->setAxisRect(r2);
  t2->position->setCoords(0.5, -0.02);
  t2->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t2->setClipToAxisRect(false);
  
  QList<QCPAxis*> allAxes;
  allAxes << r1->axes() << r2->axes();
  foreach (QCPAxis *axis, allAxes)
  {
    axis->setLayer("axes");
    axis->grid()->setLayer("grid");
  }
  customPlot->savePng(dir.filePath("QCPColorGradient-levelcount.png"), 450, 180);
}

void MainWindow::genQCPColorGradient_Periodic()
{
  resetPlot(false);
  customPlot->plotLayout()->clear();
  
  QCPAxisRect *r1 = new QCPAxisRect(customPlot, true);
  QCPAxisRect *r2 = new QCPAxisRect(customPlot, true);
  r1->setupFullAxesBox(true);
  r2->setupFullAxesBox(true);
  r1->axis(QCPAxis::atLeft)->setTickLabels(false);
  r1->axis(QCPAxis::atBottom)->setTickLabels(false);
  r2->axis(QCPAxis::atLeft)->setTickLabels(false);
  r2->axis(QCPAxis::atBottom)->setTickLabels(false);
  QCPColorScale *scale1 = new QCPColorScale(customPlot);
  QCPColorScale *scale2 = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 0, r1);
  customPlot->plotLayout()->addElement(0, 1, scale1);
  customPlot->plotLayout()->addElement(0, 2, r2);
  customPlot->plotLayout()->addElement(0, 3, scale2);
  QCPColorGradient gradient(QCPColorGradient::gpHues);
  scale1->setGradient(gradient);
  gradient.setPeriodic(true);
  scale2->setGradient(gradient);
  
  QMargins m = r1->minimumMargins();
  m.setTop(m.top() + 10); // make some space for label
  m.setRight(0);
  r1->setMinimumMargins(m);
  r2->setMinimumMargins(m);

  QCPColorMap *map1 = new QCPColorMap(r1->axis(QCPAxis::atBottom), r1->axis(QCPAxis::atLeft));
  QCPColorMap *map2 = new QCPColorMap(r2->axis(QCPAxis::atBottom), r2->axis(QCPAxis::atLeft));
  customPlot->addPlottable(map1);
  customPlot->addPlottable(map2);
  int nx = 400;
  int ny = 400;
  map1->data()->setSize(nx, ny);
  map1->data()->setRange(QCPRange(0, 10), QCPRange(0, 10));
  for (int x=0; x<nx; ++x)
    for (int y=0; y<ny; ++y)
      map1->data()->setCell(x, y, (qExp(-qSqrt((x-310)*(x-310)+(y-260)*(y-260))/200.0)+
                                       qExp(-qSqrt((x-200)*(x-200)+(y-290)*(y-290))/80.0)-
                                       qExp(-qSqrt((x-180)*(x-180)+(y-140)*(y-140))/200.0)+0.436285)/1.53251*2-1);
  map2->setData(map1->data(), true);
  map1->setColorScale(scale1);
  map2->setColorScale(scale2);
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  r1->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  r2->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  scale1->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  scale2->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  scale1->axis()->setAutoTickCount(3);
  scale2->axis()->setAutoTickCount(3);
  map1->setDataRange(QCPRange(-0.2, 0.2));
  map2->setDataRange(QCPRange(-0.2, 0.2));
  customPlot->rescaleAxes();
  
  QCPItemText *t1 = new QCPItemText(customPlot);
  customPlot->addItem(t1);
  t1->setText("Periodic false");
  t1->position->setType(QCPItemPosition::ptAxisRectRatio);
  t1->position->setAxisRect(r1);
  t1->position->setCoords(0.5, -0.02);
  t1->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t1->setClipToAxisRect(false);
  QCPItemText *t2 = new QCPItemText(customPlot);
  customPlot->addItem(t2);
  t2->setText("Periodic true");
  t2->position->setType(QCPItemPosition::ptAxisRectRatio);
  t2->position->setAxisRect(r2);
  t2->position->setCoords(0.5, -0.02);
  t2->setPositionAlignment(Qt::AlignHCenter|Qt::AlignBottom);
  t2->setClipToAxisRect(false);
  
  QList<QCPAxis*> allAxes;
  allAxes << r1->axes() << r2->axes();
  foreach (QCPAxis *axis, allAxes)
  {
    axis->setLayer("axes");
    axis->grid()->setLayer("grid");
  }
  customPlot->savePng(dir.filePath("QCPColorGradient-periodic.png"), 450, 180);
}

void MainWindow::labelItemAnchors(QCPAbstractItem *item, double fontSize, bool circle, bool labelBelow)
{
  QList<QCPItemAnchor*> anchors = item->anchors();
  for (int i=0; i<anchors.size(); ++i)
  {
    if (circle)
    {
      QCPItemEllipse *circ = new QCPItemEllipse(item->parentPlot());
      item->parentPlot()->addItem(circ);
      circ->setClipToAxisRect(false);
      circ->topLeft->setParentAnchor(anchors.at(i));
      circ->bottomRight->setParentAnchor(anchors.at(i));
      circ->topLeft->setCoords(-4, -4);
      circ->bottomRight->setCoords(4, 4);
      QPen p(Qt::blue, 0, Qt::CustomDashLine);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0)) // Qt 4.8 changed the meaning of this completely...
      p.setDashPattern(QVector<qreal>()<<2<<1);
#else
      p.setDashPattern(QVector<qreal>()<<1<<3);
#endif
      circ->setPen(p);
      if (dynamic_cast<QCPItemPosition*>(anchors.at(i)))
      {
        QCPItemEllipse *circ2 = new QCPItemEllipse(item->parentPlot());
        item->parentPlot()->addItem(circ2);
        circ2->setClipToAxisRect(false);
        circ2->topLeft->setParentAnchor(anchors.at(i));
        circ2->bottomRight->setParentAnchor(anchors.at(i));
        circ2->topLeft->setCoords(-2.5, -2.5);
        circ2->bottomRight->setCoords(2.5, 2.5);
        circ2->setPen(Qt::NoPen);
        circ2->setBrush(Qt::blue);
      }
    }
    if (fontSize > 0)
    {
      QCPItemText *label = new QCPItemText(item->parentPlot());
      item->parentPlot()->addItem(label);
      label->setClipToAxisRect(false);
      label->setFont(QFont(font().family(), fontSize));
      label->setColor(Qt::blue);
      label->setText(QString("%2 (%1)").arg(i).arg(anchors.at(i)->name()));
      label->position->setParentAnchor(anchors.at(i));
      if (circle)
        label->position->setCoords(0, fontSize*2*(labelBelow?1:-1));
      else
        label->position->setCoords(0, 0);
      label->setTextAlignment(Qt::AlignCenter);
    }
  }
}

void MainWindow::addBracket(QPointF left, QPointF right, QString text, QPointF textOffset, bool textSideways, Qt::Alignment textAlign, QCPItemBracket::BracketStyle style)
{
  QCPItemBracket *bracket = new QCPItemBracket(customPlot);
  customPlot->addItem(bracket);
  bracket->setClipToAxisRect(false);
  bracket->left->setType(QCPItemPosition::ptAbsolute);
  bracket->right->setType(QCPItemPosition::ptAbsolute);
  bracket->left->setCoords(right);
  bracket->right->setCoords(left);
  bracket->setStyle(style);
  bracket->setLength(3);
  bracket->setPen(QPen(Qt::blue));
  
  QCPItemText *textItem = new QCPItemText(customPlot);
  customPlot->addItem(textItem);
  textItem->setClipToAxisRect(false);
  textItem->setText(text);
  textItem->setPositionAlignment(textAlign);
  if (textSideways)
    textItem->setRotation(-90);
  textItem->position->setParentAnchor(bracket->center);
  textItem->position->setCoords(textOffset);
  textItem->setColor(Qt::blue);
}

void MainWindow::addArrow(QPointF target, QPointF textPosition, QString text, Qt::Alignment textAlign)
{
  QCPItemText *textItem = new QCPItemText(customPlot);
  customPlot->addItem(textItem);
  textItem->setClipToAxisRect(false);
  textItem->setText(text);
  textItem->setPositionAlignment(textAlign);
  textItem->position->setType(QCPItemPosition::ptAbsolute);
  textItem->position->setCoords(textPosition);
  textItem->setColor(Qt::blue);
  QRectF textRect(textItem->topLeft->pixelPoint(), textItem->bottomRight->pixelPoint());
  
  QCPItemLine *arrowItem = new QCPItemLine(customPlot);
  customPlot->addItem(arrowItem);
  arrowItem->setClipToAxisRect(false);
  arrowItem->setHead(QCPLineEnding::esSpikeArrow);
  arrowItem->setPen(QPen(Qt::blue));
  arrowItem->end->setType(QCPItemPosition::ptAbsolute);
  arrowItem->end->setCoords(target);
  
  if (target.x() < textRect.left())
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->topLeft);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottomLeft);
    else
      arrowItem->start->setParentAnchor(textItem->left);
  } else if (target.x() > textRect.right())
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->topRight);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottomRight);
    else
      arrowItem->start->setParentAnchor(textItem->right);
  } else
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->top);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottom);
  }
}

void MainWindow::addGridLayoutOutline(QCPLayoutGrid *layout)
{
  QList<QCPLayoutElement*> elements;
  elements << layout;
  elements << layout->elements(true);
  for (int i=0; i<elements.size(); ++i)
  {
    if (!elements.at(i))
      continue;
    
    qDebug() << elements.at(i) << elements.at(i)->outerRect();
    
    QCPItemRect *outerRect = new QCPItemRect(customPlot);
    customPlot->addItem(outerRect);
    outerRect->setClipToAxisRect(false);
    outerRect->setBrush(QColor(0, 0, 0, 25));
    outerRect->setPen(QPen(QColor(180, 180, 180)));
    outerRect->topLeft->setType(QCPItemPosition::ptAbsolute);
    outerRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    outerRect->topLeft->setPixelPoint(elements.at(i)->outerRect().topLeft());
    outerRect->bottomRight->setPixelPoint(elements.at(i)->outerRect().bottomRight());
    
    QCPItemRect *innerRect = new QCPItemRect(customPlot);
    customPlot->addItem(innerRect);
    innerRect->setClipToAxisRect(false);
    innerRect->setBrush(QColor(230, 100, 100, 25));
    innerRect->setPen(QPen(QColor(180, 180, 180)));
    innerRect->topLeft->setType(QCPItemPosition::ptAbsolute);
    innerRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    innerRect->topLeft->setCoords(elements.at(i)->rect().topLeft());
    innerRect->bottomRight->setCoords(elements.at(i)->rect().bottomRight());
    
  }
}

void MainWindow::resetPlot(bool clearAxes)
{
  customPlot = new QCustomPlot(this);
  setCentralWidget(customPlot);
  if (clearAxes)
  {
    customPlot->xAxis->setRange(-0.4, 1.4);
    customPlot->yAxis->setRange(-0.2, 1.2);
    customPlot->xAxis->setVisible(false);
    customPlot->yAxis->setVisible(false);
    customPlot->axisRect()->setAutoMargins(QCP::msNone);
    customPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
  }
}
