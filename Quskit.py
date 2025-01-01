import os
import math

import time
import math
import qiskit_aer
import numpy as np
from qiskit import QuantumCircuit

qubits = 3

def validate(sv, file_name):
    file_state = np.fromfile(file_name, dtype=np.complex128)
    if (np.abs(file_state[0] - 1) < 1e-9):  # cannot be the initial state
        return False
    return np.all(np.abs(sv - file_state) < 1e-9)

def checker(state, file_name, hw_id):
    print(hw_id)
    print(state)
    if os.path.isfile(file_name):
        if validate(state, file_name):
            print("Validation Success")
        else:
            print("Validation Failed")
    print("--------")

# Task 1: RX Gates (already implemented)
def hw1_2_1_circuit():
    qc = QuantumCircuit(qubits)
    for qubit in range(qubits):
        qc.rx(math.pi / 4, qubit)
    qc.save_statevector()
    simulator = qiskit_aer.AerSimulator(
        method='statevector',
        device='CPU',
        precision='double',
        fusion_enable=False,
        blocking_enable=False,
    )
    job = simulator.run(qc, cuStateVec_enable=False)
    return job.result().data()['statevector'].data

# Task 2: Hadamard Gates
def hw1_2_2_circuit():
    qc = QuantumCircuit(qubits)
    for qubit in range(qubits):
        qc.h(qubit) 
    qc.save_statevector()
    simulator = qiskit_aer.AerSimulator(
        method='statevector',
        device='CPU',
        precision='double',
        fusion_enable=False,
        blocking_enable=False,
    )
    job = simulator.run(qc, cuStateVec_enable=False)
    return job.result().data()['statevector'].data

# Task 3: Hadamard and Controlled Phase Gates
def hw1_2_3_circuit():
    qc = QuantumCircuit(qubits)
    # Apply Hadamard gates
    for qubit in range(qubits):
        qc.h(qubit)
    qc.cp(math.pi / 4, 1, 0) 
    qc.cp(math.pi / 4, 2, 0)  

    qc.save_statevector()
    simulator = qiskit_aer.AerSimulator(
        method='statevector',
        device='CPU',
        precision='double',
        fusion_enable=False,
        blocking_enable=False,
    )
    job = simulator.run(qc, cuStateVec_enable=False)
    return job.result().data()['statevector'].data

# Task 4: Quantum Fourier Transform
def hw1_2_4_circuit():
    qc = QuantumCircuit(qubits)
    for qubit in range(qubits):
        qc.h(qubit) 
        for target in range(qubit + 1, qubits):
            qc.cp(math.pi / (2 ** (target - qubit)), qubit, target) 
    qc.save_statevector()
    simulator = qiskit_aer.AerSimulator(
        method='statevector',
        device='CPU',
        precision='double',
        fusion_enable=False,
        blocking_enable=False,
    )
    job = simulator.run(qc, cuStateVec_enable=False)
    return job.result().data()['statevector'].data

def hw1_2_1():
    state = hw1_2_1_circuit()
    checker(state, "./rx.txt", "[hw1_2_1]")

def hw1_2_2():
    state = hw1_2_2_circuit()
    checker(state, "./h.txt", "[hw1_2_2]")

def hw1_2_3():
    state = hw1_2_3_circuit()
    checker(state, "./cp.txt", "[hw1_2_3]")

def hw1_2_4():
    state = hw1_2_4_circuit()
    checker(state, "./qft.txt", "[hw1_2_4]")


def execute_and_time():
    qubits = 3
    qc = QuantumCircuit(qubits)
    
    start = time.time()

    # Apply RX gates
    for qubit in range(qubits):
        qc.rx(math.pi / 4, qubit)

    simulator = AerSimulator(method='statevector')
    qc.save_statevector()
    job = simulator.run(qc)
    result = job.result()
    statevector = result.get_statevector()

    end = time.time()
    print(f"Qiskit Execution Time: {(end - start) * 1e6} microseconds")

if __name__ == '__main__':
    execute_and_time()

