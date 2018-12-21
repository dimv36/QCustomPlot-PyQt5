# QCustomPlot-PyQt5
Bindings for graphics lib QCustomPlot for PyQt5.

## Requirements:
  - PyQt5
  - sip
  - qmake (Qt5)
  - make/nmake/jom
  - Linux/Windows operating systems
  - gcc/msvc compilers

## Build & install:
Currently QCustomPlot is statically linked and downloaded as GIT submodule.

  - git submodule update --init
  - python setup.py build && sudo python setup.py install

## Arguments for build_ext option:
  * --qmake -- Path to qmake for building QCustomPlot static library (default: detected from PyQt5 installation)
  * --make -- Path to make utility (default: nmake.exe on Windows, make elsewhere)
  * --qt-include-dir -- Path to Qt's include location (default: detected from PyQt5 installation)

## TODO:
  - No debug builds are currently supported
  - Test on other than Linux & Windows systems
  - Test on compilers other than gcc/MSVC and specify minimum versions
