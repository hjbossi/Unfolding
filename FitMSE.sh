#!/bin/bash

Tag=${1}
Type=kErrors

mkdir -p Output/$Tag/Bayes/MC/$Type/MSE
mkdir -p Output/$Tag/SVD/MC/$Type/MSE
mkdir -p Output/$Tag/Bayes/Flat/$Type/MSE
mkdir -p Output/$Tag/SVD/Flat/$Type/MSE

mkdir -p log/$Tag/Bayes/MC/$Type/MSE
mkdir -p log/$Tag/SVD/MC/$Type/MSE
mkdir -p log/$Tag/Bayes/Flat/$Type/MSE
mkdir -p log/$Tag/SVD/Flat/$Type/MSE

echo Running $Tag

for i in `ls Theory/$Tag/`
do
#    echo $i
    echo "./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/Bayes/MC/$Type/MSE/$i --Prior MC --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/Bayes/MC/$Type/MSE/$i.txt 2>&1  &"
    # ./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/Bayes/MC/$Type/MSE/$i --Prior MC --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/Bayes/MC/$Type/MSE/$i.txt 2>&1  &
    echo "./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/SVD/MC/$Type/MSE/$i --Prior MC --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/SVD/MC/$Type/MSE/$i.txt 2>&1  &"
    # ./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/SVD/MC/$Type/MSE/$i --Prior MC --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/SVD/MC/$Type/MSE/$i.txt 2>&1  &
    echo "./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/Bayes/Flat/$Type/MSE/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/Bayes/Flat/$Type/MSE/$i.txt 2>&1  &"
    # ./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/Bayes/Flat/$Type/MSE/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/Bayes/Flat/$Type/MSE/$i.txt 2>&1  &
    echo "./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/SVD/Flat/$Type/MSE/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/SVD/Flat/$Type/MSE/$i.txt 2>&1  &"
    # ./ExecuteMSE --Input Theory/$Tag/$i --Output Output/$Tag/SVD/Flat/$Type/MSE/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > log/$Tag/SVD/Flat/$Type/MSE/$i.txt 2>&1  &
done
