#!/bin/csh

#number of threads:
foreach t (1 2 4 8)
	echo NUMT = $t
		g++ -DNUMT=$t main.c -o pro2 -O3 -lm -fopenmp 
		./pro2
	end
end
