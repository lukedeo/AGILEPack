#!/usr/bin/env bash
#PBS -t 1-3
#PBS -l mem=16gb,walltime=00:010:00:00
#PBS -l nodes=1:ppn=1
#PBS -q hep
#PBS -o output/out.txt
#PBS -e output/error.txt
#PBS -m ae
#PBS -M luke.deoliveira@yale.edu


declare -a formulas=("top~fjet_Tau1_flat+fjet_Tau2_flat+fjet_Tau3_flat+fjet_SPLIT23_flat" "top~Tau32+Tau21+fjet_SPLIT23_flat" "top~fjet_Tau1_flat+fjet_Tau2_flat+fjet_Tau3_flat+fjet_SPLIT23_flat+Tau32+Tau21")
declare -a structures=("4 5 3 2 1" "3 5 4 2 1" "6 7 5 3 1")

cd $PBS_O_WORKDIR 
mkdir -p output
echo 'submitted from: ' $PBS_O_WORKDIR 


./AGILETopTagger --file=~/scratch_space/mc12_8TeV_JZX_Zprime_perfntuple_shuffled.root --tree=top_train_ntup --save=AGILETopTagger_no_pretrain_"$(echo -n "${formulas[$PBS_ARRAYID-1]}" | openssl dgst -sha1 -hmac "key")".yaml --learning=0.00086 --momentum=0.8 --batch=1 --config=./top_tag_config.yaml -uepochs=0 -d0 -sepochs=40 -start=0 -end=1671000 --type=binary --formula=${formulas[$PBS_ARRAYID-1]} --struct=${structures[$PBS_ARRAYID-1]}
./AGILETopTagger --file=~/scratch_space/mc12_8TeV_JZX_Zprime_perfntuple_shuffled.root --tree=top_train_ntup --save=AGILETopTagger"$(echo -n "${formulas[$PBS_ARRAYID-1]}" | openssl dgst -sha1 -hmac "key")".yaml --learning=0.00086 --momentum=0.8 --batch=1 --config=./top_tag_config.yaml -uepochs=10 -sepochs=40 -start=0 -end=1671000 --type=binary --formula=${formulas[$PBS_ARRAYID-1]} --struct=${structures[$PBS_ARRAYID-1]}



