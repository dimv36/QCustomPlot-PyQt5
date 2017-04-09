import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication
from PyQt5.QtGui import QColor, QPen
from qcustomplot import QCustomPlot, QCPBars, QCP, QCPBarsGroup


if __name__ == '__main__':

    app = QApplication(sys.argv)

    w = QCustomPlot()

    datax = [1, 2, 3, 4, 5]
    datay1 = [0.6, 0.5, 0.3, 0.15, 2.]
    datay2 = [0.3, 0.28, 0.2, 0.1, 3.]
    datay3 = [0.33, 0.31, 0.27, 0.13, 4.]

    group = QCPBarsGroup(w)
    bars1 = QCPBars(w.xAxis, w.yAxis)
    w.addPlottable(bars1)
    bars1.setData(datax, datay1)
    bars1.setBrush(QColor(0, 0, 255, 50))
    bars1.setPen(QColor(0, 0, 255))
    bars1.setWidth(0.15)
    bars1.setBarsGroup(group)

    bars2 = QCPBars(w.xAxis, w.yAxis)
    w.addPlottable(bars2)
    bars2.setData(datax, datay2)
    bars2.setBrush(QColor(180, 00, 120, 50))
    bars2.setPen(QColor(180, 00, 120))
    bars2.setWidth(0.15)
    bars2.setBarsGroup(group)

    bars3 = QCPBars(w.xAxis, w.yAxis)
    w.addPlottable(bars3)
    bars3.setData(datax, datay3)
    bars3.setBrush(QColor(255, 154, 0, 50))
    bars3.setPen(QColor(255, 154, 0))
    bars3.setWidth(0.15)
    bars3.setBarsGroup(group)

    w.xAxis.setRange(0.1, 4.9)
    w.yAxis.setRange(0, 0.7)
    w.xAxis.setAutoTickStep(False)
    w.xAxis.setTickStep(1)
    w.show()

    sys.exit(app.exec())
