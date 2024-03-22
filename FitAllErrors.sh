#!/bin/bash

Tag=${1}
Date=${2}

Type=kErrors

mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/MC/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/MC/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/Flat/$Type/

mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/MC/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/MC/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/Flat/$Type/

echo Running $Tag

for i in AANominal0.root AANominal1.root AANominal2.root AANominal3.root PPNominal0.root AAQCD0.root AAQCD1.root AAQCD2.root AAQCD3.root PPQCD0.root
do
    echo $i
    ./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/MC/$Type/$i --Prior MC --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/MC/$Type/$i.txt 2>&1  &
    ./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/MC/$Type/$i --Prior MC --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/MC/$Type/$i.txt 2>&1  &
    ./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &
    ./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &

    # echo "./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/MC/$Type/$i --Prior MC --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/MC/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/MC/$Type/$i --Prior MC --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/MC/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteErrorsLight --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Errors/Data/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Errors/Data/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &"
done
