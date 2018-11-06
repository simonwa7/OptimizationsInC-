#!/bin/bash 
#SBATCH --partition=batch 
#SBATCH --output=slurm-numbers.%N.%j.out

module load anaconda/2 
python setup.py build
python setup.py install --user


#python main.py name pubchem multiplicity charge mapping
# or
#python main.py name multiplicity charge mapping symbol X_cord Y_cord Z_cord symbol X_cord Y_cord Z_cord ... etc

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Helium 1 0 BK He 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Helium 1 0 JW He 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Hydrogen 2 0 BK H 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Hydrogen 2 0 JW H 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Beryllium 1 0 BK Be 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Beryllium 1 0 JW Be 0 0 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Boron 2 0 BK B 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Boron 2 0 JW B 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Carbon 3 0 BK C 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Carbon 3 0 JW C 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Fluoride 2 0 BK F 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Fluoride 2 0 JW F 0 0 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Neon 1 0 BK Ne 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Neon 1 0 JW Ne 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Lithium 2 0 BK Li 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Lithium 2 0 JW Li 0 0 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydrogen pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydrogen pubchem 1 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Helium_Hydride 1 1 BK He 0 0 0 H 0.772 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Helium_Hydride 1 1 JW He 0 0 0 H 0.772 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Nitrogen 4 0 BK N 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Nitrogen 4 0 JW N 0 0 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Oxygen 3 0 BK O 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Atomic_Oxygen 3 0 JW O 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py LiH pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py LiH pubchem 1 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydroxide pubchem 1 -1 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydroxide pubchem 1 -1 JW"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydrogen_Fluoride 1 0 BK H 0 0 0 F 0.920 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Hydrogen_Fluoride 1 0 JW H 0 0 0 F 0.920 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Methylene pubchem 3 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Methylene pubchem 3 0 JW"
# sbatch --mem=8000 -c24 --time=0-00:10:00 --wrap="python main.py Water pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Water pubchem 1 0 JW"

# All done in v2
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Ammonia pubchem 1 0 BK"
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Ammonia pubchem 1 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Chloride pubchem 2 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Chloride pubchem 2 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Silicon pubchem 3 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Silicon pubchem 3 0 JW"

# All done in v2
# sbatch --mem=64000 -c24 --time=0-00:10:00 --wrap="python main.py Borane 1 0 BK B 0 0 0 H 1.19 0 0 H -1.03 .595 0 H -1.03 -.595 0"
# sbatch --mem=64000 -c24 --time=0-00:10:00 --wrap="python main.py Borane 1 0 JW B 0 0 0 H 1.19 0 0 H -1.03 .595 0 H -1.03 -.595 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Sulfur pubchem 3 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Sulfur pubchem 3 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Al pubchem 2 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Al pubchem 2 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Sodium pubchem 2 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Sodium pubchem 2 0 JW"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Magnesium pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Magnesium pubchem 1 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Ar pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Ar pubchem 1 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Phosphorus 4 0 BK P 0 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Phosphorus 4 0 JW P 0 0 0"

# All done in v2
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Ammonium pubchem 1 1 BK"
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Ammonium pubchem 1 1 JW"
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Methane pubchem 1 0 BK"
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Methane pubchem 1 0 JW"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Diatomic_Nitrogen 1 0 BK N 0 0 0 N 1.119 0 0"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Diatomic_Nitrogen 1 0 JW N 0 0 0 N 1.119 0 0"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Oxygen pubchem 3 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Oxygen pubchem 3 0 JW"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Oxygen pubchem 1 0 BK"
# sbatch --mem=4000 -c24 --time=0-00:10:00 --wrap="python main.py Oxygen pubchem 1 0 JW"

# All done in v2
# sbatch --mem=4000 -c24 --time=0-00:45:00 --wrap="python main.py Diatomic_Fluorine 1 0 BK F 0 0 0 F 1.42 0 0"
# sbatch --mem=4000 -c24 --time=0-00:45:00 --wrap="python main.py Diatomic_Fluorine 1 0 JW F 0 0 0 F 1.42 0 0"

# All done in v2
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Carbon_Monoxide pubchem 1 0 BK"
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Carbon_Monoxide pubchem 1 0 JW"
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Sodium_Hydride pubchem 1 0 BK"
# sbatch --mem=12000 -c24 --time=0-00:10:00 --wrap="python main.py Sodium_Hydride pubchem 1 0 JW"

# All done in v2
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Hydrogen_Chloride 1 0 BK H 0 0 0 Cl 1.290 0 0"
# sbatch --mem=32000 -c24 --time=0-00:20:00 --wrap="python main.py Hydrogen_Chloride 1 0 JW H 0 0 0 Cl 1.290 0 0"

# All done in v2
# sbatch --mem=40000 -c24 --time=0-00:20:00 --wrap="python main.py Magnesium_Hydride pubchem 1 0 BK"
# sbatch --mem=40000 -c24 --time=0-00:30:00 --wrap="python main.py Magnesium_Hydride pubchem 1 0 JW"

# All done in v2
# sbatch --mem=40000 -c24 --time=0-00:30:00 --wrap="python main.py Ethyne pubchem 1 0 BK" 
# sbatch --mem=40000 -c24 --time=0-00:30:00 --wrap="python main.py Ethyne pubchem 1 0 JW"

# All done in v2
# sbatch --mem=124964 --time=0-02:00:00 --wrap="python main.py Hydrogen_Sulfide pubchem 1 0 BK"
# sbatch --mem=124964 --time=0-01:00:00 --wrap="python main.py Hydrogen_Sulfide pubchem 1 0 JW"

# All done in v2
# sbatch --mem=258127 --time=0-03:00:00 --wrap="python main.py Lithium_Hydroxide pubchem 1 0 BK"
# sbatch --mem=124964 --time=0-02:00:00 --wrap="python main.py Lithium_Hydroxide pubchem 1 0 JW"

# All done in v2
# sbatch --mem=160000 --time=0-03:00:00 --wrap="python main.py Methanal pubchem 1 0 BK"
# sbatch --mem=258127 --time=0-03:00:00 --wrap="python main.py Methanal pubchem 1 0 JW"

# All done in v2
# sbatch --mem=258127 --time=0-03:00:00 --wrap="python main.py Hydrogen_Peroxide pubchem 1 0 BK"
# sbatch --mem=258127 --time=0-03:00:00 --wrap="python main.py Hydrogen_Peroxide pubchem 1 0 JW"

# All done in v2
# sbatch --mem=124964 --time=0-03:00:00 --wrap="python main.py Potassium 2 0 BK K 0 0 0"
# sbatch --mem=124964 --time=0-02:00:00 --wrap="python main.py Potassium 2 0 JW K 0 0 0"

# All done in v2
# sbatch --mem=387598 --time=0-06:00:00 --wrap="python main.py Ethene pubchem 1 0 BK"
# sbatch --mem=387598 --time=0-06:00:00 --wrap="python main.py Ethene pubchem 1 0 JW"

# Need both (running on: 828, 829)
# sbatch --mem=516751 --time=1-00:00:00 --wrap="python main.py Methanol pubchem 1 0 BK"
# sbatch --mem=516751 --time=1-00:00:00 --wrap="python main.py Methanol pubchem 1 0 JW"

# All done in v2
# sbatch --mem=124964 --time=0-02:00:00 --wrap="python main.py Carbon_Dioxide pubchem 1 0 BK"
# sbatch --mem=124964 --time=0-02:00:00 --wrap="python main.py Carbon_Dioxide   pubchem 1 0 JW"

# All done in v2
# sbatch --mem=124964 --time=0-03:00:00 --wrap="python main.py Nitrogen_Dioxide pubchem 2 0 BK"
# sbatch --mem=124964 --time=0-03:00:00 --wrap="python main.py Nitrogen_Dioxide pubchem 2 0 JW"

# Need both (running on: 085, 086)
# sbatch --mem=516751 --time=1-00:00:00 --wrap="python main.py Sodium_Hydroxide pubchem 1 0 BK"
# sbatch --mem=516751 --time=1-00:00:00 --wrap="python main.py Sodium_Hydroxide pubchem 1 0 JW"

# Need both (running on: 149, 150)
# sbatch --mem=516751 --time=0-10:00:00 --wrap="python main.py Methylamine pubchem 1 0 BK"
# sbatch --mem=516751 --time=0-10:00:00 --wrap="python main.py Methylamine pubchem 1 0 JW"

# All done in v2
# sbatch --mem=387598 --time=0-06:00:00 --wrap="python main.py Ethane pubchem 1 0 BK"
# sbatch --mem=387598 --time=0-06:00:00 --wrap="python main.py Ethane   pubchem 1 0 JW"

# Need both (running on: 833, 834)
# sbatch --mem=124964 --time=0-04:00:00 --wrap="python main.py Bromine 2 0 BK Br 0 0 0"
# sbatch --mem=124964 --time=0-04:00:00 --wrap="python main.py Bromine 2 0 JW Br 0 0 0"

# Need both (running on: 846, 847)
# sbatch --mem=124964 --time=0-04:00:00 --wrap="python main.py Bromine 1 -1 BK Br 0 0 0"
# sbatch --mem=124964 --time=0-04:00:00 --wrap="python main.py Bromine 1 -1 JW Br 0 0 0"

# Need both (running on: 860, 861)
# sbatch --mem=160000 --time=0-06:00:00 --wrap="python main.py Chlorine 1 0 BK Cl 0 0 0 Cl 2.052 0 0"
# sbatch --mem=160000 --time=0-06:00:00 --wrap="python main.py Chlorine 1 0 JW Cl 0 0 0 Cl 2.052 0 0"









