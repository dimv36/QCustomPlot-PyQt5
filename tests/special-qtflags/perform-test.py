#!/usr/bin/env python
import os, sys, subprocess, shutil

baseDir = sys.path[0];
os.chdir(baseDir) # change current working dir to script dir

execTestSuffix = "& sleep 1; kill $!"; # appended to test execution command line. Starts test executable in background and quits it after one second (if executable fails, kill will fail and thus propagate !=0 return value)

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
    
def cleanup():
  subprocess.call("make clean", shell=True)
  if os.path.isfile("special-qtflags"):
    os.remove("special-qtflags")
  if os.path.isfile("Makefile"):
    os.remove("Makefile") 
    
# get fresh sources from main QCP directory and prepare them with make-no-keywords-compatible.py script:
shutil.copy2("../../qcustomplot.cpp", "./")
shutil.copy2("../../qcustomplot.h", "./")
subprocess.call("./make-no-keywords-compatible.py qcustomplot.h qcustomplot.cpp", shell=True)
    
# main test loop:
qmakeVersions = ["qmake464", "qmake474", "qmake486", "qmake501", "qmake511", "qmake520", "qmake532", "qmake540"]
for qmakecommand in qmakeVersions:
  try:
    qmakeproc = subprocess.Popen([qmakecommand, "-v"], stdout=subprocess.PIPE)
    printinfo(qmakeproc.stdout.readlines()[1].rstrip()); # print used Qt version in this run
  except:
    printinfo("Qt version of '"+qmakecommand+"' not found, skipping.");
    continue
  runQmakeMake(qmakecommand)
  if subprocess.call("./special-qtflags"+execTestSuffix, shell=True) != 0:
    printerror("Execution unsuccessful")
    sys.exit(-1)
  cleanup()
  
os.remove("qcustomplot.cpp")
os.remove("qcustomplot.h")
