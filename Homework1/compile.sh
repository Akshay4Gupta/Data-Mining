trees="apriori fptree"
for tree in $trees; do
	g++ -std=c++11 -o $tree $tree.cpp
	echo $tree compiled
done
