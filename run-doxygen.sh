#!/bin/bash
cd "$( dirname "$0" )"
rm -r documentation/html
mkdir documentation/html

# let doxygen generate qhp index and html hierarchy from source files:
doxygen

# do post-processing on documentation images and html hierarchy:
echo Post-processing documentation images...
./documentation/postprocess-doc-images.py
echo Post-processing documentation html...
./documentation/postprocess-doc-html.py

# generate qhc file from qhp index and html for QtAssistant integration:
echo Building qhc...
qhelpgenerator documentation/html/index.qhp -o documentation/qthelp/qcustomplot.qch

# cleanup html hierarchy:
rm documentation/html/index.qhp
rm documentation/html/*.map
rm documentation/html/*.md5

