
## NOTE: linear will not work, lines must be represented as 
##       a single segment USE single OR multiple ONLY
rimtestCplex: 
	$(MAKE) rim verbose=yes optimizer=cplex debug=yes obj_type=single

rimtestNoOpt: 
	$(MAKE) rim verbose=yes optimizer=no_opt debug=yes

rimMultiCplex: 
	$(MAKE) rim verbose=yes optimizer=cplex debug=yes obj_type=multiple

	