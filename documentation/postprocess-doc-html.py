#!/usr/bin/env python

#
# This script performs various customizations on the documentation html files that can't be
# achieved with the regular doxygen options.
#

import os, sys, re

baseDir = sys.path[0];
os.chdir(baseDir) # change current working dir to script dir

def performReplacement(filename):
  print "html postprocessing '"+filename+"'..."
  patterns = []
  patterns.append((re.compile("<div class=\"title\">Related Pages</div>"), "<div class=\"title\">Special Pages</div>"))
  patterns.append((re.compile("<div class=\"textblock\">Here is a list of all related documentation pages:</div>"), ""))
  patterns.append((re.compile("<div class=\"textblock\">Here are the data structures with brief descriptions:</div>"), ""))
  patterns.append((re.compile("This inheritance list is sorted roughly, but not completely, alphabetically:"), ""))
  patterns.append((re.compile("<div class=\"levels\">\\[detail level (<span onclick=\"javascript:toggleLevel\\(\d\\);\">\d</span>)+\\]</div>"), ""))
  patterns.append((re.compile("Go to the graphical class hierarchy"), "Switch to graphical view"))
  patterns.append((re.compile("Go to the textual class hierarchy"), "Switch to list view"))
  # link images with inline image maps for class overview page:
  patterns.append((re.compile("<img src=\"RelationOverview.png\" alt=\"RelationOverview.png\"/>"), "<img src=\"RelationOverview.png\" alt=\"RelationOverview.png\" usemap=\"#relationoverviewmap\"/>"))
  patterns.append((re.compile("<img src=\"InheritanceOverview.png\" alt=\"InheritanceOverview.png\"/>"), "<img src=\"InheritanceOverview.png\" alt=\"InheritanceOverview.png\" usemap=\"#inheritanceoverviewmap\"/>"))
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


# main:
replacementFiles = ('html/pages.html', 'html/annotated.html', 'html/hierarchy.html', 'html/inherits.html', 'html/classoverview.html');
for filename in replacementFiles:
  if not os.path.isfile(filename):
    print "file '"+filename+"' not found"
    sys.exit(-1)
  performReplacement(filename);


