// #include <iostream>
#include "circuitList.h"
#include <fstream>

using namespace std;

Gate lineToGate(string line);
double getCoefficient(string line, unsigned int &i);

int main(){
	CircuitList* circuit = new CircuitList;

	string line;
	ifstream qasm ("qasm.txt");

	if(qasm.is_open()){
		while(getline(qasm, line)){
			// cout << line << endl;
			Gate gate = lineToGate(line);
			circuit->addAndOptimize(gate);
		}
	}

	circuit->saveQASM();

	delete circuit;

	return 0;
}

Gate lineToGate(string line){ 
    Gate obj; 
    obj.gateType = 4;
    obj.targetQubit = -1;
    obj.controlQubit = -1; 
    obj.coefficient = 0; 
    obj.lastControl = NULL;
    obj.next = NULL;
    obj.before = NULL;

    string gateTypeString = ""; 
    
    unsigned int i = 0; 
    while(line[i] != ' '){ 
        gateTypeString += line[i]; 
        ++i; 
    } 
    ++i; 

    if(gateTypeString == "H"){ 
        obj.gateType = 0; 
        string qubit = "";
        while(i < line.length()){
            qubit += line[i];
            ++i;
        }
        obj.targetQubit = stoi(qubit);
    }else if(gateTypeString == "CNOT"){ 
        obj.gateType = 1; 
        string qubit = "";
        while(line[i] != ' '){
            qubit += line[i];
            ++i;
        }
        obj.controlQubit = stoi(qubit);
        qubit = "";
        while(i < line.length()){
            qubit += line[i];
            ++i;
        }
        obj.targetQubit = stoi(qubit);
    }else{ 
        if(gateTypeString == "Rx"){ 
            obj.gateType = 2; 
        }else if(gateTypeString == "Rz"){ 
            obj.gateType = 3; 
        }else{ 
            cout << "Trouble identifying gate type" << '\n'; 
        } 

        obj.coefficient = getCoefficient(line, i);
        string qubit = "";
        while(i < line.length()){
            qubit += line[i];
            ++i;
        }
        obj.targetQubit = stoi(qubit);
    } 
    return obj; 
}

double getCoefficient(string line, unsigned int &i){
    string untypedCoefficient = "";
    
    while(line[i] != ' '){
        untypedCoefficient += line[i];
        i++;
    }
    
    const char *cstr = untypedCoefficient.c_str();
    return atof(cstr);
}