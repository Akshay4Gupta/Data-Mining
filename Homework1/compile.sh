trees="apriori fptree"
for tree in $trees; do
	g++ -o $tree $tree.cpp
	echo $tree compiled
done
