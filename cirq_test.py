import cirq
import sys
from cirq import Simulator
import time

import numpy

def parse_to_whitespace(qasm_string, i):
    string = ''
    while((i < len(qasm_string)) and (qasm_string[i] != ' ')):
        string += qasm_string[i]
        i += 1
    return string.rstrip(), i+1


def get_gate(qasm_string):
    gate = None

    gateType, i = parse_to_whitespace(qasm_string, 0)
    if((gateType == 'Rx') or (gateType == 'Ry') or (gateType == 'Rz')):
        rotation, i = parse_to_whitespace(qasm_string, i)
        rotation = float(rotation)

        qubit, i = parse_to_whitespace(qasm_string, i)
        qubit = int(qubit)
        if(gateType == 'Rx'):
            gate = cirq.Rx(rotation)(qubit)
        elif(gateType == 'Ry'):
            gate = cirq.Ry(rotation)(qubit)
        else:
            gate = cirq.Rz(rotation)(qubit)
        # print(gateType, rotation, qubit)
    elif((gateType == 'H')):
        qubit, i = parse_to_whitespace(qasm_string, i)
        qubit = int(qubit)
        gate = cirq.H(qubit)
        # print(gateType, qubit)
    elif((gateType == 'CNOT')):
        control, i = parse_to_whitespace(qasm_string, i)
        target, i = parse_to_whitespace(qasm_string, i)
        control = int(control)
        target = int(target)
        gate = cirq.CNOT(control, target)
        # print(gateType, control, target)

    return gate


def simulate_qasm(qasm_circuit):
    circuit = cirq.Circuit()
    simulator = Simulator()

    qasm_circuit = open(qasm_circuit, "r")
    for qasm_gate in qasm_circuit:
        circuit.append(get_gate(qasm_gate))
    qasm_circuit.close()

    return(simulator.simulate(circuit).final_state)


def check_difference(circuit1, circuit2):
    start = time.time()
    result1 = simulate_qasm(circuit1)
    time1 = time.time()-start
    start = time.time()
    result2 = simulate_qasm(circuit2)
    time2 = time.time()-start
    file = open("data.txt", "a")
    file.write("{} {}\n".format(time1, time2))
    file.close()
    total_diff = 0
    for result in zip(result1, result2):
        total_diff += numpy.linalg.norm(result[0]-result[1])
    # print("\n------ TEST RESULTS -------\n")
    difference = numpy.linalg.norm(result1-result2)
    if((difference < 10**-2) or (total_diff < 10**-1)):
        print(True)
    else:
        print total_diff
        print difference
        print result1
        print result2
        print(False)
    # print("\n------ END OF RESULTS -----\n")
    #     print(result1)
    #     print(result2)
    #     print(difference)
    # else:
    #     print(False)
    #     print(result1)
    #     print(result2)
    #     print(difference)



if __name__ == '__main__':
    check_difference(sys.argv[1], sys.argv[2])