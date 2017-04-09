# QCustomPlot-PyQt5
Bindings for graphics lib QCustomPlot for PyQt5

## Requirements:
 - PyQt5
 - sip
 - qmake (for Qt5)
 - make
 - Linux operationg system

## Build & install:
  python setup.py build && sudo python setup.py install

## Arguments for build_ext option:
  * --qmake -- Path to qmake for building QCustomPlot static library (defaults: /usr/bin/qmake)
  * --make -- Path to make utility (defaults: /usr/bin/make)
  * --qt-include-dir -- Path to Qt's include location (defaults: /usr/include/qt)

## TODO:
  * Autodetecting Qt5 installation (in current version we suppose Qt5 installation headers to /usr/include/qt)
  * Windows building support
