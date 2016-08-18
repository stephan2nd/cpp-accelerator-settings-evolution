#!/usr/bin/python

import sys
import math
from functools import partial
import matplotlib.pyplot as plt


fobj = open(sys.argv[1], "r")


if sys.argv[2]=="hist2d":
	x = []
	y = []
	z = []
	first = True
	myBins = 0
	for line in fobj:
		if first:
			myBins =  float(line.rstrip())
			first = False
		else:
			linestr = str(line.rstrip())
			linearr = linestr.split(",")
			x.append(float(linearr[0]))
			y.append(float(linearr[1]))
			z.append(float(linearr[2]))	    
	fobj.close()

	plt.hist2d(x, y, bins=myBins, weights=z)
	plt.colorbar()
	plt.show()

elif sys.argv[2]=="hist":
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
	
else: 

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