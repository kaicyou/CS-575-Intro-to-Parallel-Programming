#!/bin/csh

#number of threads:
foreach t (1 4 16 64 256 1024 2048 4096 8192)
	#echo NMB = $t
		g++ -o third -DNMB=$t third.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -w
		./third
	end
end
