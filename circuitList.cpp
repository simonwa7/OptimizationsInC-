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

void CircuitList::addAndOptimize(Gate gate){
    cerr << "Attempting to add " << GATETYPE[gate.gateType] << gate.coefficient << " " << gate.controlQubit << " " << gate.targetQubit << "\n";
    addQubits(gate.targetQubit, gate.controlQubit);
    Gate *current = this->circuit[gate.targetQubit]->tail;
    bool cancelled = false;

    while(current != NULL){
        if(checkIfGatesCancel(&gate, current)){
            removeNext(gate, current);
            cancelled = true;
            break;
        }else if(checkIfGatesCommute(&gate, current)){
            cerr << "Gates commuting\n";
            current = current->before;
        }else{
            cerr << "Gates not commuting\n";
            break;
        }
    }
    
    if(!cancelled){
        add(gate);
    }
}

bool CircuitList::checkIfGatesCommute(Gate* gate1, Gate* gate2){
    /* Purpose: Check if two gates commute with one another based on defined 
                commutation rules:
        Input Parameter: Two Gate Objects
        Return: boolean (true if gates commute, false otherwise)
        Time Complexity: O(c)
    */
    cerr << "Does " << GATETYPE[gate1->gateType] << gate1->coefficient << " " << gate1->controlQubit << " " << gate1->targetQubit << "\n";
    cerr << "Commute with " << GATETYPE[gate2->gateType] << gate2->coefficient << " " << gate2->controlQubit << " " << gate2->targetQubit << "\n";    
    // If they aren't CNOT gates, then since they must be acting on the same qubit, they cannot commute
    if((gate1->gateType != 1) && (gate2->gateType != 1) && (gate2->gateType != 5) && (gate2->gateType != 5)){
        cerr << "false\n";
        return false;
    }else if((gate1->gateType == 1) && (gate2->gateType == 3)){
        // With a CNOT and an Rz, they'll commute if the Rz doesn't act on the control qubit for the CNOT
        cerr << (gate1->controlQubit != gate2->targetQubit) << endl;
        return (gate1->controlQubit != gate2->targetQubit);
    }else if((gate1->gateType == 5) && (gate2->gateType == 3)){
        // CNOT control qubit is same as Rz (gateType5 has switched contol and target)
        cerr << "false\n";
        return false;
    }else if((gate1->gateType == 3) && (gate2->gateType == 1)){
        // same as before (Rz cannot act on control qubit)
        cerr << !(gate1->targetQubit != gate2->controlQubit) << endl;
        return !(gate1->targetQubit != gate2->controlQubit);
    }else if((gate1->gateType == 3) && (gate2->gateType == 5)){
        // same as before (CNOT and Rz - again flippd qubits for gatetype 5)
        cerr << "false\n";
        return false;
    }else if((gate1->gateType == 1) && (gate2->gateType == 5)){
        // gates don't commute if both CNOT and have swapped control/targets
        cerr << "false\n";
        return false;
    }else if((gate1->gateType == 5) && (gate2->gateType == 1)){
        cerr << "false\n";
        return false;
    }
    // so here, at least one is a CNOT and the other is an Rx or H. In that case, 
    // if they share any qubits, we should return false... which they must since
    // they are on the same qubitList!
    cerr << "false\n";
    return false;
}


bool CircuitList::checkIfGatesCancel(Gate* gate1, Gate* gate2){
    /* Purpose: Check if two gates can cancel with eachother
        Input Parameter: Two Gate Objects
        Return: boolean (true if gates can cancel, false otherwise)
        Time Complexity: O(c)
    */
    if((gate1->gateType == gate2->gateType) && 
       (gate1->controlQubit == gate2->controlQubit)){
        return true;
    }
    return false;
}

void CircuitList::removeNext(Gate gate, Gate* nextGate){
    /* Purpose: Cancel and remove gates based on the two inputs
        Input Parameter: Pointers to Two Node Objects
        Return: void
        Time Complexity: O(c)
    */
    cerr << "Cancelling " << GATETYPE[gate.gateType] << gate.coefficient << " " << gate.controlQubit << " " << gate.targetQubit << "\n";
    cerr << "With " << GATETYPE[nextGate->gateType] << nextGate->coefficient << " " << nextGate->controlQubit << " " << nextGate->targetQubit << "\n";
    // Combine gates if gates are of type Rx or Rz
    if((nextGate->gateType == 2) || (nextGate->gateType == 3)){
        nextGate->coefficient += gate.coefficient;
        // --this->optimizedLength;
        
        // keep rotations within 2pi
        if(nextGate->coefficient >= 6.2831853071795864){
            nextGate->coefficient -= 6.2831853071795864;
        }
        if(nextGate->coefficient <= -6.2831853071795864){
            nextGate->coefficient += 6.2831853071795864;
        }

        // Remove gates if combining gates results in a gate with 0 coefficient
        if(nextGate->coefficient == 0){
            // --this->optimizedLength;
            removeGate(nextGate);
        }
    }else{
        if((gate.gateType == 1) || (gate.gateType == 5)){
            // If a CNOT gate cancellation, make sure to remove the other
            // instance of that gate
            removeGate(nextGate->lastControl);
            // this->optimizedNumCNOT -= 2;
        }
        // this->optimizedLength -= 2;
        removeGate(nextGate);
    }
    return;
}

void CircuitList::removeGate(Gate* gate){
    /* Purpose: Remove input gate from the list
        Input Parameter: Pointer to a Node Object
        Return: void
        Time Complexity: O(c)
    */
    // cerr << "Before nothing\n";
    QubitList* qubit = this->circuit[gate->targetQubit];
    if(qubit->head == NULL || gate == NULL){
        return;
    }
    
    // cerr << "Before start\n";
    // Update start to next gate if gate to remove is the start
    if(qubit->head == gate){
        // cerr << "Was start\n";
        qubit->head = gate->next;
    }
    
    // cerr << "Before last\n";
    // Update last to previous gate if gate to remove is the last
    if(qubit->tail == gate){
        // cerr << "Was last\n";
        qubit->tail = gate->before;
    }
    
    // cerr << "Before reset next\n";
    // Update next gate's before pointer
    if(gate->next != NULL){
        gate->next->before = gate->before;
    }
    
    // cerr << "Before reset last\n";
    // Update before gate's next pointer
    if(gate->before != NULL){
        gate->before->next = gate->next;
    }
    
    // cerr << "Before memory release\n";
    // Free memory if gate exists
    if(gate != NULL){
        delete gate;
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

void CircuitList::saveBeforeThisCNOT(Gate* CNOT, vector<Gate*> &currents, ofstream &qasm){
    Gate* current = currents[CNOT->controlQubit];

    if(qasm.is_open()){
        // print every gate that needs to come before
        while(current != CNOT->lastControl){

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