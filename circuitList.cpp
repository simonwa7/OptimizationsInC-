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

        delete circuit[i];
    }

    return;
}

void CircuitList::add(Gate gate){
    /* Purpose: Add a gate at the end of the circuit
        Input Parameter: Gate object
        Return: void
        Time Complexity: O(c)
        Note: For every gate, the targetQubit value is the qubit that is
        being acted on. Only in the CNOT gate is the controlQubit value
        populated. When a CNOT gate is added, two gates are added to this
        data structure. The target gate acting on the current qubit and 
        the control gate - with pointers in the lastControl instance pointing 
        to each other.

        GateType == 1 means a target CNOT
        GateType == 5 means a control CNOT
    */
    // Initialize new Node
    Gate* newGate = new Gate;
    newGate->gateType = gate.gateType;
    newGate->coefficient = gate.coefficient;
    newGate->targetQubit = gate.targetQubit;
    newGate->controlQubit = gate.controlQubit;
    newGate->lastControl = gate.lastControl;
    newGate->next = NULL;
    newGate->before = NULL;

    addQubits(gate.targetQubit, gate.controlQubit);

    QubitList* qubit = this->circuit[gate.targetQubit];

    // Update start if first node
    if(qubit->head == NULL){
        // cout << "updated head" << endl;
        qubit->head = newGate;
    }
    
    // Update previous "last" gate if one exists
    if(qubit->tail != NULL){
        // cout << "updated previous tail" << endl;
        qubit->tail->next = newGate;
        newGate->before = qubit->tail;
    }
    
    // Update "tail" gate
    // cout << "updated tail" << endl;
    qubit->tail = newGate;

    if(gate.gateType == 1){
        // Initialize CNOT alert Node. I am the qubit that is the control
        // my lastControl pointer points to my target and my target/control
        // values are flipped
        Gate alertGate;
        alertGate.gateType = 5;
        alertGate.coefficient = 0;
        alertGate.targetQubit = gate.controlQubit;
        alertGate.controlQubit = gate.targetQubit;
        alertGate.lastControl = newGate;
        add(alertGate);

        newGate->lastControl = this->circuit[gate.controlQubit]->tail; 
    }
    // cout << qubit->head->gateType << endl;
    // print();
    return;
}

void CircuitList::addQubits(int target, int control){    

    while((control > this->maxQubit) or (target > this->maxQubit)){
        // cout << "Adding qubit: " << this->maxQubit+1 << endl;
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
    // cout << "entered print" << endl;
    // This vector contains pointers to the next gate that needs to be
    // printed for each qubit
    vector<Gate *> currents;
    for(int i=0; i<=this->maxQubit; i++){
        if(this->circuit[i]->head == NULL){
            // cout << "head for qubit: " << i << " is empty" << endl;
        }
        currents.push_back(this->circuit[i]->head);
    }

    // print list for each qubitList to make sure all gates get printed
    for(int i=0; i<=this->maxQubit; i++){
        // front of current qubitList
        Gate* current = currents[i];

        // while there are still more gates in this qubitList...
        while(current != NULL){
 
            if((current->gateType == 5) or (current->gateType == 1)){
                // CNOT gate, so print all the other gates that need to
                // come first
                printBeforeThisCNOT(current, currents);
            }else{
                // print this gate
                cout << GATETYPE[current->gateType] << " ";

                if(current->coefficient != 0){
                    cout << current->coefficient << " ";
                }

                if(current->controlQubit != -1){
                    cout << current->controlQubit << " ";
                }

                cout << current->targetQubit << "\n";
            }

            // go to next
            current=current->next;
        }
    }
    return;
}

void CircuitList::printBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents){
    Gate* current = currents[CNOT->controlQubit];

    // print every gate that needs to come before
    while(current != CNOT->lastControl){

        if((current->gateType == 5) or (current->gateType == 1)){
            // CNOT gate, so print all the other gates that need to
            // come first
            printBeforeThisCNOT(current, currents);
        }else{
            // print this gate
            cout << GATETYPE[current->gateType] << " ";

            if(current->coefficient != 0){
                cout << current->coefficient << " ";
            }

            if(current->controlQubit != -1){
                cout << current->controlQubit << " ";
            }

            cout << current->targetQubit << "\n";
        }

        // go to next
        current=current->next;

    }    

    // last thing to do is print out this CNOT gate then update currents
    cout << "CNOT ";
    if(current->gateType == 1){
        cout << current->controlQubit << " " << current->targetQubit << "\n";
    }else if(current->gateType == 5){
        cout << current->targetQubit << " " << current->controlQubit << "\n";
    }
    // update other list's next gate
    currents[CNOT->controlQubit] = current->next;
}

void CircuitList::saveQASM(){
    ofstream qasm ("outputQASM.txt");

    vector<Gate *> currents;
    for(int i=0; i<=this->maxQubit; i++){
        currents.push_back(this->circuit[i]->head);
    }

    if(qasm.is_open()){
        // print list for each qubitList to make sure all gates get printed
        for(int i=0; i<=this->maxQubit; i++){
            // front of current qubitList
            Gate* current = currents[i];

            // while there are still more gates in this qubitList...
            while(current != NULL){
     
                if((current->gateType == 5) or (current->gateType == 1)){
                    // CNOT gate, so print all the other gates that need to
                    // come first
                    saveBeforeThisCNOT(current, currents, qasm);
                }else{
                    // print this gate
                    qasm << GATETYPE[current->gateType] << " ";

                    if(current->coefficient != 0){
                        qasm << current->coefficient << " ";
                    }

                    if(current->controlQubit != -1){
                        qasm << current->controlQubit << " ";
                    }

                    qasm << current->targetQubit << "\n";
                }

                // go to next
                current=current->next;
            }
        }
    }else{
        cout << "Couldn't open file\n";
    }
    return;
}

void CircuitList::saveBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents,
                                     ofstream qasm){
    Gate* current = currents[CNOT->controlQubit];

    if(qasm.is_open()){
        // print every gate that needs to come before
        while(current != CNOT->lastControl){

            if((current->gateType == 5) or (current->gateType == 1)){
                // CNOT gate, so print all the other gates that need to
                // come first
                printBeforeThisCNOT(current, currents);
            }else{
                // print this gate
                qasm << GATETYPE[current->gateType] << " ";

                if(current->coefficient != 0){
                    qasm << current->coefficient << " ";
                }

                if(current->controlQubit != -1){
                    qasm << current->controlQubit << " ";
                }

                qasm << current->targetQubit << "\n";
            }

            // go to next
            current=current->next;
        }    

        // last thing to do is print out this CNOT gate then update currents
        qasm << "CNOT ";
        if(current->gateType == 1){
            qasm << current->controlQubit << " " << current->targetQubit << "\n";
        }else if(current->gateType == 5){
            qasm << current->targetQubit << " " << current->controlQubit << "\n";
        }
        // update other list's next gate
        currents[CNOT->controlQubit] = current->next;
    }else{
        cout << "Couldn't open file again\n";
    }
}