#!/usr/bin/env python

# This script is used to make the amalgamated sources qcustomplot.h/.cpp compatible
# to compiles with flag QT_NO_KEYWORDS set. It applies the following regular expression replacements
#
#   (^|[^\a_])emit\s        ->  \1Q_EMIT 
#   ^( *)signals:           ->  \1Q_SIGNALS:
#   (^|[^\a_])foreach( *)\( ->  \1Q_FOREACH\2(
#
# to the files whose filenames are passed on the command line. The replacement can in principle be
# applied to the sources with any other tool or editor that knows regular expressions.
#
# To test: Copy freshly amalgamated qcustomplot.h/.cpp files into this directory, call this script on
# them, and then run qmake; make

import os, sys, re

baseDir = sys.path[0];
os.chdir(baseDir) # change current working dir to script dir
  
def performKeywordReplacement(filename):
  print "making '"+filename+"' no-keywords-compatible..."
  patterns = []
  patterns.append((re.compile("(^|[^\\a_])emit\\s"), "\\1Q_EMIT "))
  patterns.append((re.compile("^( *)signals:"), "\\1Q_SIGNALS:"))
  patterns.append((re.compile("(^|[^\\a_])foreach( *)\\("), "\\1Q_FOREACH\\2("))
  inFile = open(filename)
  outFilename = filename + ".tmp"
  outFile = open(outFilename, "w")
  for line in inFile:
    for patt in patterns:
      line = re.sub(patt[0], patt[1], line)
    outFile.write(line)
  outFile.close()
  inFile.close()
  os.remove(filename)
  os.rename(outFilename, filename)
    
for filename in sys.argv[1:]:
  if not os.path.isfile(filename):
    print "file '"+filename+"' not found"
    sys.exit(-1)
  performKeywordReplacement(filename);

