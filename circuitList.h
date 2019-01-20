/*  Class Name: CircuitList
    Author: William Simon
    On: 8/1/2018
    
    Purpose: This class is designed to be a representation of a quantum circuit.
    
    It is implemented using a linked list structure wherein each node is a
    quantum gate which points to the next gate in the circuit. 
    
    The main operation of this class apart from:
        adding gates to the end of the circuit, looping through the circuit, and 
        printing the circuit
    is to use circuit optimization techniques to resolve gates to the unitary 
    (therefore removing them from the circuit) if they will result in an 
    equivalent circuit. This will therefore reduce the overall size of the
    circuit will keeping the result the same. 
    
    Time Complexity: Worst Case: O(g^3), but most likely O(g^2) or O(g)
    
    Future additions: 
            Expand possible gate cancellations 
            Control number of iterations
            Remove certain functions from public domain
*/

#ifndef CIRCUITLIST_H_
#define CIRCUITLIST_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

// An array of the possible gate types. This allows gate type to be an integer
//      attribute to reduce time and memory.
const std::string GATETYPE[] = {"H", "CNOT", "Rx", "Rz", "CNOT (control)"};

// Declaration of the Gate object which also acts as the Node in a doubly 
//		linked list.
struct Gate{
    unsigned short int gateType;
    double coefficient;
    int targetQubit;
    int controlQubit;
    Gate* lastControl;
    Gate* next;
    Gate* before;
};	

struct QubitList{
    Gate* head;
    Gate* tail;
};

class CircuitList{
    public: 
        CircuitList();
        ~CircuitList();

        // add gate to the end of the circuit
        void add(Gate newGate);

        // add gate to the end of the circuit and attempt to cancel this gate
        void addAndOptimize(Gate gate);

        // print out circuit in order in QASM format
        void print();
        // save circuit in QASM
        void saveQASM(string outputName);
        // print length, numCNOT, optimizedLength, and optimizedNumCNOT
        void printLengths();

        // getter functions
        unsigned long long getLength();
        unsigned long long getNumCNOT();
        unsigned long long getOptimizedLength();
        unsigned long long getOptimizedNumCNOT();
    
    private:
        unsigned long long length;
        unsigned long long numCNOT;
        unsigned long long optimizedLength;
        unsigned long long optimizedNumCNOT;

        // maximum qubit index in current circuit
        short int maxQubit;
        // vector of pointers to qubitlists to represent the circuit
        std::vector<QubitList*> circuit;

        // add qubits to the circuit
        void addQubits(int target, int control);

        // check the previous gates in the qubitlist for possible cancellations
        Gate* checkPreviousGates(Gate gate);
        // determine if two gates commute
        bool checkIfGatesCommute(Gate* gate1, Gate* gate2);
        // determine if two gates cancel
        bool checkIfGatesCancel(Gate* gate1, Gate* gate2);

        // removes nextGate while appropriately updating (and possibly
        // cancelling) other gates
        void removeNext(Gate gate, Gate* nextGate);
        // deletes the gate from the circuit
        void removeGate(Gate* gate);

        // recursive helper functions for print() and saveQASM()
        void printBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents);
        void saveBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents, 
                                ofstream &qasm);
};

#endif