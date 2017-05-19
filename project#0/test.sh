#!/bin/csh

#number of threads:
foreach t (1 2 4 8 16)
	echo NUMT = $t
	foreach s ( 5000 10000 15000 20000 25000 30000 )
		echo ARRAYSIZE = $s	
		g++ -DARRAYSIZE=$s -DNUMT=$t main.c -o pro0 -lm -fopenmp 
		./pro0
	end
end
