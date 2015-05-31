#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QHBoxLayout>
#include <QDir>

#include "../../src/qcp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
  // tests:
  void setupItemAnchorTest(QCustomPlot *customPlot);
  void setupItemTracerTest(QCustomPlot *customPlot);
  void setupGraphTest(QCustomPlot *customPlot);
  void setupExportTest(QCustomPlot *customPlot);
  void setupExportMapTest(QCustomPlot *customPlot);
  void setupLogErrorsTest(QCustomPlot *customPlot);
  void setupSelectTest(QCustomPlot *customPlot);
  void setupDateTest(QCustomPlot *customPlot);
  void setupTickLabelTest(QCustomPlot *customPlot);
  void setupDaqPerformance(QCustomPlot *customPlot);
  void setupLayoutTest(QCustomPlot* customPlot);
  void setupMultiAxisTest(QCustomPlot* customPlot);
  void setupLayoutElementBugTest(QCustomPlot *customPlot);
  void setupMarginGroupTest(QCustomPlot* customPlot);
  void setupInsetLayoutTest(QCustomPlot* customPlot);
  void setupLegendTest(QCustomPlot *customPlot);
  void setupMultiAxisRectInteractions(QCustomPlot *customPlot);
  void setupAdaptiveSamplingTest(QCustomPlot *customPlot);
  void setupColorMapTest(QCustomPlot *customPlot);
  void setupBarsTest(QCustomPlot *customPlot);
  void setupBarsGroupTest(QCustomPlot *customPlot);
  // testbed:
  void setupTestbed(QCustomPlot *customPlot);
  
  // presets:
  void presetInteractive(QCustomPlot *customPlot);
  
  // helpers:
  void labelItemAnchors(QCPAbstractItem *item, double fontSize=8, bool circle=true, bool labelBelow=true);
  void showSelectTestColorMap(QCustomPlot *customPlot);
  
  // special use cases test:
  void setupIntegerTickStepCase(QCustomPlot *customPlot);
  
public slots:
  void tracerTestMouseMove(QMouseEvent *event);
  void selectTestColorMapRefresh();
  void mouseMoveRotateTickLabels(QMouseEvent *event);
  void tickLabelTestTimerSlot();
  void setupMultiAxisRectInteractionsMouseMove(QMouseEvent *event);
  void daqPerformanceDataSlot();
  void daqPerformanceReplotSlot();
  void colorMapMouseMove(QMouseEvent *event);
  
  void integerTickStepCase_xRangeChanged(QCPRange newRange);
  void integerTickStepCase_yRangeChanged(QCPRange newRange);
  void testbedMouseClick(QMouseEvent *event);
  void mouseWheel(QWheelEvent *event);
  
private:
  Ui::MainWindow *ui;
  QCustomPlot *mCustomPlot;
  QCPItemTracer *tracerTestTracer;
  QTimer mReplotTimer;
  QTimer mDataTimer;
};

#endif // MAINWINDOW_H
