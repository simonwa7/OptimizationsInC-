''' 
Purpose: 
This script uses the generateCircuit class (dependent on OpenFerimon and
Psi4) to generate a circuit in QASM format that represents the time evoltuion
of the hamiltonian through the Trotter-Suzuki decomposition method. Through the
command line, a client will pass the molecular definition and once created, the
circuit is parsed and each gate is converted to an internal gate representation
before being passed to the CircuitList class (written in C++). This class is
wrapped (making it useable in Python) through the qcircuit module (which must
be built and installed locally prior to running this script). The CircuitList 
class then uses internal optimization techniques to reduce possible gate
cancellations. The gate count statistics are then printed to a datafile.

INPUT FORMAT (all on one line):
               python main.py [name] [multiplicity] [charge] [mapping]/
for each atom: [atomic symbol] [z coord] [y coord] [x coord]/
               [basis]
           OR:
               python main.py [name] pubchem [multiplicity] [charge]/
               [mapping] [basis] 

EXAMPLE FORMAT:
python main.py H2 1 0 JW H 0 0 0 H 0.7414 0 0 sto-3g
^above simulates the H2 molecule at equilibrium bond-length

TODO:

Biggest Time Concerns:
    generating circuit with OpenFermion
    *optimizing circuit with CircuitList

'''
import qcircuit
from generateCircuit import GenerateCircuit
import sys
import time
import array
from openfermion.utils import count_qubits

def parse_inputs(input):
    ''' 
    ARGS: 
        input: string (sys.argv)
    RETURNS:
        geometry: list of tuples in format: 
            ([atomic symbol], ([z], [y], [x]))
        basis: string for the calculation of the molecular integrals
        name: string for the name of the molecule
        multiplicity: integer
        charger: integer
        mapping: string (either BK or JW) representing the desired transformation
    Purpose:
        This function parses the command line arguements to the syntax desired
        in this script.
    '''
    # Parse input arguements
    name = str(input[1])
    if(len(input) == 7):
        # In this case, the client has chosen to get the geometry from the 
        # pubchem database. This is possible through OpenFermion, so the 
        # geometry is just set to "pubchem" from the input 
        geometry = str(input[2])
        multiplicity = int(input[3])
        charge = int(input[4])
        mapping = str(input[5])
        basis = str(input[6])
    elif((len(input)-6)%4 == 0):
        # In this case, the client has chosen to specify the geometry on input.
        # Because each additional atom requires 4 parameters (name z, y, x), the
        # number of input parameters should be 4n+6
        multiplicity = int(input[2])
        charge = int(input[3])
        mapping = str(input[4])
        geometry = list()
        num_atoms = (len(input)-6)/4
        basis = str(input[len(input)-1])
        for index in range(num_atoms):
            atom_index = (index*4)+5
            atom = tuple((str(input[atom_index]), 
                (float(input[atom_index+1]),float(input[atom_index+2]),
                 float(input[atom_index+3]))))
            geometry.append(atom)
    else:
        sys.exit("Incorrect command line arguements")

    return geometry, basis, name, multiplicity, charge, mapping


def getCircuit(name, geometry, basis, multiplicity, charge, mapping):
    '''
    ARGS:
        name: string representing the name of the molecule
        geometry: list of tuples in format: 
            ([atomic symbol], ([z], [y], [x]))
        basis: string representing the basis for the calculation
        multiplicity: integer
        charge: integer
        mapping: string (either BK or JW) to determine the desired 
            transformation
    RETURNS:
        circuit: generator object that returns QASM strings
        n_qubits: integer for the number of qubits needed for simulation

    Pupose:
        This function is designed to interact with the generateCircuit wrapper
        for OpenFermion. Its main functionality is to provide the generator
        object produced by OpenFermion for the QASM circuit of the molecule
    '''
    # Get the wrapper
    molecule = GenerateCircuit() 
    
    # Set variables
    molecule.set_name(name)
    if(geometry == "pubchem"):
        print(name)
        molecule.get_geometry_from_pubchem()
    else:
        molecule.set_geometry(geometry)
    molecule.set_basis(basis)
    molecule.set_multiplicity(multiplicity)
    molecule.set_charge(charge)

    # load molecule
    molecule.load_molecule() 
    # create the hamiltonians
    molecule.create_hamiltonians()
    n_qubits = count_qubits(molecule.fermion_hamiltonian)
    molecule.create_circuits(mapping)
    if(mapping == "BK"):
        return [molecule.bk_circuit, n_qubits]
    elif(mapping == "JW"):
        return [molecule.jw_circuit, n_qubits]
    else:
        sys.exit("Didn't understand mapping")
   

if __name__ == '__main__':
    geometry, basis, name, multiplicity, charge, mapping = parse_inputs(sys.argv)
    
    # used for debugging
    print geometry, "\n", basis
    name = name.replace("_", " ")
    
    # initialize empty array to store the QASM strings
    qasm = [];
    # time the process of getting the circuit
    start = time.time()
    # circuit is a generator object to get the next gate in the circuit
    circuit, n_qubits = getCircuit(name, geometry, basis, multiplicity, charge, mapping)
    
    # in order to test the timing of the methods appropriately, we need to
    # load the entire circuit into RAM. Hence we loop through the returned
    # generator object so that no more time is needed to generate the next gate
    for line in circuit:
        qasm.append(line);
    time_to_generate = time.time()-start
    
    name = name.replace(" ", "_")
    
    # used for debugging
    if(mapping == "BK"):
        print("BRAVYI-KITAEV MAPPING\n")
    elif(mapping == "JW"):
        print("\nJORDAN-WIGNER MAPPING")
        
    # time the process of adding and optimizing 
    start = time.time()
    for line in qasm:
        # add and optimize each line through the process. A negligible amount of
        # time is spent translating from QASM to the gate object required by the
        # circuitList class
        qcircuit.addAndOptimizeGate(line)
    time_to_loop = time.time()-start
    
    gate_count = qcircuit.get("length")
    CNOT_count = qcircuit.get("numCNOT")
    optimized_gate_count = qcircuit.get("optimizedLength")
    optimized_CNOT_count = qcircuit.get("optimizedNumCNOT")

    # record data and performance information
    dataFileName = 'data.txt'
    recorded_data = open(dataFileName, "a") 
    recorded_data.write("{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11}\n"
                     .format(name, n_qubits, mapping,
                             basis, multiplicity, charge, 
                             gate_count, CNOT_count, 
                             optimized_gate_count, optimized_CNOT_count,
                             time_to_generate, time_to_loop)
                     )
                     
    recorded_data.close();