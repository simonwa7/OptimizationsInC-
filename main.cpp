#include <iostream>
#include "circuitList.h"
#include <fstream>

using namespace std;

Gate lineToGate(string line);
double getCoefficient(string line, int &i);

int main(){
	CircuitList* circuit = new CircuitList;

	string line;
	ifstream qasm ("qasm.txt");

	if(qasm.is_open()){
		while(getline(qasm, line)){
			cout << line << endl;
			Gate gate = lineToGate(line);
			circuit.add(gate);
		}
	}

	circuit.print();

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

    string gateTypeString = ""; 
    
    int i = 0; 
    while(line[i] != ' '){ 
        gateTypeString += line[i]; 
        ++i; 
    } 
    ++i; 

    if(gateTypeString == "H"){ 
        obj.gateType = 0; 
        obj.targetQubit = line[i]-48;
    }else if(gateTypeString == "CNOT"){ 
        obj.gateType = 1; 
        obj.controlQubit = line[i]-48;
        obj.targetQubit = line[i+2]-48;
    }else{ 
        if(gateTypeString == "Rx"){ 
            obj.gateType = 2; 
        }else if(gateTypeString == "Rz"){ 
            obj.gateType = 3; 
        }else{ 
            cout << "Trouble identifying gate type" << '\n'; 
        } 

        obj.coefficient = getCoefficient(line, i);
        obj.targetQubit = line[i+1]-48; 
    } 
    return obj; 
}

double getCoefficient(string line, int &i){
    string untypedCoefficient = "";
    
    while(line[i] != ' '){
        untypedCoefficient += line[i];
        i++;
    }
    
    const char *cstr = untypedCoefficient.c_str();
    return atof(cstr);
}