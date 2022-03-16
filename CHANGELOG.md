# BioDeg Change Log

## Version 0.1 - June 2019
* Full 3D implementation of static degradation model for Mg
* Interface tracking based on Level set equation
* Modeling the formation of a protective film and the effect of Cl ions
* Automatic 3D mesh adaptation on the interface
* Simple domain decomposition using MPI
* Using MUMPS direct parallel solver to solve algebraic equations
* Ability to adaptively refine and (basically) coarsen the mesh during simulation
* Element-wise VTK output
* Based on FreeFem++ 3.6

## Version 0.2 - August 2019
* Ability to import complex geometries in Eulerian mesh
* Adding an ASCII art as the title

## Version 0.3 - November 2019
* Adding the pH changes modeling equation to the base model
* Making the code more configurable
* Changing the ASCII art to include university of Liege
* Implementing a non-linear solver for OH equation (not released finally in the code)
* Making the MPI parallelization scheme compatible with FreeFEM 4.x

## Version 0.4 - January 2020
* Massively parallel code using PETSc preconditioners and iterative solvers
* Restricted additive Schwarz Domain decomposition using HPDDM
* Using METIS for mesh partitioning
* Not compatible with FreeFEM versions prior to 4.4.3!
* Changing the VTK output to node-wise format (point data instead of cell data)

## Version 0.5 - June 2020
* Implementing a newton solver to calculate initial interface velocity
* Mesh import improvement to include more elements for the scaffold
* Adding more parameters and configuration settings
* Ability to save the last state of the model on a non-overlapping mesh
* Changing the boundary condition of Cl and OH equations to no-flux on the interface

## Version 0.6 - October 2020
* Ability to export the scaffold (material block) using Mmg
* Ability to interpolate state variables on the original non-partitioned mesh
* Dramatic mesh import improvement using chi function
* Support to read preprocessed distance function (output of an external tool)
* A minor change in the mesh partitioning technique
* Saving mass loss fraction to the text output file

## Version 0.7 - February 2021
* Adding fluid flow equation to the core model
* Adding convection effect on the transport equations
* Ability to switch between full Navier-Stokes and Stokes equation
* Ability to turn off flow effect and equation (or adjust if it should be solved every N steps)
* Fieldsplit preconditioners for fluid flow equation

## Version 0.8 - November 2021
* Replacing density and viscosity with dynamic viscosity in fluid equations
* Rewriting the redistancing code to be compatible with the parallelization scheme
* Considering the porous characteristic of the film for the fluid equation
* Fixing the convection behavior on the moving corrosion interface
* Fixing the MPI buffer issue in Windows
