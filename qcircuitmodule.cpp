#include <Python.h>
#include <iostream>
#include <string.h>
#include "circuitList.h"
#include <stdlib.h>

using namespace std;

static PyObject *qcircuitError;

Gate lineToGate(string line);
double getCoefficient(string line, int &i);

CircuitList* CIRCUIT = new CircuitList;

static PyObject* qcircuit_addGate(PyObject* self, PyObject *args){
    const char* msg;
    int sts = 1;
    
    if(!PyArg_ParseTuple(args, "s", &msg)){
        return NULL;
    }

    std::string line = msg;
    
    if(line == "error"){
        PyErr_SetString(qcircuitError, "There was an exception raised while adding Gates");
        return NULL;
    }
    
    sts = 0;

    Gate gate = lineToGate(line);
    CIRCUIT -> add(gate);

    return Py_BuildValue("i", sts);
}

static PyObject* qcircuit_addAndOptimizeGate(PyObject* self, PyObject *args){
//     const char* msg;
//     int sts = 1;
    
//     if(!PyArg_ParseTuple(args, "s", &msg)){
//         return NULL;
//     }

//     std::string line = msg;
    
//     if(line == "error"){
//         PyErr_SetString(qcircuitError, "There was an exception raised while adding Gates");
//         return NULL;
//     }
    
//     sts = 0;

//     Gate gate = lineToGate(line);
//     CIRCUIT -> addAndOptimize(gate);

//     return Py_BuildValue("i", sts);
}

static PyObject* qcircuit_show(PyObject* self, PyObject *args){
    const char* msg;
    int sts = 1;
    
    if(!PyArg_ParseTuple(args, "s", &msg)){
        return NULL;
    }
    
    std::string command = msg;
    
    if(command == "circuit"){
        CIRCUIT -> print();
        sts = 0;
//     }else if(command == "Number of gates"){
//         cout << CIRCUIT -> getLength() << "\n";
//         sts = 0;
//     }else if(command == "Number of CNOT gates"){
//         cout << CIRCUIT -> getNumCNOT() << "\n";
//         sts = 0;
    }else{
        string error = "There was an exception raised while trying to use the ";
        error += "'show' fucntion.\n";
        error += "Did not understand value of: " + command;
        const char* char_error = error.c_str();
        PyErr_SetString(qcircuitError, char_error);
        return NULL;
    }

    return Py_BuildValue("i", sts);
}

static PyObject* qcircuit_optimize(PyObject* self, PyObject *args){
//     const char* msg;
//     int sts = 1;
    
//     if(!PyArg_ParseTuple(args, "s", &msg)){
//         return NULL;
//     }
    
//     sts = 0;
    
//     std::string command = msg;
    
//     if(command != "circuit"){
//         string error = "There was an exception raised while trying to use the ";
//         error += "'optimize' fucntion.\n";
//         error += "Did not understand value of: " + command;
//         const char* char_error = error.c_str();
//         PyErr_SetString(qcircuitError, char_error);
//         return NULL;
//     }
    
//     CIRCUIT -> optimize();

//     return Py_BuildValue("i", sts);
}

static PyObject* qcircuit_clear(PyObject* self, PyObject *args){
//     const char* msg;
//     int sts = 1;
    
//     if(!PyArg_ParseTuple(args, "s", &msg)){
//         return NULL;
//     }
    
//     sts = 0;
    
//     std::string command = msg;
    
//     if(command != "circuit"){
//         string error = "There was an exception raised while trying to use the ";
//         error += "'clear' fucntion.\n";
//         error += "Did not understand value of: " + command;
//         const char* char_error = error.c_str();
//         PyErr_SetString(qcircuitError, char_error);
//         return NULL;
//     }
    
//     CIRCUIT -> clear();

//     return Py_BuildValue("i", sts);
}

static PyObject* qcircuit_get(PyObject* self, PyObject *args){
//     const char* msg;
//     unsigned long long value = 0;
    
//     if(!PyArg_ParseTuple(args, "s", &msg)){
//         return NULL;
//     }
    
//     std::string command = msg;
    
//     if(command == "Number of gates"){
//         value = CIRCUIT -> getLength();
//     }else if(command == "Number of CNOT gates"){
//         value = CIRCUIT -> getNumCNOT();
//     }else if(command == "Optimized Number of CNOT gates"){
//         value = CIRCUIT -> getOptimizedNumCNOT();
//     }else if(command == "Optimized Number of gates"){
//         value = CIRCUIT -> getOptimizedLength();
//     }else{
//         string error = "There was an exception raised while trying to use the ";
//         error += "'get' fucntion.\n";
//         error += "Did not understand value of: " + command;
//         const char* char_error = error.c_str();
//         PyErr_SetString(qcircuitError, char_error);
//         return NULL;
//     }

//     //return unsigned py long long
//     return Py_BuildValue("K", value);
}

static PyMethodDef qcircuit_methods[] = {
    {"addGate", qcircuit_addGate, METH_VARARGS, "Add gates to a quantum circuit in C++"},
    {"addAndOptimizeGate", qcircuit_addAndOptimizeGate, METH_VARARGS, "Add gates to a quantum circuit - if they don't cancel - in C++"},
    {"show", qcircuit_show, METH_VARARGS, "Print the current state of the quantum circuit"},
    {"optimize", qcircuit_optimize, METH_VARARGS, "Optimize the current quantum circuit with gate cancellations"},
    {"clear", qcircuit_clear, METH_VARARGS, "Clear the current quantum circuit"},
    {"get", qcircuit_get, METH_VARARGS, "Get gate numbers for current quantum circuit"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initqcircuit(void){
    PyObject *m;
    m = Py_InitModule("qcircuit", qcircuit_methods);
    
    if(m == NULL) return;
    
    qcircuitError = PyErr_NewException("qcircuit.error", NULL, NULL);
    Py_INCREF(qcircuitError);
    
    PyModule_AddObject(m, "error", qcircuitError); 
}
/* NEEEEEED TO MODIFY GATE STRUCT*/
Gate lineToGate(string line){ 
    Gate obj; 
    obj.gateType = 4;
    obj.targetQubit = -1;
    obj.controlQubit = -1; 
    obj.coefficient = 0; 

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