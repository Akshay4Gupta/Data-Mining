datafile=$1
support=$2
totalargs=$#
if [ $totalargs = 4 ]
then
	treetype=$(echo $3 | cut -d'-' -f 2)
	outputfile=$4
	g++ -o $treetype $treetype.cpp
	$treetype $datafile $support $outputfile.txt
else
	treetype=$(echo $support | cut -d'-' -f 2)
	if [ $treetype = "plot" ]
	then
		python3 layout.py		
	else
		echo "Invalid argument "$2
	fi	
fi
