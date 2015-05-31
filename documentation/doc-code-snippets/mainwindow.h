#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "../../qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
public slots:
  // snippet methods:
  void snippetQCPColorGradient();
  void snippetQCPColorScale();
  void snippetQCPColorMap();
  void snippetQCPCurve();
  void snippetQCPItemLine();
  void snippetQCPMarginGroup();
  void snippetQCPPlotTitle();
  void snippetQCPLineEnding();
  void snippetQCPScatterStyle();
  void snippetQCPBarsGroup();
  void snippetQCPBars();
  void snippetQCPStatisticalBox();
  
  // website code methods:
  void websiteBasicPlottingBars();
  
private:
  void resetPlot();
  
  Ui::MainWindow *ui;
  QCustomPlot *customPlot;
};

#endif // MAINWINDOW_H
