import numpy as np
import matplotlib.pyplot as plt
import scipy.interpolate
import math
import matplotlib.cm as cm
import sys

def parseArg(argv):
	global inputfile
	global outputfile
	inputfile = argv[0]
	if inputfile == '':
		print 'plot.py <inputfile>'
		sys.exit(2)
	return inputfile
	

def do_plot(inputfile, log = False, removeAxes = True):
	
	#Get data
	try:
		data = np.genfromtxt(inputfile)
	except IOError:
		print "Unable to read file " + inputfile
		sys.exit(1)

	x = data[:,0]
	y = data[:,1]
	if log:
		z = np.log10(data[:,2] + 1e-99)
	else:
		z = data[:,2]
	
	grid_size2 = len(x)
	grid_size = math.sqrt(grid_size2)
	
	if(grid_size % 1 != 0):
		print "There is not a square number of grid points (" + str(grid_size2) + ")"
		sys.exit(1)
		
	grid_size = int(grid_size)
		
	print "Grid is " + str(grid_size) + " by " + str(grid_size)
	
	z_grid = np.reshape(z, (-1, grid_size))
	
	zmin = np.inf
	zmax = -np.inf
	
	for n in z:
		if n < zmin and not np.isinf(n):
			zmin = n
		if n > zmax and not np.isinf(n):
			zmax = n
	
	if removeAxes:
		fig = plt.figure(frameon=False)
		fig.set_size_inches((x.max()-x.min())*0.01,(y.max()-y.min())*0.01)
		ax = plt.Axes(fig, [0., 0., 1., 1.])
		ax.set_axis_off()
		fig.add_axes(ax)
	
	plt.imshow(z_grid, vmin=zmin, vmax=zmax, origin='lower', extent=[x.min(), x.max(), y.min(), y.max()], cmap = cm.gray, interpolation = 'none')	
	if not removeAxes:
		cbar = plt.colorbar()
	outputfile = inputfile.rpartition('.')[0] + '.png'
	#plt.savefig(outputfile, dpi=300, bbox_inches="tight")
	plt.savefig(outputfile, dpi=150)
	plt.clf()
		
	print 'Plotted ' + outputfile

if __name__ == "__main__":
	fname = parseArg(sys.argv[1:])
	do_plot(fname)


