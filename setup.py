#!/usr/bin/env python
"""QCustomPlot: bindings for QCustomPlot library.

QCustomPlot is a Qt C++ widget for plotting and data visualization. It has no further dependencies and is fully
documented. This plotting library focuses on making good looking, publication quality 2D plots, graphs and charts,
as well as offering high performance for realtime visualization applications.
"""
from __future__ import division, print_function

DOCLINES = __doc__.split("\n")

import os
import sys


if sys.version_info[:2] < (2, 6) or (3, 0) <= sys.version_info[0:2] < (3, 2):
    raise RuntimeError("Python version 2.6, 2.7 or >= 3.2 required.")

if sys.version_info[0] >= 3:
    import builtins
else:
    import __builtin__ as builtins

CLASSIFIERS = """\
Development Status :: 5 - Production/Stable
Environment :: MacOS X
Environment :: Win32 (MS Windows)
Environment :: X11 Applications :: Qt
Intended Audience :: Developers
License :: OSI Approved :: GNU General Public License (GPL)
License :: Other/Proprietary License
Operating System :: MacOS :: MacOS X
Operating System :: Microsoft :: Windows
Operating System :: POSIX
Operating System :: Unix
Programming Language :: C++
Programming Language :: Python
Programming Language :: Python :: 2
Programming Language :: Python :: 3
Topic :: Software Development :: User Interfaces
Topic :: Software Development :: Widget Sets
"""

MAJOR = 1
MINOR = 3
MICRO = 1
VERSION = '%d.%d.%d' % (MAJOR, MINOR, MICRO)
BUILD_FILE = 'qcustomplot.sbf'

# BEFORE importing distutils, remove MANIFEST. distutils doesn't properly
# update it when the contents of directories change.
if os.path.exists('MANIFEST'):
    os.remove('MANIFEST')

# This is a bit hackish: we are setting a global variable so that the main
# numpy __init__ can detect if it is being loaded by the setup routine, to
# avoid attempting to load components that aren't built yet.  While ugly, it's
# a lot more robust than what was previously being used.
builtins.__NUMPY_SETUP__ = True

def write_version_py(filename='qcustomplot/version.py'):
    cnt = """
# THIS FILE IS GENERATED FROM QCUSTOMPLOT SETUP.PY
version = '%(version)s'
"""
    a = open(filename, 'w')
    try:
        a.write(cnt % {'version': VERSION})
    finally:
        a.close()

def generate_sip():
    # Определяем наличие sip в системе
    try:
        import sipconfig
    except ImportError:
        print('SIP module is required')
        exit(1)
    else:
        pkg_config = sipconfig._pkg_config
        print('Found SIP %s' % pkg_config['sip_version_str'])
    # Определяем версию библиотеки PyQt
    try:
        import PyQt5.QtCore
    except ImportError:
        print('PyQt5 module is required')
        exit(1)
    else:
        print('Found PyQt %s' % PyQt5.QtCore.PYQT_VERSION_STR)

    cwd = os.path.abspath(os.path.dirname(__file__))
    cwd += '/qcustomplot'
    print('Generate sources for QCustomPlot...')
    import sipconfig
    from PyQt5.QtCore import PYQT_CONFIGURATION
    sip_config = sipconfig._pkg_config
    cmd = ''.join([sip_config['default_bin_dir'] + '/sip',
                   ' -c %s' % cwd,
                   ' -b %s/qcustomplot.sbf' % cwd,
                   ' -o'
                   ' -I %s/PyQt5' % sip_config['default_sip_dir'],
                   ' %s' % PYQT_CONFIGURATION['sip_flags'],
                   ' %s/qcustomplot.sip' % cwd])
    print(cmd)
    status = os.system(cmd)
    if status != 0:
        raise RuntimeError("Running SIP failed!")

def build():
    from sipconfig import ModuleMakefile, Configuration
    old_pwd = os.path.abspath(os.path.dirname(__file__))
    new_pwd = old_pwd + '/qcustomplot'
    os.chdir(new_pwd)
    config = Configuration()
    makefile = ModuleMakefile(configuration=config,
                              build_file=BUILD_FILE)
    makefile.extra_include_dirs = ['/usr/include',
                                   '/usr/include/qt5',
                                   '/usr/include/qt5/QtCore',
                                   '/usr/include/qt5/QtWidgets',
                                   '/usr/include/qt5/QtGui',
                                   '/usr/include/qt5/QtPrintSupport']
    makefile.extra_libs = ['qcustomplot']
    makefile.extra_lib_dirs = ["/usr/lib64"]
    makefile.generate()
    status = os.system('make -j2')
    if status:
        print('Error!')
    os.chdir(old_pwd)

def setup_package():
    src_path = os.path.dirname(os.path.abspath(sys.argv[0]))
    old_path = os.getcwd()
    os.chdir(src_path)
    sys.path.insert(0, src_path)

    # Rewrite the version file everytime
    write_version_py()

    metadata = dict(
        name='QCustomPlot',
        maintainer='Dmitry Voronin',
        maintainer_email='carriingfate92@yandex.ru',
        description = DOCLINES[0],
        long_description='\n'.join(DOCLINES[2:]),
        url='https://github.com/dimV36/QCustomPlot-PyQt5',
        author='Dmitry Voronin',
        download_url='https://github.com/dimV36/QCustomPlot-PyQt5',
        license='GPL',
        classifiers=[_f for _f in CLASSIFIERS.split('\n') if _f],
        platforms=['Windows', 'Linux', 'Mac OS-X']
    )

    # Run build
    if len(sys.argv) >= 2 and ('--help' in sys.argv[1:] or
            sys.argv[1] in ('--help-commands', 'egg_info', '--version',
                            'clean')):
        # Use setuptools for these commands (they don't work well or at all
        # with distutils).  For normal builds use distutils.
        try:
            from setuptools import setup
        except ImportError:
            from distutils.core import setup
        metadata['version'] = VERSION
    else:
        if len(sys.argv) >= 2 and sys.argv[1] == 'bdist_wheel':
            # bdist_wheel needs setuptools
            import setuptools
        cwd = os.path.abspath(os.path.dirname(__file__))
        if not os.path.exists(os.path.join(cwd, 'PKG-INFO')):
            # Generate sip sources, unless building from source release
            generate_sip()
    try:
        from distutils.core import setup
        setup(**metadata)
    finally:
        del sys.path[0]
        os.chdir(old_path)
    return


if __name__ == '__main__':
    setup_package()