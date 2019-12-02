#!/usr/bin/env python
# -*- coding: utf-8 -*-

# PyQt5 binding for QCustomPlot v2.0.0
#
# Authors: Dmitry Voronin, Giuseppe Corbelli
# License: MIT
#
# QCustomPlot author: Emanuel Eichhammer
# QCustomPlot Website/Contact: http://www.qcustomplot.com

import math

from PyQt5.QtCore import QTimer, QPointF, Qt
from PyQt5.QtGui import QPen, QBrush, QColor, QRadialGradient
from PyQt5.QtWidgets import QMainWindow
from PyQt5.uic import loadUi

import QCustomPlot

from QCustomPlot import QCP

class MainWindow(QMainWindow):
    def __init__(self, argv, parent=None):
        super().__init__(parent)
        loadUi("mainwindow.ui", self)

        self._available_demos = {
            0: self.setupQuadraticDemo,
            1: self.setupSimpleDemo,
#             2: self.setupSincScatterDemo,
#             3: self.setupScatterStyleDemo,
#             4: self.setupScatterPixmapDemo,
#             5: self.setupLineStyleDemo,
#             6: self.setupDateDemo,
#             7: self.setupTextureBrushDemo,
#             8: self.setupMultiAxisDemo,
#             9: self.setupLogarithmicDemo,
#             10: self.setupRealtimeDataDemo,
            11: self.setupParametricCurveDemo,
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

    def setupSimpleDemo(self):
        self.demoName = "Simple Demo"

        # add two new graphs and set their look:
        self.customPlot.addGraph()
        self.customPlot.graph(0).setPen(QPen(QColor("blue")))  # line color blue for first graph
        self.customPlot.graph(0).setBrush(QBrush(QColor(0, 0, 255, 20)))  # first graph will be filled with translucent blue
        self.customPlot.addGraph();
        self.customPlot.graph(1).setPen(QPen(QColor("red")))  # line color red for second graph
        # generate some points of data (y0 for first, y1 for second graph):
        x = [0.0] * 251
        y0 = [0.0] * 251
        y1 = [0.0] * 251
        for i in range(0, 251):
            x[i] = float(i)
            y0[i] = pow(math.e, -i/150.0) * math.cos(i/10.0)  # exponentially decaying cosine
            y1[i] = pow(math.e, -i/150.0)  # exponential envelope

        # configure right and top axis to show ticks but no labels:
        # (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
        self.customPlot.xAxis2.setVisible(True)
        self.customPlot.xAxis2.setTickLabels(False)
        self.customPlot.yAxis2.setVisible(True)
        self.customPlot.yAxis2.setTickLabels(False)
        # make left and bottom axes always transfer their ranges to right and top axes:
        self.customPlot.xAxis.rangeChanged.connect(self.customPlot.xAxis2.setRange)
        self.customPlot.yAxis.rangeChanged.connect(self.customPlot.yAxis2.setRange)
        # pass data points to graphs:
        self.customPlot.graph(0).setData(x, y0)
        self.customPlot.graph(1).setData(x, y1)
        # let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        self.customPlot.graph(0).rescaleAxes()
        # same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
        self.customPlot.graph(1).rescaleAxes(True)
        # Note: we could have also just called customPlot->rescaleAxes(); instead
        # Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        # TODO: figure out how to skip the explicit intermediate QCP.Interactions
        self.customPlot.setInteractions(QCustomPlot.QCP.Interactions(QCP.iRangeDrag | QCP.iRangeZoom | QCP.iSelectPlottables))

#   void setupSincScatterDemo(QCustomPlot *customPlot);
#   void setupScatterStyleDemo(QCustomPlot *customPlot);
#   void setupLineStyleDemo(QCustomPlot *customPlot);
#   void setupScatterPixmapDemo(QCustomPlot *customPlot);
#   void setupDateDemo(QCustomPlot *customPlot);
#   void setupTextureBrushDemo(QCustomPlot *customPlot);
#   void setupMultiAxisDemo(QCustomPlot *customPlot);
#   void setupLogarithmicDemo(QCustomPlot *customPlot);
#   void setupRealtimeDataDemo(QCustomPlot *customPlot);

    def setupParametricCurveDemo(self):
        self.demoName = "Parametric Curves Demo"

        # create empty curve objects. As they are not adopted by main QCustomPlot an explicit
        # reference must be kept
        self.fermatSpiral1 = QCustomPlot.QCPCurve(self.customPlot.xAxis, self.customPlot.yAxis)
        self.fermatSpiral2 = QCustomPlot.QCPCurve(self.customPlot.xAxis, self.customPlot.yAxis)
        self.deltoidRadial = QCustomPlot.QCPCurve(self.customPlot.xAxis, self.customPlot.yAxis)
        # generate the curve data points:
        pointCount = 501
        dataSpiral1 = [[0.0] * pointCount, [0.0] * pointCount, [0.0] * pointCount]
        dataSpiral2 = [[0.0] * pointCount, [0.0] * pointCount, [0.0] * pointCount]
        dataDeltoid = [[0.0] * pointCount, [0.0] * pointCount, [0.0] * pointCount]
        for i in range(0, pointCount):
            phi = i/(pointCount-1) * 8 * math.pi
            theta = i/(pointCount-1) * 2 * math.pi
            dataSpiral1[0][i] = float(i)
            dataSpiral1[1][i] = math.sqrt(phi) * math.cos(phi)
            dataSpiral1[2][i] = math.sqrt(phi) * math.sin(phi)
            dataSpiral2[0][i] = float(i)
            dataSpiral2[1][i] = -dataSpiral1[1][i]
            dataSpiral2[2][i] = -dataSpiral1[2][i]
            dataDeltoid[0][i] = float(i)
            dataDeltoid[1][i] = 2 * math.cos(2*theta) + math.cos(1*theta) + 2 * math.sin(theta)
            dataDeltoid[2][i] = 2 * math.sin(2*theta) - math.sin(1*theta)

        # pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
        self.fermatSpiral1.setData(dataSpiral1[0], dataSpiral1[1], dataSpiral1[2], True)
        self.fermatSpiral2.setData(dataSpiral2[0], dataSpiral2[1], dataSpiral2[2], True)
        self.deltoidRadial.setData(dataDeltoid[0], dataDeltoid[1], dataDeltoid[2], True)
        # color the curves:
        self.fermatSpiral1.setPen(QPen(Qt.blue))
        self.fermatSpiral1.setBrush(QBrush(QColor(0, 0, 255, 20)))
        self.fermatSpiral2.setPen(QPen(QColor(255, 120, 0)))
        self.fermatSpiral2.setBrush(QBrush(QColor(255, 120, 0, 30)))
        radialGrad = QRadialGradient(QPointF(310, 180), 200)
        radialGrad.setColorAt(0, QColor(170, 20, 240, 100))
        radialGrad.setColorAt(0.5, QColor(20, 10, 255, 40))
        radialGrad.setColorAt(1, QColor(120, 20, 240, 10))
        self.deltoidRadial.setPen(QPen(QColor(170, 20, 240)))
        self.deltoidRadial.setBrush(QBrush(radialGrad))
        # set some basic customPlot config:
        self.customPlot.setInteractions(QCustomPlot.QCP.Interactions(QCustomPlot.QCP.iRangeDrag | QCustomPlot.QCP.iRangeZoom | QCustomPlot.QCP.iSelectPlottables))
        self.customPlot.axisRect().setupFullAxesBox()
        self.customPlot.rescaleAxes()

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
