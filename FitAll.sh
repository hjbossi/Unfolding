#!/bin/bash

Tag=${1}
Type=kErrors

mkdir -p Output/$Tag/Bayes/Flat/$Type
mkdir -p Output/$Tag/Bayes/MC/$Type

mkdir -p log/$Tag/Bayes/Flat/$Type
mkdir -p log/$Tag/Bayes/MC/$Type

echo Running $Tag

export LD_LIBRARY_PATH=/Applications/root_v6.14.00/lib:/Users/Molly/Desktop/Unfolding/RooUnfold/build/
export DYLD_LIBRARY_PATH=/Applications/root_v6.14.00/lib

for i in `ls Input/$Tag/`
do
   echo $i
    ./Execute --Input Input/$Tag/$i --Output Output/$Tag/Bayes/MC/$Type/$i --Prior MC --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit true --Error $Type > log/$Tag/Bayes/MC/$Type/$i.txt 2>&1  &
    ./Execute --Input Input/$Tag/$i --Output Output/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit true --Error $Type > log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &
done