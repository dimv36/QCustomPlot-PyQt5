#!/usr/bin/env python

#
# This script compresses documentation PNGs in the html directory by reducing their palettes.
# The color count was chosen individually to achieve a good balance between image quality and
# file size. All PNGs found in the directory that are not specified in this script create a
# warning (set colorcount to 0 to skip image without compressing).
#

import os, sys, glob, subprocess

baseDir = sys.path[0];
os.chdir(baseDir) # change current working dir to script dir

# generate a list of (colorcount, filename) tuples.
# If colorcount is 0, no compression is performed.
fileList = []

# QCustomPlot images:
fileList += [(256 ,"html/RelationOverview.png")]
fileList += [(256 ,"html/LayoutsystemSketch.png")]
fileList += [(64  ,"html/layoutsystem-multipleaxisrects.png")]
fileList += [(64  ,"html/layoutsystem-addingplottitle.png")]
fileList += [(256 ,"html/InheritanceOverview.png")]
fileList += [(64  ,"html/AxisNamesOverview.png")]
fileList += [(64  ,"html/AxisRectSpacingOverview.png")]
fileList += [(0   ,"html/adaptive-sampling-scatter.png")] # is already compressed
fileList += [(0   ,"html/adaptive-sampling-line.png")] # is already compressed
fileList += [(0   ,"html/qcp-doc-logo.png")]
fileList += [(256 ,"html/QCPColorMap.png")]
fileList += [(256 ,"html/QCPColorMap-interpolate.png")]
fileList += [(64  ,"html/QCPColorMap-tightboundary.png")]
fileList += [(64  ,"html/QCPStatisticalBox.png")]
fileList += [(64  ,"html/QCPGraph.png")]
fileList += [(64  ,"html/QCPFinancial.png")]
fileList += [(64  ,"html/QCPCurve.png")]
fileList += [(64  ,"html/QCPBars.png")]
fileList += [(64  ,"html/QCPBarsGroup.png")]
fileList += [(64  ,"html/QCPLineEnding.png")]
fileList += [(64  ,"html/QCPMarginGroup.png")]
fileList += [(256 ,"html/QCPColorGradient-levelcount.png")]
fileList += [(256 ,"html/QCPColorGradient-periodic.png")]
fileList += [(0   ,"html/QCPColorGradient.png")]
fileList += [(256 ,"html/QCPColorScale.png")]
fileList += [(64  ,"html/QCPItemBracket-length.png")]
fileList += [(64  ,"html/QCPItemEllipse.png")]
fileList += [(64  ,"html/QCPItemText.png")]
fileList += [(64  ,"html/QCPItemRect.png")]
fileList += [(64  ,"html/QCPItemBracket.png")]
fileList += [(64  ,"html/QCPItemTracer.png")]
fileList += [(64  ,"html/QCPItemLine.png")]
fileList += [(64  ,"html/QCPItemStraightLine.png")]
fileList += [(64  ,"html/QCPItemCurve.png")]
fileList += [(64  ,"html/QCPItemPixmap.png")]
fileList += [(8   ,"html/ssDot.png")]
fileList += [(8   ,"html/ssCross.png")]
fileList += [(8   ,"html/ssPlus.png")]
fileList += [(8   ,"html/ssCircle.png")]
fileList += [(8   ,"html/ssDisc.png")]
fileList += [(8   ,"html/ssSquare.png")]
fileList += [(8   ,"html/ssDiamond.png")]
fileList += [(8   ,"html/ssStar.png")]
fileList += [(8   ,"html/ssTriangle.png")]
fileList += [(8   ,"html/ssTriangleInverted.png")]
fileList += [(8   ,"html/ssCrossSquare.png")]
fileList += [(8   ,"html/ssPlusSquare.png")]
fileList += [(8   ,"html/ssCrossCircle.png")]
fileList += [(8   ,"html/ssPlusCircle.png")]
fileList += [(8   ,"html/ssPeace.png")]

# Doxygen images:
fileList += [(16  , f) for f in glob.glob("html/class*__inherit__graph.png")]
fileList += [(16  , f) for f in glob.glob("html/inherit_graph_*.png")]
fileList += [(16  ,"html/ftv2blank.png")]
fileList += [(16  ,"html/ftv2plastnode.png")]
fileList += [(16  ,"html/ftv2doc.png")]
fileList += [(16  ,"html/closed.png")]
fileList += [(16  ,"html/nav_f.png")]
fileList += [(16  ,"html/ftv2pnode.png")]
fileList += [(16  ,"html/ftv2node.png")]
fileList += [(16  ,"html/tab_s.png")]
fileList += [(16  ,"html/bdwn.png")]
fileList += [(16  ,"html/ftv2mo.png")]
fileList += [(16  ,"html/ftv2mnode.png")]
fileList += [(16  ,"html/sync_off.png")]
fileList += [(16  ,"html/open.png")]
fileList += [(16  ,"html/bc_s.png")]
fileList += [(16  ,"html/tab_h.png")]
fileList += [(16  ,"html/tab_b.png")]
fileList += [(16  ,"html/ftv2folderopen.png")]
fileList += [(16  ,"html/ftv2cl.png")]
fileList += [(16  ,"html/ftv2splitbar.png")]
fileList += [(16  ,"html/ftv2mlastnode.png")]
fileList += [(16  ,"html/tab_a.png")]
fileList += [(16  ,"html/ftv2folderclosed.png")]
fileList += [(16  ,"html/nav_h.png")]
fileList += [(16  ,"html/ftv2ns.png")]
fileList += [(16  ,"html/ftv2link.png")]
fileList += [(16  ,"html/ftv2vertline.png")]
fileList += [(16  ,"html/nav_g.png")]
fileList += [(16  ,"html/ftv2lastnode.png")]
fileList += [(16  ,"html/sync_on.png")]
fileList += [(2   ,"html/doxygen.png")]

allPngFiles = glob.glob("html/*.png")
for (colors, fileName) in fileList:
  allPngFiles.remove(fileName)
  if colors > 0:
    print "compressing colors of '"+fileName+"'"
    if subprocess.call("mogrify -colorspace RGB -colors "+str(colors)+" +dither "+fileName, shell=True) != 0:
      print "ERROR: color compression failed for '"+fileName+"'"


for fileName in allPngFiles:
  print "WARNING: documentation image not color compressed (was not specified in script): '"+fileName+"'"

