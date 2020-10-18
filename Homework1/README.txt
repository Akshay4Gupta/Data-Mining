Assignment 1: Frequent Set Data Mining
======================================================================================================

#Files
------------------------------------------------------------------------------------------------------
install.sh:     download the git repo and load modules.
compile.sh:     creates binaries for fptree.cpp and apriori.cpp.
MCS192556.sh:   executes the program based on flag given in the assignment.
remove.sh:      remove all the files created during the process.
apriori.cpp:    code for apriori implementation.
fptree.cpp:     code for fptree implementation.
layout.py:      code for plotting the comparision graph for given values for two above implementation.


#Observation
------------------------------------------------------------------------------------------------------
Execution time for higher support thresholds we observe similar execution time for both the implementation.
but with the lower in support thresholds we observe faster depletion of performance in apriori compared to fptree.

##Apriori
With decrease in support threshold number of candidates as well as frequent itemsets increases rapidly.
We then reduce the candidate set by pruning them based on subset checking to reduce number of database scan.
Still due to huge pruned candidate set we need to calculated support for each one of them to create frequent itemset.
This is the bottleneck in the algorithm.

##FPTree
FPTree generates frequent itemsets only by design, meaning that it doesn't have to calculate supports of itemsets to check whether they are frequent or not which saves time and the algorithm requires only two database scans thus minimizing the delay caused due to IO operations.

#Team Members
------------------------------------------------------------------------------------------------------
2019MCS2554. Aaditya Sinha
2019MCS2556. Akshay Gupta
2019MCS2574. Vivek Singh
