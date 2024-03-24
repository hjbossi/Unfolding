#!/bin/bash

Tag=${1}
Date=${2}

Type=kErrors

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Flat/$Type/

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Flat/$Type/

echo Running $Tag

for i in `ls /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/`
do
    echo $i
    ./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Original/$Type/$i.txt 2>&1  &
    ./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Original/$Type/$i.txt 2>&1  &
    ./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &
    ./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &

    # echo "./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Original/$Type/$i.txt 2>&1  &"
    # echo "./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Original/$Type/$i.txt 2>&1  &"
    # echo "./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &"
    # echo "./Execute --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Data/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Data/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &"
done
