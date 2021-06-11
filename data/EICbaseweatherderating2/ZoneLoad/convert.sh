#!/bin/bash

start_column=2;
for zone in `seq 1 39`; do
	let column=$zone+$start_column
	filename=Zone_$zone
	if [ $zone -lt 10 ];  then
		filename=Zone_0$zone
	fi
	cut PROMOD_Zonal_Load_in_EOM.csv -d, -f$column | tail -n +2 > $filename.csv
	../../../bin/binaries.py txt2bin $filename.csv $filename.uc
done

