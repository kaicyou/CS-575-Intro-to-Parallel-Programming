#!/bin/csh

#number of threads:
foreach t (1 2 4)
	echo NUMT = $t
	foreach num ( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 )
		echo NUM = $num	
		g++ -DNUM=$num -DNUMT=$t main.c -o pro3 -lm -fopenmp 
		./pro3
	end
end
