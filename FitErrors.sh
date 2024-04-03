#!/bin/bash

Tag=${1}
Date=${2}

Type=kErrors

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/log/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/log/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/log/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/log/

echo Running $Tag

for i in AANominal0.root AANominal1.root AANominal2.root AANominal3.root PPNominal0.root # AAQCD0.root AAQCD1.root AAQCD2.root AAQCD3.root PPQCD0.root
do
    echo $i
    ./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/log/$i.txt 2>&1  &
    ./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/log/$i.txt 2>&1  &
    ./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/log/$i.txt 2>&1  &
    ./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/log/$i.txt 2>&1  &

    # echo "./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Original/$Type/log/$i.txt 2>&1  &"
    # echo "./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Original/$Type/log/$i.txt 2>&1  &"
    # echo "./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/Bayes/Flat/$Type/log/$i.txt 2>&1  &"
    # echo "./ExecuteErrors --Input /data/submit/mitay/unfolding/${Date}/Input/Data/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Errors/$Tag/SVD/Flat/$Type/log/$i.txt 2>&1  &"
done
