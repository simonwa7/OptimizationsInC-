#include "circuitList.h"

CircuitList::CircuitList(){
    // Basic constructor for CircuitList class
    this->length = 0;
    this->numCNOT = 0;
    this->optimizedLength = 0;
    this->optimizedNumCNOT = 0;
    this->maxQubit = -1;
    
    return;
}

CircuitList::~CircuitList(){
    // Destructor for CircuitList. Deletes all nodes still left in the circuit

    // loop through each qubit
    for(int i=this->maxQubit; i >= 0; i--){
        // get the head of the list
        Gate* current = this->circuit[i]->head;
        
        // continue until current reaches the end
        while(current != NULL){
            // update current before deleting node
            Gate* previous = current;
            current = current->next;
            delete previous;
        }

        // delete pointer to qubitList
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
    ++this->length;
    ++this->optimizedLength;

    // Initialize new Node
    Gate* newGate = new Gate;
    newGate->gateType = gate.gateType;
    newGate->coefficient = gate.coefficient;
    newGate->targetQubit = gate.targetQubit;
    newGate->controlQubit = gate.controlQubit;
    newGate->lastControl = gate.lastControl;
    newGate->next = NULL;
    newGate->before = NULL;

    // add more qubits to circuit if necessary
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
        ++this->numCNOT;
        ++this->optimizedNumCNOT;
        --this->length;
        --this->optimizedLength;
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
    /*
    Purpose: add qubits to circuit if current number in circuit is smaller than
            either the target or control qubits
    ARGS: 
        target: int
        control: int
    RETURNS:
        NULL
    TIME:
        O(c) - limit on number of qubits should realistically keep this as O(c)
    */
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
    /*
    Purpose: add a gate to the end of the circuit, but first check if it cancels
            with a previous gate. If cancellation occurs, adding the gate is
            unnecessary and the corresponding gate is instead deleted. 
    ARGS: 
        Gate object
    RETURNS:
        NULL
    TIME:
        O(n) 
    */
    addQubits(gate.targetQubit, gate.controlQubit);
    
    // If not a CNOT gate
    if(gate.gateType != 1){

        // try to find previous gate to cancel with (NULL if no such gate)
        Gate* gateToCancel = checkPreviousGates(gate);

        if(gateToCancel == NULL){
            add(gate);
        }else{
            removeNext(gate, gateToCancel);
            // keep track of the unoptimized length
            ++this->length;
        }
    }else{
        // gate is a CNOT gate, so much check both CNOT target and CNOT control
        // check CNOT target gate
        Gate* gateToCancel1 = checkPreviousGates(gate);

        // create and check CNOT control gate
        Gate alertGate;
        alertGate.gateType = 5;
        alertGate.coefficient = 0;
        alertGate.targetQubit = gate.controlQubit;
        alertGate.controlQubit = gate.targetQubit;
        alertGate.lastControl = NULL;
        Gate* gateToCancel2 = checkPreviousGates(alertGate);

        // if both checks returned gates to control, we know they will point
        // to the same gate, so we only need to cancel one
        if((gateToCancel1 != NULL) and (gateToCancel2 != NULL)){
            removeNext(gate, gateToCancel1);
            // keep track of optimized gate counts
            ++this->length;
            ++this->numCNOT;
        }else{
            add(gate);
        }
    }
}

Gate* CircuitList::checkPreviousGates(Gate gate){
    /*
    Purpose: iterate through the previous gates in the qubit's list of gates and
            try to find a gate that will reduce to the identity. If the current
            iteration is unsuccessful, proceed to the next gate as long as 
            the gates pass the commutation rules.
    ARGS: 
        Gate object
    RETURNS:
        pointer to a gate to be cancelled (NULL if no such gate exists)
    TIME:
        O(n) 
    */
    // Get the pointer to the end of the qubit's list of gates
    Gate* current = this->circuit[gate.targetQubit]->tail;

    // iterate from tail to head 
    while(current != NULL){
        if(checkIfGatesCancel(&gate, current)){
            // if gates cancel, return pointer to matching gate
            return current;
        }else if(checkIfGatesCommute(&gate, current)){
            // if the gates do not cancel, but commute, proceed to previous gate
            current = current->before;
        }else{
            // gates neither cancel nor commute, so we must return NULL
            return NULL;
        }
    }
    // no gate cancellation was found
    return NULL;
}
/*

Commutation Rules:
  - Hadamard with Hadamard -> should cancel, don't need to recheck
  - Hadamard with CNOT (1) -> Andrew says NO
  - Hadamard with CNOT (5) -> Andrew says NO
  x Hadamard with Rx -> Andrew says probably not
  - Hadamard with Rz -> Andrew says probably not 
    Observation: Hadamards won't commute with anything under current rules

    "CNOTs commute unless one's target is other's control"
    CNOT with Rz -> Commute iff Rz target != CNOT target
    
  - CNOT(1) with Hadamard -> Andrew says NO
  - CNOT(1) with CNOT(1) -> should cancel, don't need to recheck
  - CNOT(1) with CNOT(5) -> Andrew says NO (current qubit is CNOT(1) target and CNOT(5) control)
  + CNOT(1) with Rx -> Andrew says this should commute if a simple X rotation but labels it as false
  - CNOT(1) with Rz -> Andrew says NO

  - CNOT(5) with Hadamard -> Andrew says NO
  - CNOT(5) with CNOT(1) -> Andrew says NO (current qubit is CNOT(5) control and CNOT(1) target)
  - CNOT(5) with CNOT(5) -> should cancel, don't need to recheck
  - CNOT(5) with Rx -> Andrew says this should commute if a simple X rotation but labels it as false
 +* CNOT(5) with Rz -> Rz target == CNOT control, hence Rz target != CNOT target --> COMMUTES

  x Rx with Hadamard -> Andrew says probably not
  + Rx with CNOT(1) -> Andrew says this should commute if a simple X rotation but labels it as false
  - Rx with CNOT(5) -> Andrew says this should commute if a simple X rotation but labels it as false
  - Rx with Rx -> should cancel, don't need to recheck
  - Rx with Rz -> Andrew says probably not

  - Rz with Hadamard -> Andrew says probably not
  - Rz with CNOT(1) -> Andrew says NO
  * Rz with CNOT(5) -> Rz target == CNOT control, hence Rz target != CNOT target --> COMMUTES
  - Rz with Rx -> Andrew says probably not
  - Rz with Rz -> should cancel, don't need to recheck

  * COMMUTES UNDER CURRENT RULES
  - EXPERIMENTALLY NO
  + EXPERIMENTALLY YES
 ** Check this experimentally
  x EXPERIMENTALLY MAYBE
*/

bool CircuitList::checkIfGatesCommute(Gate* gate1, Gate* gate2){
    /*
    Purpose: A simple check to see if two gates commute based on 
            predecided/precalculated commutation rules. The ruleset is defined
            above and has been tested experimentally with the cirq simulator. 
    ARGS: 
        gate1: (pointer to a gate object) First gate to be compared
        gate2: (pointer to a gate object) Second gate to be compared
    RETURNS:
        boolean: True if gates commute, false otherwise
    TIME:
        O(c) 
    */

    // Experimentally YES to CNOT(5) with Rz
    if((gate1->gateType == 5) and (gate2->gateType == 3)){
        return true;
    }
    if((gate1->gateType == 3) and (gate2->gateType == 5)){
        return true;
    }

    // Experimentally YES to CNOT(1) with Rx
    if((gate1->gateType == 1) and (gate2->gateType == 2)){
        return true;
    }
    if((gate1->gateType == 2) and (gate2->gateType == 1)){
        return true;
    }

    // Experimentally MAYBE to H and Rx... Did not add any cancellations
    if((gate1->gateType == 0) and (gate2->gateType == 2)){
        return true;
    }
    if((gate1->gateType == 2) and (gate2->gateType == 0)){
        return true;
    }

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
    // Combine gates if gates are of type Rx or Rz
    if((nextGate->gateType == 2) || (nextGate->gateType == 3)){
        nextGate->coefficient += gate.coefficient;
        
        // keep rotations within 2pi
        if(nextGate->coefficient >= 6.2831853071795864){
            nextGate->coefficient -= 6.2831853071795864;
        }
        if(nextGate->coefficient <= -6.2831853071795864){
            nextGate->coefficient += 6.2831853071795864;
        }

        // Remove gates if combining gates results in a gate with 0 coefficient
        if(nextGate->coefficient == 0){
            --this->optimizedLength;
            removeGate(nextGate);
        }
    }else{
        if((gate.gateType == 1) || (gate.gateType == 5)){
            // If a CNOT gate cancellation, make sure to remove the other
            // instance of that gate
            removeGate(nextGate->lastControl);
            --this->optimizedNumCNOT;
        }
        --this->optimizedLength;
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
    QubitList* qubit = this->circuit[gate->targetQubit];
    if(qubit->head == NULL || gate == NULL){
        return;
    }
    
    // Update start to next gate if gate to remove is the start
    if(qubit->head == gate){
        qubit->head = gate->next;
    }
    
    // Update last to previous gate if gate to remove is the last
    if(qubit->tail == gate){
        qubit->tail = gate->before;
    }
    
    // Update next gate's before pointer
    if(gate->next != NULL){
        gate->next->before = gate->before;
    }
    
    // Update before gate's next pointer
    if(gate->before != NULL){
        gate->before->next = gate->next;
    }
    
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

void CircuitList::saveQASM(string outputName){
    ofstream qasm (outputName);
    qasm.precision(15);
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
        // cerr << "COULDNT OPEN QASM IN saveBeforeThisCNOT\n";
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

void CircuitList::printLengths(){
    cout << "\n ------- Gate Counts -------\n";
    cout << "Unoptimized Length: " << this->length << endl;
    cout << "Unoptimized Number of CNOT Gates: " << this->numCNOT << endl;
    cout << "Optimized Length: " << this->optimizedLength << endl;
    cout << "Optimized Number of CNOT Gates: " << this->optimizedNumCNOT << endl;
}

unsigned long long CircuitList::getLength(){
    return this->length;
}
unsigned long long CircuitList::getNumCNOT(){
    return this->numCNOT;
}
unsigned long long CircuitList::getOptimizedLength(){
    return this->optimizedLength;
}
unsigned long long CircuitList::getOptimizedNumCNOT(){
    return this->optimizedNumCNOT;
}