#!/bin/bash

Tag=${1}
Date=${2}

Type=kErrors

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Flat/$Type/

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Original/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Flat/$Type/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Flat/$Type/

echo Running $Tag

for i in `ls /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/`
do
    # echo $i
    # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Original/$Type/$i.txt 2>&1  &
    # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Original/$Type/$i.txt 2>&1  &
    # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &
    # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &
    echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Original/$Type/$i --Prior Original --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Original/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Original/$Type/$i --Prior Original --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Original/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/Bayes/Flat/$Type/$i --Prior Flat --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/Bayes/Flat/$Type/$i.txt 2>&1  &"
    # echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/$Tag/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/$Tag/SVD/Flat/$Type/$i --Prior Flat --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error $Type > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/$Tag/SVD/Flat/$Type/$i.txt 2>&1  &"
done
