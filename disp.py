#!/usr/bin/python

import sys
import math
from functools import partial
import matplotlib.pyplot as plt
import numpy as np

fobj = open(sys.argv[1], "r")
diagram_type = fobj.read().split('\n', 1)[0]
fobj.close()

print "diagram type =", diagram_type 

def hist2d(): #if sys.argv[2]=="hist2d":
	fobj = open(sys.argv[1], "r")
	x = []
	y = []
	z = []
	line_nr = 0
	xBins = 0
	yBins = 0
	for line in fobj:
		if line_nr == 0:
			pass
		elif line_nr == 1:
			xBins =  float(line.rstrip())
		elif line_nr == 2:
			yBins =  float(line.rstrip())					
		else:
			linestr = str(line.rstrip())
			linearr = linestr.split(",")
			x.append(float(linearr[0]))
			y.append(float(linearr[1]))
			z.append(float(linearr[2]))	 
		line_nr += 1   
	fobj.close()

	plt.hist2d(x, y, bins=[xBins, yBins], weights=z)
	plt.colorbar()
	plt.show()



def func():
	fobj = open(sys.argv[1], "r")
        x = []
        y = []
	line_nr = 0
        for line in fobj:
		if line_nr == 0:
			pass
		else:
			linestr =  str(line.rstrip())
			linearr = linestr.split(",")
			x.append(float(linearr[0]))
			y.append(float(linearr[1]))
		line_nr += 1
	fobj.close()

	#plt.semilogy(y)
	plt.plot(y)
	plt.title('max fitness per generation') 
	plt.grid(True)
	plt.show()



if diagram_type == "hist2d":
        hist2d()
elif diagram_type == "func":
	func()




def hist():
	x = []
	z = []
	for line in fobj:
	    linestr =  str(line.rstrip())
	    linearr = linestr.split(",")
	    x.append(float(linearr[0]))
	    z.append(float(linearr[1]))	    
	fobj.close()
 
	#plt.autoscale(enable=False, axis='x', tight=True)
	plt.hist(x, bins=100, weights=z)
	plt.xlim(-50,50)
	plt.show()

def other():
	x = []
	y = []
	s = []
	for line in fobj:
	    linestr =  str(line.rstrip())
	    linearr = linestr.split(",")
	    x.append(float(linearr[0]))
	    y.append(float(linearr[1]))
	    s.append(float(linearr[2]))
	fobj.close()
	
	miny = 0;
	maxy = 0;
	minx = min(x);
	maxx = max(x);
	if sys.argv[2]=="1" or sys.argv[2]=="all": 
		miny = 1.5*min(y)
		maxy = 1.5*max(y)
		plt.plot(x,y, 'r-')
	if sys.argv[2]=="2" or sys.argv[2]=="all": 
		miny = 1.5*min(s)
		maxy = 1.5*max(s) 
		plt.plot(x,s, 'k-')
	if sys.argv[2]=="cor": 
		minx = 1.5*min(y)
		maxx = 1.5*max(y)
		miny = 1.5*min(s)
		maxy = 1.5*max(s) 
		plt.plot(y,s, 'k.')
	
		
	plt.axis([minx, maxx, miny, maxy])
	plt.show()


