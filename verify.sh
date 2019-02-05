#!/bin/sh

python setup.py build
python setup.py install

python main.py Helium 1 0 BK He 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Helium 1 0 JW He 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Hydrogen 2 0 BK H 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Hydrogen 2 0 JW H 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Beryllium 1 0 BK Be 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Beryllium 1 0 JW Be 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Boron 2 0 BK B 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Boron 2 0 JW B 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Carbon 3 0 BK C 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Carbon 3 0 JW C 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Fluoride 2 0 BK F 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Fluoride 2 0 JW F 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Neon 1 0 BK Ne 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Neon 1 0 JW Ne 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py Lithium 2 0 BK Li 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py Lithium 2 0 JW Li 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py H2 1 0 BK H 0 0 0 H 0 0 0.7414 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py H2 1 0 JW H 0 0 0 H 0 0 0.7414 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py HeH 1 1 BK He 0 0 0 H 0 0 0.772 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py HeH 1 1 JW He 0 0 0 H 0 0 0.772 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py N 4 0 BK N 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py N 4 0 JW N 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

python main.py O 3 0 BK O 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt
python main.py O 3 0 JW O 0 0 0 sto-3g
python test_cirq.py test_qasm.txt test_qasm_opt.txt

# python main.py LiH pubchem 1 0 BK sto-3g
# python test_cirq.py test_qasm.txt test_qasm_opt.txt
# python main.py LiH pubchem 1 0 JW sto-3g
# python test_cirq.py test_qasm.txt test_qasm_opt.txt

# python main.py Hydroxide pubchem 1 -1 BK sto-3g
# python test_cirq.py test_qasm.txt test_qasm_opt.txt
# python main.py Hydroxide pubchem 1 -1 JW sto-3g
# python test_cirq.py test_qasm.txt test_qasm_opt.txt