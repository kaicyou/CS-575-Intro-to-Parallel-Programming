#!/bin/csh

#number of threads:
foreach t (1 4 16 64 256 1024 2048 4096 8192)
	#echo NMB = $t
	foreach s ( 8 16 32 64 128 256 512 )
		#echo LOCAL_SIZE = $s	
		g++ -o first -DNMB=$t -DLOCAL_SIZE=$s first.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -w
		./first
	end
end
