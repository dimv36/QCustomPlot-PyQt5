# setup.py - distutils packaging
#
# Copyright (C) 2016 Dmitry Voronin  <carriingfate92@yandex.ru>
#
# qcustomplot - Python binding for graphic library QCustomPLot

"""qcustomplot - Python binding for graphic library QCustomPLot

QCustomPlot is a Qt widget for plotting and data visualization.
"""

# note: if you are changing the list of supported Python version please fix
# the docs in install.rst and the /features/ page on the website.
classifiers = """\
Platform: any
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

# Note: The setup.py must be compatible with both Python 2 and 3

import os
import sys
import re
import subprocess
from distutils.core import setup, Extension
from distutils.command.build_ext import build_ext
from distutils.sysconfig import get_python_inc
from distutils.ccompiler import get_default_compiler
from distutils.util import get_platform

try:
    from distutils.command.build_py import build_py_2to3
except ImportError:
    from distutils.command.build_py import build_py
else:
    class build_py(build_py_2to3):
        # workaround subclass for ticket #153
        pass

try:
    import configparser
except ImportError:
    import ConfigParser as configparser

# Take a look at http://www.python.org/dev/peps/pep-0386/
# for a consistent versioning pattern.

PSYCOPG_VERSION = '1.3.1'

version_flags = ['dt', 'dec']

PLATFORM_IS_WINDOWS = sys.platform.lower().startswith('win')


class SipConfig:
    def __init__(self, build_ext):
        self.build_ext = build_ext
        self.sip_exe = self.build_ext.sip_exe
        if not self.sip_exe:
            self.sip_exe = self.autodetect_sip_path()
        if self.sip_exe is None:
            sys.stderr.write("""\
Error: sip executable not found.

Please add the directory containing pg_config to the PATH
or specify the full executable path with the option:

    python setup.py build_ext --sip /path/to/pg_config build ...

or with the pg_config option in 'setup.cfg'.
""")
            sys.exit(1)

    def query(self, attr_name):
        """Spawn the pg_config executable, querying for the given config
        name, and return the printed value, sanitized. """
        try:
            pg_config_process = subprocess.Popen(
                [self.sip_exe, "--" + attr_name],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE)
        except OSError:
            raise Warning("Unable to find 'sip' file in '%s'" %
                          self.sip_exe)
        pg_config_process.stdin.close()
        result = pg_config_process.stdout.readline().strip()
        if not result:
            raise Warning(pg_config_process.stderr.readline())
        if not isinstance(result, str):
            result = result.decode('ascii')
        return result

    def find_on_path(self, exename, path_directories=None):
        if not path_directories:
            path_directories = os.environ['PATH'].split(os.pathsep)
        for dir_name in path_directories:
            fullpath = os.path.join(dir_name, exename)
            if os.path.isfile(fullpath):
                return fullpath
        return None

    def autodetect_sip_path(self):
        """Find and return the path to the pg_config executable."""
        if PLATFORM_IS_WINDOWS:
            return self.autodetect_sip_path_windows()
        else:
            return self.find_on_path('sip')

    def autodetect_sip_path_windows(self):
        """Attempt several different ways of finding the pg_config
        executable on Windows, and return its full path, if found."""

        # This code only runs if they have not specified a sip option
        # in the config file or via the commandline.

        # First, check for sip.exe on the PATH, and use that if found.
        sip_exe = self.find_on_path('sip.exe')
        if sip_exe:
            return sip_exe

        return None


class qcustomplot_build_ext(build_ext):
    """Conditionally complement the setup.cfg options file.

    This class configures the include_dirs, library_dirs, libraries
    options as required by the system. Most of the configuration happens
    in finalize_options() method.

    If you want to set up the build step for a peculiar platform, add a
    method finalize_PLAT(), where PLAT matches your sys.platform.
    """
    user_options = build_ext.user_options[:]
    boolean_options = build_ext.boolean_options[:]

    def __init__(self, *args, **kwargs):
        build_ext.__init__(self, *args, **kwargs)

    def initialize_options(self):
        build_ext.initialize_options(self)
        self.sip_exe = None

    def compiler_is_msvc(self):
        return self.get_compiler_name().lower().startswith('msvc')

    def compiler_is_mingw(self):
        return self.get_compiler_name().lower().startswith('mingw')

    def get_compiler_name(self):
        """Return the name of the C compiler used to compile extensions.

        If a compiler was not explicitly set (on the command line, for
        example), fall back on the default compiler.
        """
        if self.compiler:
            # distutils doesn't keep the type of self.compiler uniform; we
            # compensate:
            if isinstance(self.compiler, str):
                name = self.compiler
            else:
                name = self.compiler.compiler_type
        else:
            name = get_default_compiler()
        return name

    def get_export_symbols(self, extension):
        # Fix MSVC seeing two of the same export symbols.
        if self.compiler_is_msvc():
            return []
        else:
            return build_ext.get_export_symbols(self, extension)

    def build_extension(self, extension):
        build_ext.build_extension(self, extension)
        sysVer = sys.version_info[:2]

    def finalize_win32(self):
        """Finalize build system configuration on win32 platform."""
        sysVer = sys.version_info[:2]

        # Add compiler-specific arguments:
        extra_compiler_args = []

        if self.compiler_is_mingw():
            # Default MinGW compilation of Python extensions on Windows uses
            # only -O:
            extra_compiler_args.append('-O3')

            # GCC-compiled Python on non-Windows platforms is built with strict
            # aliasing disabled, but that must be done explicitly on Windows to
            # avoid large numbers of warnings for perfectly idiomatic Python C
            # API code.
            extra_compiler_args.append('-fno-strict-aliasing')

            # Force correct C runtime library linkage:
            if sysVer <= (2, 3):
                # Yes:  'msvcr60', rather than 'msvcrt', is the correct value
                # on the line below:
                self.libraries.append('msvcr60')
            elif sysVer in ((2, 4), (2, 5)):
                self.libraries.append('msvcr71')
            # Beyond Python 2.5, we take our chances on the default C runtime
            # library, because we don't know what compiler those future
            # versions of Python will use.

        for extension in ext:  # ext is a global list of Extension objects
            extension.extra_compile_args.extend(extra_compiler_args)
        # End of add-compiler-specific arguments section.

        self.libraries.append("ws2_32")
        self.libraries.append("advapi32")
        if self.compiler_is_msvc():
            # MSVC requires an explicit "libpq"
            self.libraries.remove("pq")
            self.libraries.append("secur32")
            self.libraries.append("libpq")
            self.libraries.append("shfolder")
            for path in self.library_dirs:
                if os.path.isfile(os.path.join(path, "ms", "libpq.lib")):
                    self.library_dirs.append(os.path.join(path, "ms"))
                    break
            if self.have_ssl:
                self.libraries.append("libeay32")
                self.libraries.append("ssleay32")
                self.libraries.append("crypt32")
                self.libraries.append("user32")
                self.libraries.append("gdi32")

    def finalize_linux(self):
        """Finalize build system configuration on GNU/Linux platform."""
        # tell piro that GCC is fine and dandy, but not so MS compilers
        for extension in self.extensions:
            extension.extra_compile_args.append(
                '-Wdeclaration-after-statement')

    finalize_linux2 = finalize_linux
    finalize_linux3 = finalize_linux

    def finalize_options(self):
        """Complete the build system configuration."""
        build_ext.finalize_options(self)

        sip_config = SipConfig(self)

        if hasattr(self, "finalize_" + sys.platform):
            getattr(self, "finalize_" + sys.platform)()

def is_py_64():
    # sys.maxint not available since Py 3.1;
    # sys.maxsize not available before Py 2.6;
    # this is portable at least between Py 2.4 and 3.4.
    import struct
    return struct.calcsize("P") > 4

QCUSTOMPLOT_VERSION = '1.3.1'

# let's start with macro definitions (the ones not already in setup.cfg)
define_macros = []
include_dirs = []

# gather information to build the extension module
ext = []
data_files = []

# sources

sources = [
    'qcustomplot.cpp'
]

depends = [
    # headers
    'qcustomplot.h'
]

parser = configparser.ConfigParser()
parser.read('setup.cfg')

# generate a nice version string to avoid confusion when users report bugs
version_flags.append('pq3') # no more a choice
version_flags.append('ext') # no more a choice

if version_flags:
    QCUSTOMPLOT_VERSION_EX = QCUSTOMPLOT_VERSION + " (%s)" % ' '.join(version_flags)
else:
    QCUSTOMPLOT_VERSION_EX = QCUSTOMPLOT_VERSION

if not PLATFORM_IS_WINDOWS:
    define_macros.append(('QCUSTOMPLOT_VERSION', '"' + QCUSTOMPLOT_VERSION_EX + '"'))
else:
    define_macros.append(('PSYCOPG_VERSION', '\\"' + QCUSTOMPLOT_VERSION_EX + '\\"'))

# build the extension

sources = [os.path.join('qcustomplot', x) for x in sources]
depends = [os.path.join('qcustomplot', x) for x in depends]

ext.append(Extension("qcustomplot._qcustomplot", sources,
                     define_macros=define_macros,
                     include_dirs=include_dirs,
                     depends=depends,
                     undef_macros=[]))

try:
    f = open("README.rst")
    readme = f.read()
    f.close()
except:
    print("failed to read readme: ignoring...")
    readme = __doc__

setup(name="qcustomplot",
      version=QCUSTOMPLOT_VERSION,
      maintainer="Dmitry Voronin",
      maintainer_email="carriingfate92@yandex.ru",
      author="Dmitry Voronin",
      author_email="carriingfate92@yandex.ru",
      url="https://github.com/dimV36/QCustomPlot-PyQt5",
      license="LGPL",
      platforms=["any"],
      description=readme.split("\n")[0],
      long_description="\n".join(readme.split("\n")[2:]).lstrip(),
      classifiers=[x for x in classifiers.split("\n") if x],
      data_files=data_files,
      package_dir={'qcustomplot': 'lib'},
      packages=['qcustomplot'],
      cmdclass={
          'build_ext': qcustomplot_build_ext,
          'build_py': build_py, },
      ext_modules=ext)
