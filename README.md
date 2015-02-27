mc-scatter
==========

Simple ray tracing code. Has two functions:

* monte carlo scattered light simulations
* column density maps at arbitrary angles

In order to be compiled mc-scatter needs the following libraries:

* gnu readline
* lua

mc-scatter can be compiled using make. The makefile may need to be modified so that mc-scatter can locate the required headers and libraries on a given machine. The bin directory can be add to your systems $PATH environmental variable for easier access.

Sphere example problem
----------------------

To run the sphere example problem, first compile mc-scatter as described above, navigate to examples/sphere and run the mc-scatter binary. Either in serial:

```$ mc-scatter```

or parallel:

```$ mpirun -n 8 mc-scatter```


The config.lua file defines the parameters of the grid, and will automatically be read by mc-scatter. This can be checked using ```info config```.

Currently the grid is empty, as can be seen from ```info grid```. Read in the density structure using ```read lua density.lua```. ```info grid``` will now show the the grid has been initialised.

Add a column density image to be created, using ```addimage colden <theta> <phi>```. The theta and phi variables will determine the angle (in degress) at which the column density will be taken. As the example is a simple sphere all angles are equivilent, so ```addimage colden 0 0``` will do. ```info images``` will shown that a column density image has been staged.

Use ```colden``` to tell mc-scatter to perform the calculations. The image data will be output in the directory specified by colden\_location in config.lua, by default this is ./data/colden. Quit mc-scatter with ```q``` or ```quit```. The data can then be plotted using the make\_plot.py python script.


Coordinates
-----------

Thought the code, mc-scatter often refers to the angles theta and phi. These are defined as follows:

Theta is the angle away from positive x, in the x-y plane.
phi is the angle out of the x-y plane: phi = pi/2 points in the positive z direction, phi = -pi/2 points in the negative z direction.


Example images
--------------

Column density from the sphere test problem. With 500x500x500 density grid and 500x500 image grid.

<img src="https://github.com/drumber-1/mc-scatter-examples/raw/master/colden/sphere.png" alt="Galaxy column density" style="width: 300px;"/>

A snapshot of a fluid dynamic simulation, showing the column density at a variety of angles: (Black artifacts at the edge are due to the cube not extending further than the image)

<img src="https://github.com/drumber-1/mc-scatter-examples/raw/master/colden/disk_gal.gif" alt="Galaxy column density" style="width: 300px;"/>

