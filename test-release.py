#!/usr/bin/env python
import os, sys, subprocess, shutil, distutils.dir_util, argparse

# Define command line interface:
argparser = argparse.ArgumentParser()
argparser.add_argument("-qt", type=int, default=0,
                    help="Specify a specific qt version (e.g. 474 or 502) to test.")
argparser.add_argument("-p", "--pause", action="store_true",
                    help="Pause after every qt version.")
argparser.add_argument("-s", "--short", action="store_true",
                    help="Only test the plots example.")
argparser.add_argument("-i", "--interactive", action="store_true",
                    help="Keep tests open for user interaction and continue test only after user quits them.")
argparser.add_argument("-r", "--reuseqcpobject", action="store_true",
                    help="Compiles the qcustomplot.o file only once per Qt version tested, and reuses it for all test projects")
config = argparser.parse_args()

execTestSuffix = "& sleep 1; kill $!"; # appended to test execution command line. Starts test executable in background and quits it after one second (if executable fails, kill will fail and thus propagate !=0 return value)
if config.interactive:
  execTestSuffix = "";
qcpObjectDir = ""; # if -r option is set, points to the path of the compiled qcustomplot.o, so subsequent examples can pull them in

# define functions:
def printinfo(message):
  print "\033[1;36m"+message+"\033[1;m"

def printerror(message):
  print "\033[1;31m"+message+"\033[1;m"
  
def runQmakeMake(qmakecommand):
  if subprocess.call(qmakecommand, shell=True) != 0:
    printerror("qmake failed"); sys.exit(1)
  if subprocess.call("make -j5", shell=True) != 0:
    printerror("make failed"); sys.exit(1)

def runExample(examplePath, executableName):
  global qcpObjectDir
  workingDirectory = os.getcwd()
  printinfo(examplePath)
  os.chdir(examplePath)
  if config.reuseqcpobject:
    if len(qcpObjectDir) != 0:
      shutil.copy2(qcpObjectDir+"/qcustomplot.o", "./")
      shutil.copy2(qcpObjectDir+"/moc_qcustomplot.o", "./")
    else:
      qcpObjectDir = os.getcwd();
  runQmakeMake(qmakecommand)
  if subprocess.call("./"+executableName+execTestSuffix, shell=True) != 0:
    printerror("Execution unsuccessful")
  os.chdir(workingDirectory)

# main test loop:
qmakeVersions = ["qmake464", "qmake474", "qmake486", "qmake501", "qmake511", "qmake520", "qmake532", "qmake540"]
if (config.qt > 0):
  qmakeVersions = ["qmake"+str(config.qt)];
for qmakecommand in qmakeVersions:
  qcpObjectDir = "" # make sure to compile qcustomplot objects new for every qt version, if -r flag set
  try:
    qmakeproc = subprocess.Popen([qmakecommand, "-v"], stdout=subprocess.PIPE)
    printinfo(qmakeproc.stdout.readlines()[1].rstrip()); # print used Qt version in this run
  except:
    printinfo("Qt version of '"+qmakecommand+"' not found, skipping.");
    continue
  baseDir = sys.path[0];
  tempDir = baseDir+"/temp"
  os.mkdir(tempDir)

  # ================== QCustomPlot full ==================
  printinfo("QCustomPlot full:")
  os.chdir(tempDir)
  shutil.copy2(baseDir+"/QCustomPlot.tar.gz", "./")
  if subprocess.call("tar -xf QCustomPlot.tar.gz", shell=True) != 0:
    printerror("Failed to untar QCustomPlot.tar.gz"); sys.exit(1)
  os.chdir(tempDir+"/qcustomplot")
  # test examples:
  runExample("examples/plots", "plot-examples")
  if not config.short:
    runExample("examples/interactions", "interaction-example")
    runExample("examples/text-document-integration", "text-document-integration")
    runExample("examples/scrollbar-axis-range-control", "scrollbar-axis-range-control")
  os.chdir(tempDir)

  # ================== QCustomPlot-sharedlib and -source package ==================
  if not config.short:
    printinfo("QCustomPlot sharedlib and source:")
    os.chdir(tempDir)
    shutil.copy2(baseDir+"/QCustomPlot-sharedlib.tar.gz", "./")
    if subprocess.call("tar -xf QCustomPlot-sharedlib.tar.gz", shell=True) != 0:
      printerror("Failed to untar QCustomPlot-sharedlib.tar.gz"); sys.exit(1)
    shutil.copy2(baseDir+"/QCustomPlot-source.tar.gz", "./")
    if subprocess.call("tar -xf QCustomPlot-source.tar.gz", shell=True) != 0:
      printerror("Failed to untar QCustomPlot-source.tar.gz"); sys.exit(1)
    shutil.copy2(tempDir+"/qcustomplot-source/qcustomplot.h", tempDir+"/")   # copy qcp source to dir above sharedlib dir (sharedlib code expects it two dirs above source files)
    shutil.copy2(tempDir+"/qcustomplot-source/qcustomplot.cpp", tempDir+"/") # copy qcp source to dir above sharedlib dir (sharedlib code expects it two dirs above source files)
    # sharedlib compile:
    printinfo("sharedlib-compilation")
    os.chdir(tempDir+"/qcustomplot-sharedlib/sharedlib-compilation")
    runQmakeMake(qmakecommand)
    subprocess.call("cp libqcustomplot* ../sharedlib-usage", shell=True)
    # sharedlib use:
    printinfo("sharedlib-usage")
    os.chdir(tempDir+"/qcustomplot-sharedlib/sharedlib-usage")
    runQmakeMake(qmakecommand)
    if subprocess.call("export LD_LIBRARY_PATH=.; ./sharedlib-usage"+execTestSuffix, shell=True) != 0:
      printerror("Execution unsuccessful")
    os.chdir(tempDir)

  # ================== Cleanup ==================
  answer = ""
  if config.pause:
    if qmakecommand == qmakeVersions[len(qmakeVersions)-1]:
      printinfo("Press any key to finish release test.")
    else:
      printinfo("Enter 'q' and hit return to stop release test, enter nothing to continue with next qt version...")
    answer = raw_input()
  os.chdir(baseDir)
  shutil.rmtree("./temp")
  printinfo("cleaned up")
  if (answer == "q"):
    break
  

