#!/usr/bin/env bash
#PBS -l mem=8gb,walltime=00:10:00:00
#PBS -l nodes=1:ppn=1
#PBS -q hep
#PBS -o output/out.txt
#PBS -e output/error.txt
#PBS -m ae
#PBS -M luke.deoliveira@yale.edu


form="top~fjet_WIDTH_flat+fjet_PlanarFlow_flat+fjet_SPLIT12_flat+fjet_SPLIT23_flat+\
	fjet_SPLIT34_flat+fjet_ZCUT12_flat+fjet_ZCUT23_flat+fjet_ZCUT34_flat+fjet_Angularity_flat+\
	Tau21+Tau32+fjet_Dip12_flat+fjet_Dip13_flat+fjet_Dip23_flat+fjet_DipExcl12_flat+fjet_PullMag_flat+\
	fjet_PullPhi_flat+fjet_QW_flat+fjet_Tau1_flat+fjet_Tau2_flat+fjet_Tau3_flat+fjet_m_flat"


cd $PBS_O_WORKDIR 
mkdir -p output
echo 'submitted from: ' $PBS_O_WORKDIR 


./AGILETopTagger --file=~/scratch_space/mc12_8TeV_JZX_Zprime_perfntuple_shuffled.root --tree=top_train_ntup --save=AGILETopTaggerTwikiVariables.yaml --learning=0.0006 --momentum=0.86 --batch=1 --config=./top_tag_config.yaml -uepochs=30 -sepochs=80 -start=0 -end=1671000 --type=binary --formula="${form}" --struct=22 25 21 16 8 4 1



