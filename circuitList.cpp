#include "circuitList.h"

CircuitList::CircuitList(){
// Basic constructor for CircuitList class
    // this->length = 0;
    // this->numCNOT = 0;
    // this->optimizedLength = 0;
    // this->optimizedNumCNOT = 0;
    this->maxQubit = -1;
    
    return;
}

CircuitList::~CircuitList(){
// Destructor for CircuitList. Deletes all nodes still left in the circuit
    for(int i=this->maxQubit; i >= 0; i--){
        Gate* current = this->circuit[i]->head;
        
        while(current != NULL){
            Gate* previous = current;
            current = current->next;
            delete previous;
        }
    }

    return;
}

void CircuitList::add(Gate gate){
/* Purpose: Add a gate at the end of the circuit
    Input Parameter: Gate object
    Return: void
    Time Complexity: O(c)
    Note: If a gate is a CNOT gate, the first input gate is 
    the control, and the second is the target.
*/
    // Initialize new Node
    Gate* newGate = new Gate;
    newGate->gateType = gate.gateType;
    newGate->coefficient = gate.coefficient;
    newGate->targetQubit = gate.targetQubit;
    newGate->controlQubit = gate.controlQubit;
    newGate->lastControl = NULL;
    newGate->next = NULL;
    newGate->before = NULL;

    addQubits(gate.targetQubit, gate.controlQubit);

    QubitList* qubit = this->circuit[gate.targetQubit];

    // Update start if first node
    if(qubit->head == NULL){
        cout << "updated head" << endl;
        qubit->head = newGate;
    }
    
    // Update previous "last" gate if one exists
    if(qubit->tail != NULL){
        cout << "updated previous tail" << endl;
        qubit->tail->next = newGate;
        newGate->before = qubit->tail;
    }
    
    // Update "tail" gate
    cout << "updated tail" << endl;
    qubit->tail = newGate;

    if(gate.gateType == 1){
        // Initialize alert Node. I am the qubit that is the control
        // my target and control values are flipped
        Gate alertGate;
        alertGate.gateType = 5;
        alertGate.coefficient = 0;
        alertGate.targetQubit = gate.controlQubit;
        alertGate.controlQubit = gate.targetQubit;
        add(alertGate);

        newGate->lastControl = this->circuit[gate.controlQubit]->tail; 
    }
    cout << qubit->head->gateType << endl;
    print();
    return;
}

void CircuitList::addQubits(int target, int control){    

    while((control > this->maxQubit) or (target > this->maxQubit)){
        cout << "Adding qubit: " << this->maxQubit+1 << endl;
        QubitList* newQubit = new QubitList;
        newQubit->head = NULL;
        newQubit->tail = NULL;
        this->circuit.push_back(newQubit);
        this->maxQubit++;
    }
    return;
}

void CircuitList::print(){
/* Purpose: Print the circuit in readable QASM format
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    cout << "entered print" << endl;
    vector<Gate *> currents;
    for(int i=0; i<=this->maxQubit; i++){
        if(this->circuit[i]->head == NULL){
            cout << "head for qubit: " << i << " is empty" << endl;
        }
        currents.push_back(this->circuit[i]->head);
    }

    for(int i=0; i<=this->maxQubit; i++){
        Gate* current = currents[i];
        printFromCurrentToHere(current, this->circuit[i]->tail, currents);
        if((this->circuit[i]->tail != NULL) and (this->circuit[i]->tail->gateType != 5)){
            std::cout << GATETYPE[this->circuit[i]->tail->gateType] << ' ';
            
            if(this->circuit[i]->tail->coefficient != 0){
                std::cout << this->circuit[i]->tail->coefficient << ' ';
            }

            if(this->circuit[i]->tail->controlQubit != -1){
                cout << this->circuit[i]->tail->controlQubit << " ";
            }
            
            std::cout << this->circuit[i]->tail->targetQubit << ' ';
            
            std::cout << "\n";

        }
    }
    return;
}

void CircuitList::printFromCurrentToHere(Gate* current, Gate* here, vector<Gate*> &currents){
    // cout << "Entered printTo" << endl;

    while((current != NULL) and (current != here)){
        if(current->gateType == 5){
            printFromCurrentToHere(currents[current->controlQubit], current->lastControl, currents);
        }else if(current->gateType != 1){
            // cout << "Entered second while" << endl;
            std::cout << GATETYPE[current->gateType] << ' ';
            
            if(current->coefficient != 0){
                std::cout << current->coefficient << ' ';
            }
            
            std::cout << current->targetQubit << ' ';
            
            std::cout << "\n";
            
            current = current->next;

        }else{
            // cout << "Entered if" << endl;
            printFromCurrentToHere(currents[current->controlQubit], current->lastControl, currents);
            std::cout << "CNOT " << current->controlQubit << " " <<
                 current->targetQubit << endl;
            current = current->next;
            // currents[current->controlQubit] = currents[current->controlQubit]->next;
        }
    }
}

// Gate* CircuitList::getHead(Gate* current){
//     if(current != NULL){
//         Gate* prev = current->before;
//         while(prev != NULL){
//             current = current->before;
//             prev = current->before;
//         }
//         return current;
//     }
//     return NULL;
// }

