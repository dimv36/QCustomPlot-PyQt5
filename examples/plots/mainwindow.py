#!/usr/bin/env python
# -*- coding: utf-8 -*-

# PyQt5 binding for QCustomPlot v2.0.0
#
# Authors: Dmitry Voronin, Giuseppe Corbelli
# License: MIT
#
# QCustomPlot author: Emanuel Eichhammer
# QCustomPlot Website/Contact: http://www.qcustomplot.com

from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QMainWindow
from PyQt5.uic import loadUi

import QCustomPlot

class MainWindow(QMainWindow):
    def __init__(self, argv, parent=None):
        super().__init__(parent)
        loadUi("mainwindow.ui", self)

        self._available_demos = {
            0: self.setupQuadraticDemo,
#             1: self.setupSimpleDemo,
#             2: self.setupSincScatterDemo,
#             3: self.setupScatterStyleDemo,
#             4: self.setupScatterPixmapDemo,
#             5: self.setupLineStyleDemo,
#             6: self.setupDateDemo,
#             7: self.setupTextureBrushDemo,
#             8: self.setupMultiAxisDemo,
#             9: self.setupLogarithmicDemo,
#             10: self.setupRealtimeDataDemo,
#             11: self.setupParametricCurveDemo,
#             12: self.setupBarChartDemo,
#             13: self.setupStatisticalDemo,
#             14: self.setupSimpleItemDemo,
#             15: self.setupItemDemo,
#             16: self.setupStyledDemo,
#             17: self.setupAdvancedAxesDemo,
#             18: self.setupColorMapDemo,
#             19: self.setupFinancialDemo,
        }

        self.currentDemoIndex = -1
        self.demoName = ""
        self.setGeometry(400, 250, 542, 390)
        try:
            demoIndex = int(argv[-1])
            self._available_demos[demoIndex]
        except Exception:
            demoIndex = 0

        self.setupDemo(demoIndex)

    def setupDemo(self, demoIndex):
        self._available_demos[demoIndex]()
        self.setWindowTitle("QCustomPlot demo: {}".format(self.demoName))
        self.statusBar.clearMessage()
        self.currentDemoIndex = demoIndex
        self.customPlot.replot()

    def setupQuadraticDemo(self):
        self.demoName = "Quadratic Demo"
        # generate some data: initialize with entries 0..100
        x = [0] * 100
        y = [0] * 100
        for i in range(0, 100):
            x[i] = i/50.0 - 1  # x goes from -1 to 1
            y[i] = x[i]*x[i]  # let's plot a quadratic function

        # create graph and assign data to it:
        self.customPlot.addGraph()
        self.customPlot.graph(0).setData(x, y)
        # give the axes some labels:
        self.customPlot.xAxis.setLabel("x")
        self.customPlot.yAxis.setLabel("y")
        # set axes ranges, so we see all data:
        self.customPlot.xAxis.setRange(-1, 1)
        self.customPlot.yAxis.setRange(0, 1)

#   void setupSimpleDemo(QCustomPlot *customPlot);
#   void setupSincScatterDemo(QCustomPlot *customPlot);
#   void setupScatterStyleDemo(QCustomPlot *customPlot);
#   void setupLineStyleDemo(QCustomPlot *customPlot);
#   void setupScatterPixmapDemo(QCustomPlot *customPlot);
#   void setupDateDemo(QCustomPlot *customPlot);
#   void setupTextureBrushDemo(QCustomPlot *customPlot);
#   void setupMultiAxisDemo(QCustomPlot *customPlot);
#   void setupLogarithmicDemo(QCustomPlot *customPlot);
#   void setupRealtimeDataDemo(QCustomPlot *customPlot);
#   void setupParametricCurveDemo(QCustomPlot *customPlot);
#   void setupBarChartDemo(QCustomPlot *customPlot);
#   void setupStatisticalDemo(QCustomPlot *customPlot);
#   void setupSimpleItemDemo(QCustomPlot *customPlot);
#   void setupItemDemo(QCustomPlot *customPlot);
#   void setupStyledDemo(QCustomPlot *customPlot);
#   void setupAdvancedAxesDemo(QCustomPlot *customPlot);
#   void setupColorMapDemo(QCustomPlot *customPlot);
#   void setupFinancialDemo(QCustomPlot *customPlot);
#
#   void setupPlayground(QCustomPlot *customPlot);

# private slots:
#   void realtimeDataSlot();
#   void bracketDataSlot();
#   void screenShot();
#   void allScreenShots();
#
# private:
#   Ui::MainWindow *ui;
#   QString demoName;
#   QTimer dataTimer;
#   QCPItemTracer *itemDemoPhaseTracer;
#   int currentDemoIndex;
# };
