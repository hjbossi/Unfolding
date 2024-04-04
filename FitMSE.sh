#!/bin/bash

Tag=${1}
Date=${2}
Algorithm=${3}
Prior=${4}

Type=kErrors

if [[ ${Algorithm} == "Bayes" ]]; then
    mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/
    mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/log/

    echo Running ${Tag} ${Algorithm} ${Prior}

    for i in `ls /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/`
    do
        echo $i
        # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/$i --Prior ${Prior} --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/log/$i.txt 2>&1  &
        echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/$i --Prior ${Prior} --DoBayes true --DoRepeatedBayes false --DoSVD false --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/Bayes/${Prior}/${Type}/log/$i.txt 2>&1  &"
    done
fi

if [[ ${Algorithm} == "SVD" ]]; then
    mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/
    mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/log/

    echo Running ${Tag} ${Algorithm} ${Prior}

    for i in `ls /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/`
    do
        echo $i
        # ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/$i --Prior ${Prior} --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/log/$i.txt 2>&1  &
        echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/$i --Prior ${Prior} --DoBayes false --DoRepeatedBayes false --DoSVD true --DoTSVD false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/SVD/${Prior}/${Type}/log/$i.txt 2>&1  &"
    done
fi