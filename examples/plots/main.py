#!/usr/bin/env python
# -*- coding: utf-8 -*-

# PyQt5 binding for QCustomPlot v2.0.0
#
# Authors: Dmitry Voronin, Giuseppe Corbelli
# License: MIT
#
# QCustomPlot author: Emanuel Eichhammer
# QCustomPlot Website/Contact: http://www.qcustomplot.com

import sys

from PyQt5.QtWidgets import QApplication

import mainwindow

def main():
    app = QApplication(sys.argv)
    # First argument can be a number that specifies loaded example
    w = mainwindow.MainWindow(sys.argv)
    w.show()
    return app.exec_()


if __name__ == '__main__':
    sys.exit(main())
