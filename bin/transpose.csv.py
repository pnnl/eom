#!/usr/bin/python

def transpose_csv_file(filename):
  import csv
  from itertools import izip
  from sys import stdout
  infile = open(filename,'r')
  reader = csv.reader(infile)
  transposed_rows = izip(*reader)
  csv.writer(stdout).writerows(transposed_rows)

def usage(return_value):
  print 'usage: transpose.csv.py CSVFILE'
  return return_value

if __name__ == "__main__":
  import sys
  if (len(sys.argv) != 2): sys.exit(usage(1))
  filename = sys.argv[1]
  transpose_csv_file(filename)
