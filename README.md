mc-scatter
==========

Simple ray tracing code. Has two functions:

* monte carlo scattered light simulations
* column density maps at arbitrary angles

mc-scatter can be compiled for different problems using:

./compile "problem"

where "problem" is the name of the directory (within the problems directory) containing the prob.cpp and prob.h files you wish to compile.

See the sphere and heart problems for examples.

Coordinates
-----------

Thought the code, mc-scatter often refers to the angles theta and phi. These are defined as follows:

Theta is the angle away from positive x, in the x-y plane.
Phi is the angle out of the x-y plane. Phi = pi/2 points in the positive z direction, phi = -pi/2 points in the negative x direction

So theta = phi = zero looks down the positive x axis at the YZ plane.


Example images
--------------

Column density from the sphere test problem. With 500x500x500 density grid and 500x500 image grid.

<img src="https://github.com/drumber-1/mc-scatter-examples/raw/master/colden/sphere.png" alt="Galaxy column density" style="width: 300px;"/>

A snapshot of a fluid dynamic simulation, showing the column density at a variety of angles: (Black artifacts at the edge are due to the cube not extending further than the image)

<img src="https://github.com/drumber-1/mc-scatter-examples/raw/master/colden/disk_gal.gif" alt="Galaxy column density" style="width: 300px;"/>

