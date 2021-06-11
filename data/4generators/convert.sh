#!/bin/bash

start_column=2;
for zone in `seq 1 1`; do
	let column=$zone+$start_column
	filename=Zone_$zone
	if [ $zone -lt 10 ];  then
		filename=Zone_0$zone
	fi
	../../../bin/binaries.py txt2bin $filename.csv $filename.uc
done

