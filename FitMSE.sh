#!/bin/bash

Tag=${1}
Date=${2}
Algorithm=${3}
Prior=${4}

Type=kErrors

mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/${Algorithm}/${Prior}/${Type}/
mkdir -p /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/${Algorithm}/${Prior}/${Type}/log

echo Running ${Tag}

for i in `ls /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/`
do
    echo $i
    ./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/${Algorithm}/${Prior}/${Type}/$i --Prior ${Prior} --Do${Algorithm} false --DoRepeated${Algorithm} false --Do${Algorithm} true --DoT${Algorithm} false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/${Algorithm}/${Prior}/${Type}/log/$i.txt 2>&1  &
    # echo "./ExecuteMSE --Input /data/submit/mitay/unfolding/${Date}/Input/Theory/${Tag}/$i --Output /data/submit/mitay/unfolding/${Date}/Output/Theory/${Tag}/${Algorithm}/${Prior}/${Type}/$i --Prior ${Prior} --Do${Algorithm} true --DoRepeated${Algorithm} false --Do${Algorithm} false --DoT${Algorithm} false --DoInvert false --DoTUnfold false --DoFit false --Error ${Type} > /data/submit/mitay/unfolding/${Date}/Output/Theory/log/${Tag}/${Algorithm}/${Prior}/${Type}/log/$i.txt 2>&1  &"
done
