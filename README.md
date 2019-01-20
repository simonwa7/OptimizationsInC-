# QuantumCircuitOpt-C <br> (Quantum Circuit Optimization in Python and C++)

Author: William Simon (Building on the work done by Andrew Tranter)

## Purpose
This project is designed for research purposes to compare the Bravyi-Kitaev and Jordan-Wigner mappings both
pre and post quantum circuit optimization techniques. With the increasing drive to compute useful quantum 
circuits on realized hardware, yet the physical limitations of current hardware, preprocessing of quantum circuits 
could provide extremely valuable. This preprocessing routine works to resolve any possible gate cancellations that
would not affect the final result of the circuit. 

For usability, this program interfaces with the [OpenFermion](https://github.com/quantumlib/OpenFermion), 
[OpenFermion-Psi4](https://github.com/quantumlib/OpenFermion-Psi4), and
[Psi4](http://psicode.org/) packages, while it also outsources the computationally expensive optimization
routine into a locally installed C++ module that is wrapped for Python. 

This tool was used to show that the two mapping schemes produce similar asymptotic trends for overall optimized gate 
count and optimized CNOT gate count as a function of the number of qubits. Additionally, in molecules larger than 
30 qubits, this program showed a reduction of gate counts by larger than 80%. 

Below, a diagram of the workflow is shown and further descriptions of the theory, usage, and internal representation
are detailed.

![diagram](https://github.com/simonwa7/QuantumCircuitOpt-C/blob/parallel-lists/QCircuitOpt.jpg "Diagram")

## Theory
While OpenFermion vastly expands the field to less experienced users, the internal functions do not create the most
optimized circuit structure. The result is an exponential scaling in circuit length as the molecule size increases. 
Located in these circuits are instances of gate pairs that either resolve to the identity matrix when multiplied, or
can be added together, resulting in no change to the computation of the circuit, yet requiring longer computation on 
a Quantum Computer.

These gate cancellations are attempted to be found one by one, and removed from the circuit in the preprocessing 
routine - reducing the circuit length. Given a circuit, gates are added to the internal structure iteratively. When
first added, the gate is checked against the previous gate in the circuit to see if a cancellation can be made. If they
can be removed, the circuit is then updated, if not, then a check is made to see if the two gates commute. If they can
commute, the process moves on to the next previous gate in the circuit until either: a cancellation is found, two gates
do not commute, or the beginning of the circuit is reached. 

## Usage
Note, each local enviornment requires different installations and flows, however, this is an attempt to outline the basic
structure.

1. Build the qcircuitmodule: `python setup.py build`
2. Install the qcircuitmodule: `python setup.py install --user`
3. Run (2 options): 
  * `python main.py [molecule name] [multiplicity] [charge] [mapping]` for each atom: `[atomic symbol] [z coord] [y coord] [x coord]` then `[basis]`
  * `python main.py [molecule name] pubchem [multiplicity] [charge] [mapping] [basis]`
  
  ex: 
  * `python main.py H2 1 0 JW H 0 0 0 H 0.7414 0 0 sto-3g`
  * `python main.py Hydrogen pubchem 1 0 BK sto-3g`
  
molecule name - string with the name of the molecule (if using pubchem database, must be their name)

multiplicity - integer with the calculated multiplicity (2S+1)

charge - integer, self explanatory

mapping - string, either JW or BK for Jordan-Wigner or Bravyi-Kitaev respectively

atomic symbol - string, atomic symbol from the periodic table

coordinates - float, cartesian coordinates in 3 dimensional space

basis - string with the chosen basis set for molecular integral calculation. See Psi4 documentation for possible basis sets

## Internal Representation


## TODO:
  * Concurrent Optimization


