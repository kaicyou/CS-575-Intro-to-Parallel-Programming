#!/bin/csh

g++  -c  simd.p5.cpp -o simd.p5.o
foreach s ( 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8192000 16384000 32768000)
	echo ARRAYSIZE = $s	
	g++  -o arraymult -DARRAYSIZE=$s arraymult.cpp simd.p5.o  -lm  -fopenmp 
	./arraymult
end
