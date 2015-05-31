#!/usr/bin/env python
import re, sys, os, subprocess, numpy, time, math, argparse, platform, getpass
from collections import defaultdict

# Define command line interface:
argparser = argparse.ArgumentParser()
argparser.add_argument("-r", "--rounds", type=int, default=20,
                    help="Number of times the benchmark executable will be called. More rounds improve result statistics.")
argparser.add_argument("-l", "--log", action="store_true",
                    help="Appends to the log file, additionally to printing the result on screen.")
argparser.add_argument("--logfile", default="./benchmark-log.txt",
                    help="Specify the benchmark log file, to which the results will be appended, if option --log is set.")
argparser.add_argument("-q", "--quiet", action="store_true",
                    help="Prevents printing any regular output on the screen. (e.g. in combination with --log)")
argparser.add_argument("-x", "--executable", default="./benchmark",
                    help="Specify the benchmark executable.")
argparser.add_argument("-c", "--comment", default="",
                    help="Add a comment line to the benchmark output")
argparser.add_argument("--anonymous", action="store_true",
                    help="Prevents user name and machine name to be included in the output.")
config = argparser.parse_args()

# Define used helper functions:
def listMean(lst):
  return float(sum(lst))/len(lst)

def listStd(lst):
  mu = listMean(lst)
  return math.sqrt(sum([(x-mu)**2 for x in lst]) / len(lst))

# The benchmark executable file path:
if not os.path.isfile(config.executable):
  print "Benchmark executable not found:", config.executable
  exit()

# Setup and start the actual benchmark loops
results = defaultdict(list)
namePattern = re.compile("RESULT : Benchmark::([^(]+)\\(\\):")
resultPattern = re.compile("\\s*(\\d+(\\.\\d+)?) msecs? per iteration.*")
qtVersionPattern = re.compile(".*Qt (\\d+\\.\\d+?\\.\\d+?).*")
maxNameLength = 0
qtVersion = ""

for i in range(config.rounds):
  if sys.stdout.isatty() and not config.quiet:
    print "iteration "+str(i)+"/"+str(config.rounds)+"    \r",
    sys.stdout.flush()
  proc = subprocess.Popen([config.executable], stdout=subprocess.PIPE)
  currentName = "";
  for line in proc.stdout:
   
    m = namePattern.search(line)
    if m:
      currentName = m.group(1)
      if len(currentName) > maxNameLength: maxNameLength = len(currentName)
   
    m = resultPattern.search(line)
    if m:
      results[currentName].append(float(m.group(1)))
    
    if qtVersion == "":
      m = qtVersionPattern.search(line)
      if m:
        qtVersion = m.group(1)

# Output result statistics:
proc = subprocess.Popen(["git", "status", "--porcelain", "--branch"], stdout=subprocess.PIPE)
gitBranch = re.search("## (.*)", proc.stdout.readline()).group(1)
proc = subprocess.Popen(["git", "log", "HEAD^..HEAD", "--oneline"], stdout=subprocess.PIPE)
gitHead = proc.stdout.readline().rstrip()
timeStamp = time.strftime("%Y-%m-%d %H:%M", time.localtime())

output = "*** Benchmark on "+timeStamp+" ***\n"
if not config.anonymous:
  output += "Platform: "+platform.platform()+", "+getpass.getuser()+" on "+platform.node()+"\n"
else:
  output += "Platform: "+platform.platform()+", anonymous\n"
output += "Qt Version: "+qtVersion+"\n"
output += "Git Branch: "+gitBranch+"\n"
output += "Git Head: "+gitHead+"\n"
output += "Rounds: "+str(config.rounds)+"\n"
if config.comment:
  output += "Comment: "+config.comment+"\n"

for name, times in sorted(results.iteritems()):
  namePadding = " "*(maxNameLength-len(name)+1);
  output += "{} {}{: >6.2f} +/- {: <4.2f} ms\n".format(name, namePadding, listMean(times), listStd(times))
output += "\n\n"

if not config.quiet:
  print output

if config.log:
  with open(config.logfile, "a") as logfile:
    logfile.write(output)

