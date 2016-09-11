#!/usr/bin/python

import sys
import matplotlib.pyplot as plt

ax = [0,0,0,0,0,0,0,0]

if len(sys.argv) < 10:
	f, ((ax[0], ax[1], ax[2], ax[3]), (ax[4], ax[5], ax[6], ax[7])) = plt.subplots(nrows=2, ncols=4, sharex='col', sharey='row')

	for i in range(1,len(sys.argv)):
	
		filename = sys.argv[i]
		print "opening ", filename
		
		fobj = open(filename, "r")
		diagram_type = fobj.read().split('\n', 1)[0]
		fobj.close()
		
		print "diagram type =", diagram_type 
		
		fobj = open(filename, "r")
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

		ax[i-1].hist2d(x, y, bins=[xBins, yBins], weights=z)

	
#	plt.colorbar()
	plt.show()
		
		