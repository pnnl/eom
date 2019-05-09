#!/bin/sh

export RIM_HOME="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )"/../.. && pwd )"

cd $RIM_HOME
make pristine && \
make triangle && \
make rim optimizer=cplex verbose=yes debug=yes obj_type=single && \
make result_reader && \
cd data/triangle_rising/input
rm -rf ../output/*
rm -rf results
rm -f rim-log
export peak=900
export valley=550
export rise=100
export rampStart=20
mkdir results # TODO: BUG, RIM should make the results directory if it does not exist
$RIM_HOME/bin/triangle -peak $peak -valley $valley -period 1 -duration 30 -stripe 60  -rise $rise -rampDays 7 -rampStart $rampStart -function triangle thisDoesNotMatterButIsNeeded
$RIM_HOME/bin/triangle -peak $peak -valley $valley -period 1 -duration 30 -stripe 10  -rise $rise -rampDays 7 -rampStart $rampStart -function triangle thisDoesNotMatterButIsNeeded
$RIM_HOME/bin/triangle -peak $peak -valley $valley -period 1 -duration 30 -stripe 1  -rise $rise -rampDays 7 -rampStart $rampStart -function triangle thisDoesNotMatterButIsNeeded
mv triangle_$valley\_$peak\_1440_43200_60_\[$rise\]_\[10080_$rampStart\].bin triangle_$valley\_$peak\_1440_43200_\[$rise\]_\[10080_$rampStart\].uc 
mv triangle_$valley\_$peak\_1440_43200_10_\[$rise\]_\[10080_$rampStart\].bin triangle_$valley\_$peak\_1440_43200_\[$rise\]_\[10080_$rampStart\].lf
mv triangle_$valley\_$peak\_1440_43200_1_\[$rise\]_\[10080_$rampStart\].bin triangle_$valley\_$peak\_1440_43200_\[$rise\]_\[10080_$rampStart\].reg
$RIM_HOME/bin/triangle -peak 0 -valley 0 -period 1 -duration 30 -stripe 60 -rise 0 -rampDays 7 -rampStart 0 -function triangle thisDoesNotMatterButIsNeeded
$RIM_HOME/bin/triangle -peak 0 -valley 0 -period 1 -duration 30 -stripe 10 -rise 0 -rampDays 7 -rampStart 0 -function triangle thisDoesNotMatterButIsNeeded
$RIM_HOME/bin/triangle -peak 0 -valley 0 -period 1 -duration 30 -stripe 1 -rise 0 -rampDays 7 -rampStart 0 -function triangle thisDoesNotMatterButIsNeeded
mv triangle_0_0_1440_43200_60_\[0\]_\[10080_0\].bin triangle_0_0_1440_43200_\[0\]_\[10080_0\].uc 
mv triangle_0_0_1440_43200_10_\[0\]_\[10080_0\].bin triangle_0_0_1440_43200_\[0\]_\[10080_0\].lf
mv triangle_0_0_1440_43200_1_\[0\]_\[10080_0\].bin triangle_0_0_1440_43200_\[0\]_\[10080_0\].reg
$RIM_HOME/bin/rim sources.txt
mv results ../output
mv rim-log ../output
cd ../output
echo $RIM_HOME/bin/resultReader results/metadata results optimizedGeneration
$RIM_HOME/bin/resultReader results/metadata results optimizedGeneration
wc=`diff -w optimizedGeneration.csv ../reference-output/optimizedGeneration.csv | wc -l`
if [ "$wc" -eq 0 ] ; then
  echo "success!"
  exit 0
fi

echo "failure!"
exit 1
