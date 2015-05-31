#!/usr/bin/env python
import re, sys

if len(sys.argv) != 2:
  print "provide code file as first argument";
  exit();

f = open(sys.argv[1], "r");
line = f.readline();

hasdoc = False;
linenumber = 0;
while line:
  linenumber = linenumber + 1;
  # warn for undocumented functions and ctors:
  if (re.search("^\w+ ", line) or re.search("^\w+::[^~]", line)) and not hasdoc:
     print "line "+str(linenumber)+": "+line[0:-1];
  # warn for documented dtors:
  if (re.search("^\w+::~", line)) and hasdoc:
     print "dtor has doc, line "+str(linenumber)+": "+line[0:-1];
  hasdoc = line.startswith("*/") or line.startswith("/*");
  line = f.readline();

f.close();

