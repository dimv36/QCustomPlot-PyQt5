import os
import sys
try:
    from sipconfig import Configuration, ModuleMakefile
except ImportError:
    print('Could not import sipconfig module')
    exit(1)
try:
    from PyQt5.QtCore import PYQT_CONFIGURATION
except ImportError:
    print('Could not import PyQt5 module')
    exit(1)

# Get the PyQt configuration information.
config = Configuration()
# Should be detected automatically, but if not, override here
#config.platform = "win32-msvc2008"

# Get the extra SIP flags needed by the imported PyQt modules.  Note that
# this normally only includes those flags (-x and -t) that relate to SIP's
# versioning system.
pyqt_sip_flags = PYQT_CONFIGURATION['sip_flags']
pyqt_sip_dir = '%s/PyQt5/' % config.default_sip_dir

# Run SIP to generate the code.  Note that we tell SIP where to find the qt
# module's specification files using the -I flag.
command = " ".join([config.sip_bin, "-c", ".", "-o", "-I", pyqt_sip_dir,  pyqt_sip_flags, "qcustomplot.sip"])
os.system(command)
print(command)

# Now we run qmake and build library
os.system('qmake')
if sys.platform == 'linux':
    os.system('make')
elif sys.platform == 'windows':
    os.system('nmake')