import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication
from PyQt5.QtGui import QColor, QPen
from qcustomplot import QCustomPlot, QCPBars, QCP


if __name__ == '__main__':

    app = QApplication(sys.argv)

    w = QCustomPlot()
    regen = QCPBars(w.xAxis, w.yAxis)
    nuclear = QCPBars(w.xAxis, w.yAxis)
    fossil = QCPBars(w.xAxis, w.yAxis)

    w.addPlottable(regen)
    w.addPlottable(nuclear)
    w.addPlottable(fossil)

    pen = QPen()
    pen.setWidthF(1.2)
    fossil.setName('Fossil fuels')
    pen.setColor(QColor(255, 131, 0))
    fossil.setPen(pen)
    fossil.setBrush(QColor(255, 131, 0, 50))
    nuclear.setName('Nuclear')
    pen.setColor(QColor(1, 92, 192))
    nuclear.setPen(pen)
    nuclear.setBrush(QColor(1, 92, 191, 50))
    regen.setName('Regenerative')
    pen.setColor(QColor(150, 222, 0))
    regen.setPen(pen)
    regen.setBrush(QColor(150, 222, 0, 70))
    nuclear.moveAbove(fossil)
    regen.moveAbove(nuclear)

    ticks = [1, 2, 3, 4, 5, 6, 7]
    labels = ['USA', 'Japan', 'Germany', 'France', 'UK', 'Italy', 'Canada']
    w.xAxis.setAutoTicks(False)
    w.xAxis.setAutoTickLabels(False)
    w.xAxis.setTickVector(ticks)
    w.xAxis.setTickVectorLabels(labels)
    w.xAxis.setTickLabelRotation(60)
    w.xAxis.setSubTickCount(0)
    w.xAxis.grid().setVisible(True)
    w.xAxis.setRange(0, 8)

    w.yAxis.setRange(0, 12.1)
    w.yAxis.setPadding(5)
    w.yAxis.setLabel('Power Consumption in\nKilowatts per Capita (2007)')
    w.yAxis.grid().setSubGridVisible(True)

    grid_pen = QPen()
    grid_pen.setStyle(Qt.SolidLine)
    grid_pen.setColor(QColor(0, 0, 0, 25))
    w.yAxis.grid().setSubGridPen(grid_pen)

    fossil_data = [0.86 * 10.5, 0.83 * 5.5, 0.84 * 5.5, 0.52 * 5.8, 0.89 * 5.2, 0.90 * 4.2, 0.67 * 11.2]
    nuclear_data = [0.08 * 10.5, 0.12 * 5.5, 0.12 * 5.5, 0.40 * 5.8, 0.09 * 5.2, 0.00 * 4.2, 0.07 * 11.2]
    regen_data = [0.06 * 10.5, 0.05 * 5.5, 0.04 * 5.5, 0.06 * 5.8, 0.02 * 5.2, 0.07 * 4.2, 0.25 * 11.2]
    fossil.setData(ticks, fossil_data)
    nuclear.setData(ticks, nuclear_data)
    regen.setData(ticks, regen_data)

    w.legend.setVisible(True)
    w.axisRect().insetLayout().setInsetAlignment(0, Qt.AlignTop|Qt.AlignHCenter)
    w.legend.setBrush(QColor(255, 255, 255, 200))
    legendPen = QPen()
    legendPen.setColor(QColor(130, 130, 130, 200))
    w.legend.setBorderPen(legendPen)
    w.setInteractions(QCP.iRangeDrag or QCP.iRangeZoom)

    w.show()

    sys.exit(app.exec())
