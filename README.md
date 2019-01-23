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

## Internal Object Representation

NOTE: Understanding these representations is not necessary for usage of this program. It is merely described for clarity and for future development. 

In order to execute the logic and workflow described above, an internal syntax and structure for different varaibles used in the C++ module must be defined. Note, none of these data types are created until they enter the C++ module to maintain flexibility in the future applications of this plugin. Additionally, to allow for increased time and space efficiency, data structures are customized and linked together in different ways. These data structures and overall program structure will be described here. 


**Gate Object**

The most fundamental and core data structure used is the Gate Struct. As one would surmise, this Struct is the internal representation for a quantum gate and it contains all of the necessary information regarding the gate. It is defined in the circuitList.h file as follows.

`struct Gate{
    unsigned short int gateType;
    double coefficient;
    int targetQubit;
    int controlQubit;
    Gate* lastControl;
    Gate* next;
    Gate* before;
};`

While the first four values are self explanatory (gateType is stored as an unsigned short int as opposed to a string to reduce the space allocation), the three pointers could use clarification. The Gate pointers `next` and `before` are used to keep track of the preceeding and following Gates as one would typically image in a LinkedList. The `lastControl` Gate pointer is used to keep track of the memory address of the associated control or target instance of a CNOT gate. This is necessary because the implementation of these algorithms requires two gate objects for each CNOT gate (this is a simplification for development purposes, it may be possible to remove this requirement with further development). This rationale will be made clearer as we describe the rest of the data structures, yet it is important to note its occurence here. Additionally, it should be noted that not every value is necessary for every Gate - for example, a CNOT gate has no coefficient. While this is true, in order to simplify this program, these differences were not optimized for space complexity. In the future, this is an area where this implementation could be dramatically improved upon. 

**QubitList Object and Circuit** 

Now that we have defined and described the Gate data object, we come to the method of its storage in the program. As mentioned above, the Gate object contains pointers that allow for it to act as a node in a linked list. A trivial implementation (and in fact the first representation in this program's history) of a data structure representing a Quantum Circuit could be thought of as merely a singular linked list of each of the gates in order. While this is accurate, it dramatically increases the time complexity of the optimization algorithm. This is because gates need to be compared to the previous gates in the circuit to determine commutability and cancellation, yet it is understood that **single qubit gates** will only cancel if they act on the same qubit (similarly they will only not commute if they act on the same qubit). Hence, the QubitList Object and the inner circuit representation. In this program, the QubitList Object can be thought of as a qubit. It contains a pointer to the head and the tail of the linked list of gate objects that act on it (including CNOT gates). Then, for every qubit contained in the circuit, we have an associated QubitList instance that is stored in a vector (circuit). As mentioned prior, this current iteration of the program requires each CNOT gate to be represented twice. Once as a gate object in the target qubit's list and once in the control qubit's list. Below, a diagram of this structure is shown. 


## TODO:
  * Store CNOT gates as one gate object instead of two
  * Concurrent Optimization


