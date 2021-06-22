#!/usr/bin/python

import struct
import sys
import signal
import getopt
import csv
import subprocess

signal.signal(signal.SIGPIPE, signal.SIG_DFL)

def usage():
  print "usage: binaries.py <command>"
  print "commands:"
  print "\tsample\tduplicate this directory with only a sampling of generators and load"
  print "\ttxt2bin\tconvert text input file to binary blob file"
  print "\tbin2txt\tconvert binary blob file to text; one value per line on stdout"
  print "\taddRamp\tadd a ramp to a binary blob file"
def main():
  command = None
  try:
    command = sys.argv[1]
  except IndexError:
    pass
  args = sys.argv[2:] # rest of args passed to command
  if ("sample" == command):
    sampleGen(args)
  elif ("txt2bin" == command):
    text_to_binary(args)
  elif ("bin2txt" == command):
    binaryToText(args)
  elif ("addRamp" == command):
    addRamp(args)
  else:
    usage()
    sys.exit(1)
def addRampUsage():
  print "usage: binaries.py addRamp [START] [STEPS] [FILES]"
  print "Add a ramp to the given binary blob files.  Use the given start value.  Increase values linearly until reaching the first value in the file."
def addRamp(args):
  try:
    start = int(args[0])
    steps = int(args[1])
    filenames = args[2:]
  except (ValueError, IndexError):
    addRampUsage()
    sys.exit(1)
  for filename in filenames:
    numbers = readBinaryLoadBlob(filename)
    stop = numbers[0]
    ramp = [start+x*(stop-start)/float(steps) for x in range(0,steps)]
    writeBinaryLoadBlob(filename, ramp + numbers)
def text_to_binary_usage():
  print "usage: binaries.py txt2bin [INFILE] [OUTFILE]"
def text_to_binary(args):
  if 2 != len(args):
    text_to_binary_usage()
    sys.exit(1)
  input_stream = open(args[0])
  lines = input_stream.readlines()
  input_stream.close()
  load = [float(line) for line in lines]
  writeBinaryLoadBlob(args[1], load)
def binaryToTextUsage():
  print "usage: binaries.py bin2txt [FILES]"
def binaryToText(args):
  if 1 > len(args):
    binaryToTextUsage()
    sys.exit(1)
  for filename in args:
    numbers = readBinaryLoadBlob(filename)
    for number in numbers: print number
def sampleGenUsage():
  print "usage: binaries.py sample [SIZE]..."
  print "\tSIZE\tpositive integers"
  print "Duplicates this directory then updates the load and generators file in the duplicate.  The duplicate will have n generators sampled evenly from the original generator set and resulting loads will be multipled by n/original_number_of_generators."
def sampleGen(args):
  try:
    sizes = [int(arg) for arg in args]
  except (ValueError, IndexError):
    sizes = None
  if (sizes == None or [size > 0 for size in sizes] != [True for x in range(0,len(sizes))]):
    sampleGenUsage()
    sys.exit(1)
  # sizes are now positive integers
  for size in sizes:
    dir = "../case" + str(size) + "/"
    subprocess.call(['mkdir', dir, '-p'])
    subprocess.call('cp -r * ' +  dir, shell=True)
    generateCase(dir, size)

def generateCase(dir, size):
  file = open(dir + 'generators.csv')
  generatorLines = file.readlines()
  file.close()
  
  file = open(dir + 'generators.csv','w')
  file.write(generatorLines[0]) # write header
  file.write("".join(sample(size, generatorLines[1:])))
  file.close()

  fraction = size / float(len(generatorLines)-1) # -1 for header

  multiplyLoadFileBy(dir + 'LoadData.uc', fraction)
  multiplyLoadFileBy(dir + 'WindData.uc', fraction)
  multiplyLoadFileBy(dir + 'SolarData.uc', fraction)

def sample(n, values):
  return [values[index] for index in [int(x*len(values)/float(n)) for x in range(0,n)]]

def multiplyLoadFileBy(filename, fraction):
  load = readBinaryLoadBlob(filename)
  writeBinaryLoadBlob(filename, [fraction * value for value in load])

def readBinaryLoadBlob(filename):
  file = open(filename)
  data = file.read()
  file.close()
  numbers = [struct.unpack('d',data[index:index+8])[0] for index in range(0, len(data),8)]
  return numbers

def writeBinaryLoadBlob(filename, values):
  file = open(filename, 'w')
  file.write("".join([struct.pack('d',value) for value in values]))
  file.close()

if __name__ == "__main__":
  main()
