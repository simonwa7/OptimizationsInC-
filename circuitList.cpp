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
    for(unsigned int i=this->maxQubit; i >= 0; i--){
        Gate* current = circuit[i]->head;
        
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
*/
    // Initialize new Node
    Gate* newGate = new Gate;
    newGate->gateType = gate.gateType;
    newGate->coefficient = gate.coefficient;
    // newGate->controlQubit = gate.controlQubit;
    // newGate->targetQubit = gate.targetQubit;
    newGate->target = NULL;
    newGate->next = NULL;
    newGate->before = NULL;

    while((gate.controlQubit > this->maxQubit) or (gate.targetQubit > this->maxQubit)){
        QubitList* newQubit = new QubitList;
        newQubit->head = NULL;
        newQubit->tail = NULL;
        circuit.push_back(newQubit);
        this->maxQubit++;
    }
    
    QubitList start = circuit[gate.targetQubit];
    // // Update start if first node
    // if(this->start == NULL){
    //     this->start = newGate;
    // }
    
    // // Update previous "last" gate if one exists
    // if(this->last != NULL){
    //     this->last->next = newGate;
    //     newGate->before = this->last;
    // }
    
    // // Update "last" gate
    // this->last = newGate;
    
    return;
}




void CircuitList::addAndOptimize(Gate gate){
/* Purpose: Add a gate at the end of the circuit after attempting to cancel
            that gate from the circuit first
    Input Parameter: Gate object
    Return: void
    Time Complexity: O(g)
*/
    ++this->length;
    ++this->optimizedLength;
    if(gate.gateType == 1){
        ++this->numCNOT;
        ++this->optimizedNumCNOT;
    }
    if(!cancelNext(gate)){
        add(gate);
    }
}

bool CircuitList::cancelNext(Gate gate){
/* Purpose: Attempt to cancel the last gate in the circuit by checking
            cancellations and commutations with gates before it
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* nextGate = this->last;
    bool cancelled = false;
    
    while(nextGate != NULL){ // Loop from currentGate to last
        if(checkIfGatesCancel(&gate, nextGate)){
            removeNext(gate, nextGate);
            cancelled = true;
            break;
        }else if(!checkIfGatesCommute(&gate, nextGate)){
            break;
        }else{
            nextGate = nextGate->before;
        }
    }
    
    return cancelled;
}

void CircuitList::removeNext(Gate gate, Gate* nextGate){
/* Purpose: Cancel and remove gates based on the two inputs
    Input Parameter: Pointers to Two Node Objects
    Return: void
    Time Complexity: O(c)
*/
    // cerr << "Cancelling " << GATETYPE[currentGate->gateType] << currentGate->coefficient << " " << currentGate->controlQubit << " " << currentGate->targetQubit << "\n";
    // cerr << "With " << GATETYPE[nextGate->gateType] << nextGate->coefficient << " " << nextGate->controlQubit << " " << nextGate->targetQubit << "\n";
    // Combine gates if gates are of type Rx or Rz
    if((nextGate->gateType == 2) || (nextGate->gateType == 3)){
        nextGate->coefficient += gate.coefficient;
        --this->optimizedLength;
        
        if(nextGate->coefficient >= 62831853071795864){
            nextGate->coefficient -= 62831853071795864;
        }
        if(nextGate->coefficient <= -62831853071795864){
            nextGate->coefficient += 62831853071795864;
        }
        // Remove gates if combining gates results in a gate with 0 coefficient
        if(nextGate->coefficient == 0){
            --this->optimizedLength;
            removeGate(nextGate);
        }
    }else{
        if(gate.gateType == 1){
            this->optimizedNumCNOT -= 2;
        }
        this->optimizedLength -= 2;
        removeGate(nextGate);
    }
    return;
}




void CircuitList::optimize(){
/* Purpose: To perform all possible gate cancellations in the circuit
    Input Parameter: void
    Return: void
    Time Complexity: Worst Case: O(g^3), Probably: O(g^2)
*/
    unsigned long long old_len = this->length;
    cancelDuplicates();
    unsigned long long new_len = this->length;
    
    // Perform cancellations while the circuit length continues to improve
    while(new_len != old_len){
        old_len = new_len;
        cancelDuplicates();
        new_len = this->length;
    }
    
    return;
}

void CircuitList::cancelDuplicates(){
/* Purpose: Loop through the circuit from start to finish, iterating along the
            remaining gates in the circuit while the gates commute looking for
            possible gate cancellations.
    Input Parameter: void
    Return: void
    Time Complexity: O(g^2)
*/
    Gate* currentGate = this->start;
    
    while(currentGate != NULL){    // Loop from start to last
        Gate* nextGate = currentGate->next;
        // To hold in case the currentGate gets removed
        Gate* inCaseOfCancelGate = currentGate->before;
        bool cancelled = false;
        
        while(nextGate != NULL){ // Loop from currentGate to last
            if(checkIfGatesCancel(currentGate, nextGate)){
                cancelGates(currentGate, nextGate);
                cancelled = true;
                break;
            }else if(!checkIfGatesCommute(currentGate, nextGate)){
                break;
            }else{
                nextGate = nextGate->next;
            }
        }
        
        if(!cancelled){ // currentGate didn't cancel, so move to next
            currentGate = currentGate->next;
        }else{ // currentGate was cancelled, so must reassign it
            currentGate = inCaseOfCancelGate;
        }
    }
    return;
}

void CircuitList::cancelGates(Gate* currentGate, Gate* nextGate){
/* Purpose: Cancel and remove gates based on the two inputs
    Input Parameter: Pointers to Two Node Objects
    Return: void
    Time Complexity: O(c)
*/
    // cerr << "Cancelling " << GATETYPE[currentGate->gateType] << currentGate->coefficient << " " << currentGate->controlQubit << " " << currentGate->targetQubit << "\n";
    // cerr << "With " << GATETYPE[nextGate->gateType] << nextGate->coefficient << " " << nextGate->controlQubit << " " << nextGate->targetQubit << "\n";
    // Combine gates if gates are of type Rx or Rz
    if((currentGate->gateType == 2) || (currentGate->gateType == 3)){
        currentGate->coefficient += nextGate->coefficient;
        removeGate(nextGate);
        this->length--;
        // Remove gates if combining gates results in a gate with 0 coefficient
        if(currentGate->coefficient == 0){
            removeGate(currentGate);
            this->length--;
        }
    }else{
        if(currentGate->gateType == 1){
            this->numCNOT--;
            this->numCNOT--;
        }
        removeGate(nextGate);
        this->length--;
        removeGate(currentGate);
        this->length--;
    }
    // print();
    // cerr << "\n";
    return;
}




bool CircuitList::checkIfGatesCancel(Gate* gate1, Gate* gate2){
/* Purpose: Check if two gates can cancel with eachother
    Input Parameter: Two Gate Objects
    Return: boolean (true if gates can cancel, false otherwise)
    Time Complexity: O(c)
*/
    if((gate1->gateType == gate2->gateType) && 
       (gate1->controlQubit == gate2->controlQubit) && 
       (gate1->targetQubit == gate2->targetQubit)){
        return true;
    }
    return false;
}

bool CircuitList::checkIfGatesCommute(Gate* gate1, Gate* gate2){
/* Purpose: Check if two gates commute with one another based on defined 
            commutation rules:
    Input Parameter: Two Gate Objects
    Return: boolean (true if gates commute, false otherwise)
    Time Complexity: O(c)
*/
    if((gate1->gateType == 2) && (gate2->gateType == 2)){
        return true;
    }else if((gate1->gateType == 3) && (gate2->gateType == 3)){
        return true;
    }else if((gate1->gateType == 0) && (gate2->gateType == 0)){
        return true;
    }else if((gate1->gateType != 1) && (gate2->gateType != 1)){
        if(gate1->controlQubit != gate2->controlQubit){
            return true;
        }else{
            return false;
        }
    }else if((gate1->gateType == 1) && (gate2->gateType == 3)){
        if(gate1->targetQubit != gate2->controlQubit){
            return true;
        }else{
            return false;
        }
    }else if((gate1->gateType == 3) && (gate2->gateType == 1)){
        if(gate1->controlQubit != gate2->targetQubit){
            return true;
        }else{
            return false;
        }
    }else{
        return(!ifDuplicateQubits(gate1->controlQubit, gate1->targetQubit, gate2->controlQubit, gate2->targetQubit));
    }
    return false;
}

bool CircuitList::ifDuplicateQubits(int c1, int t1, int c2, int t2){
/* Purpose: Check if four input qubits repeat in ways that do not allow two CNOT
            gates to commute
    Input Parameter: Four integers, in order: first gate control qubit, 
                     first gate target qubit, second gate control qubit,
                     second gate target qubit
    Return: boolean (true if gates commute, false otherwise)
    Time Complexity: O(c)
*/
    if(c1 == c2){
        return true;
    }else if(c1 == t2){
        return true;
    }else if(c2 == t1){
        return true;
    }else if(t1 == -1){
        return false;
    }else if(t2 == -1){
        return false;
    }else if(t1 == t2){
        return true;
    }else{
        return false;
    }
}

void CircuitList::removeGate(Gate* gate){
/* Purpose: Remove input gate from the list
    Input Parameter: Pointer to a Node Object
    Return: void
    Time Complexity: O(c)
*/
    // cerr << "Before nothing\n";
    if(this->start == NULL || gate == NULL){
        return;
    }
    
    // cerr << "Before start\n";
    // Update start to next gate if gate to remove is the start
    if(this->start == gate){
        // cerr << "Was start\n";
        this->start = gate->next;
    }
    
    // cerr << "Before last\n";
    // Update last to previous gate if gate to remove is the last
    if(this->last == gate){
        // cerr << "Was last\n";
        this->last = gate->before;
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





void CircuitList::recount(){
/* Purpose: Loop through the circuit from start to finish (for testing)
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* current = this->start;
    this->optimizedLength = 0;
    this->optimizedNumCNOT = 0;
    
    while(current != NULL){
        ++this->optimizedLength;
        if(current->gateType == 1){
            ++this->optimizedNumCNOT;
        }
        current = current->next;
    }
    
    return;
}

void CircuitList::clear(){
/* Purpose: Clear all gates from the current circuit
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* current = this->start;
    
    while(current != NULL){
        Gate* previous = current;
        current = current->next;
        delete previous;
    }
    
    this->length = 0;
    this->numCNOT = 0;
    this->optimizedLength = 0;
    this->optimizedNumCNOT = 0;
    this->start = NULL;
    this->last = NULL;
    
    return;
}

void CircuitList::print(){
/* Purpose: Print the circuit in readable QASM format
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* current = this->start;
    
    while(current != NULL){
        std::cout << GATETYPE[current->gateType] << ' ';
        
        if(current->coefficient != 0){
            std::cout << current->coefficient << ' ';
        }
        
        std::cout << current->controlQubit << ' ';
        
        if(current->targetQubit != -1){
            std::cout << current->targetQubit;
        }
        
        std::cout << "\n";
        
        current = current->next;
    }
    
    return;
}

void CircuitList::printReverse(){
/* Purpose: Print the circuit in reverse in readable QASM format
    Input Parameter: void
    Return: void
*/
    Gate* current = this->last;
    
    while(current != NULL){
        std::cout << GATETYPE[current->gateType] << ' ';
        
        if(current->coefficient != 0){
            std::cout << current->coefficient << ' ';
        }
        
        std::cout << current->controlQubit << ' ';
        
        if(current->targetQubit != -1){
            std::cout << current->targetQubit;
        }
        
        std::cout << "\n";
        
        current = current->before;
    }
    
    return;
}

void CircuitList::printUpTo(unsigned int numGates){
/* Purpose: Print the circuit in readable QASM format up to a certain number of
            gates
    Input Parameter: unsigned int: number of gates to print
    Return: void
    Time Complexity: O(c)
*/
    Gate* current = this->start;
    
    unsigned int i=0;
    while(current != NULL){
        if(i==numGates){
            break;
        }
        std::cout << GATETYPE[current->gateType] << ' ';
        
        if(current->coefficient != 0){
            std::cout << current->coefficient << ' ';
        }
        
        std::cout << current->controlQubit << ' ';
        
        if(current->targetQubit != -1){
            std::cout << current->targetQubit;
        }
        
        std::cout << "\n";
        
        current = current->next;
        ++i;
    }
    
    return;
}

void CircuitList::loop(){
/* Purpose: Loop through the circuit from start to finish (for testing)
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* current = this->start;
    
    while(current != NULL){
        current = current->next;
    }
    
    return;
}

void CircuitList::loopReverse(){
/* Purpose: Loop through the circuit from finish to start (for testing)
    Input Parameter: void
    Return: void
    Time Complexity: O(g)
*/
    Gate* current = this->last;
    
    while(current != NULL){
        current = current->before;
    }
    
    return;
    
}





