# Fragmentsolver
Fragmentsolver is a tool for computing the restricted Hartree Fock energy given a Fragment as Input in form of a FCIDUMP.
The current FCIDUMP is build from water but you can replace it with your FCIDUMP.
A FCIDUMP is a list of one- and two-electron integrals as well as the nuclear Energy.
Molpro and Cryscor can be used to create new FCIDUMPs.
If you just want to try out you can also take one of the FCIDUMPs from the FCIDUMPs folder.

Some mathematical background behind this algorithm can be found in the workingequations file.

# Dependencies
Fragmentsolver is made using taco (https://github.com/tensor-compiler/taco).
Molpro (https://www.molpro.net/) or Cryscor for creating FCIDUMPs

# Acknowledgements
Fragmentsolver is made with Eclipse. So it should be most easy to use it by importing it into Eclipse.

# Usage
Start the programm by calling it (Fragmentsolver).

You get asked which FCIDUMP you want to have as input (../FCIDUMPs/Water).

Next you get asked how many electrons you want to add (-4).

Then it prints the number of orbitals and of electrons so you can check that the correct FCIDUMP was used.

Then the rest of the output follows:

Iteration    Energy          Ediff

1        -69.7757349     69.7757349

2        -70.5991837      0.8234488

3        -70.6032847      0.0041011

4        -70.6034191      0.0001344

5        -70.6034304      0.0000113

6        -70.6034316      0.0000012

7        -70.6034317      0.0000001

The Energy is -70.6034317

It prints for every iteration the number of iteration, the current energy and the energy difference to the last energy.
After convergence is achieved it prints out the resulting energy of the calculation.
