#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMetaEnum>
#include <QTimer>
#include <QDir>
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
  // image generators:
  void genScatterStyles();
  void genItemPixmap();
  void genItemRect();
  void genItemEllipse();
  void genItemLine();
  void genItemStraightLIne();
  void genItemCurve();
  void genItemBracket();
  void genItemText();
  void genItemTracer();
  void genLineEnding();
  void genMarginGroup();
  void genAxisRectSpacingOverview();
  void genAxisNamesOverview();
  
  void genLayoutsystem_AddingPlotTitle();
  void genLayoutsystem_MultipleAxisRects();
  
  void genQCPGraph();
  void genQCPCurve();
  void genQCPBars();
  void genQCPStatisticalBox();
  void genQCPColorMap();
  void genQCPFinancial();
  void genQCPColorScale();
  void genQCPColorGradient();
  void genQCPBarsGroup();
  
  void genQCPColorMap_Interpolate();
  void genQCPColorMap_TightBoundary();
  void genQCPColorGradient_LevelCount();
  void genQCPColorGradient_Periodic();
  
private:
  // helpers:
  void labelItemAnchors(QCPAbstractItem *item, double fontSize=8, bool circle=true, bool labelBelow=true);
  void addBracket(QPointF left, QPointF right, QString text, QPointF textOffset, bool textSideways, Qt::Alignment textAlign, QCPItemBracket::BracketStyle style=QCPItemBracket::bsRound);
  void addArrow(QPointF target, QPointF textPosition, QString text, Qt::Alignment textAlign=Qt::AlignCenter);
  void addGridLayoutOutline(QCPLayoutGrid *layout);
  void resetPlot(bool clearAxes=true);
  
  Ui::MainWindow *ui;
  QCustomPlot *customPlot;
  QDir dir; // target directory for images
  QBrush defaultBrush;
};

#endif // MAINWINDOW_H
