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

        void add(Gate newGate);
        void addAndOptimize(Gate gate);

        void print();
        void saveQASM(string outputName);
        
    
    private:
        short int maxQubit;
        std::vector<QubitList*> circuit;

        void addQubits(int target, int control);

        bool checkIfGatesCommute(Gate* gate1, Gate* gate2);
        bool checkIfGatesCancel(Gate* gate1, Gate* gate2);

        void removeNext(Gate gate, Gate* nextGate);
        void removeGate(Gate* gate);

        void printBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents);
        void saveBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents, 
                                ofstream &qasm);
};

#endif