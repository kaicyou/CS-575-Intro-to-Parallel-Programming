#!/bin/csh

#number of threads:
foreach t (1 2 4 8 16)
	echo NUMT = $t
	foreach s ( 50 100 500 1500 3000 4500 6000 )
		echo NUMS = $s	
		g++ -DNUMS=$s -DNUMT=$t main.c -o pro1 -O3 -lm -fopenmp 
		./pro1
	end
end
