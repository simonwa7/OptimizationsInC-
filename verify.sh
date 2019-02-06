#!/bin/sh

python setup.py build
python setup.py install --user

python main.py Hydrogen 2 0 BK H 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Hydrogen 2 0 JW H 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Helium 1 0 BK He 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Helium 1 0 JW He 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py H2 1 0 BK H 0 0 0 H 0 0 0.7414 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py H2 1 0 JW H 0 0 0 H 0 0 0.7414 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py HeH 1 1 BK He 0 0 0 H 0 0 0.772 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py HeH 1 1 JW He 0 0 0 H 0 0 0.772 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py H3 1 3 BK H 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py H3 1 3 JW He 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py H3 1 1 BK H 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py H3 1 1 JW He 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py H3 2 0 BK H 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py H3 2 0 JW He 0 0 0 H 1 0 0 H 2 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Lithium 2 0 BK Li 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Lithium 2 0 JW Li 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Beryllium 1 0 BK Be 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Beryllium 1 0 JW Be 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Boron 2 0 BK B 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Boron 2 0 JW B 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Carbon 3 0 BK C 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Carbon 3 0 JW C 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py N 4 0 BK N 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py N 4 0 JW N 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py O 3 0 BK O 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py O 3 0 JW O 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Fluoride 2 0 BK F 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Fluoride 2 0 JW F 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Neon 1 0 BK Ne 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Neon 1 0 JW Ne 0 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py LiH 1 0 BK H 0 0 0 Li 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py LiH 1 0 JW H 0 0 0 Li 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py BeH 2 0 BK Be 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py BeH 2 0 JW Be 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py BeH 1 1 BK Be 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py BeH 1 1 JW Be 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py BH 1 2 BK B 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py BH 1 2 JW B 0 0 0 H 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py HC 1 -3 BK H 0 0 0 C 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py HC 1 -3 JW H 0 0 0 C 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py HN 1 -2 BK H 0 0 0 N 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py HN 1 -2 JW H 0 0 0 N 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py Hydroxide pubchem 1 -1 BK sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py Hydroxide pubchem 1 -1 JW sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

python main.py HF 1 0 BK H 0 0 0 F 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt
python main.py HF 1 0 JW H 0 0 0 F 1 0 0 sto-3g
python cirq_test.py test_qasm.txt test_qasm_opt.txt

# python main.py O2 1 0 BK O 0 0 0 O 1 0 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt
# python main.py O2 1 0 JW O 0 0 0 O 1 0 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt

# python main.py water pubchem 1 0 BK sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt
# python main.py water pubchem 1 0 JW sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt

# python main.py BH3 1 0 BK H -1 -1 0 B 0 0 0 H 1 -1 0 H 0 1.414 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt
# python main.py BH3 1 0 JW H -1 -1 0 B 0 0 0 H 1 -1 0 H 0 1.414 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt

# python main.py BeH3 1 -1 BK H -1 -1 0 Be 0 0 0 H 1 -1 0 H 0 1.414 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt
# python main.py BeH3 1 -1 JW H -1 -1 0 Be 0 0 0 H 1 -1 0 H 0 1.414 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt

# python main.py BeH2 1 0 BK H -1 0 0 Be 0 0 0 H 1 0 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt
# python main.py BeH2 1 0 JW H -1 0 0 Be 0 0 0 H 1 0 0 sto-3g
# python cirq_test.py test_qasm.txt test_qasm_opt.txt