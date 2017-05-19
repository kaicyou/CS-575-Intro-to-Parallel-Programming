#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct s
{
	float value;
	int pad[NUM];
} Array[4];

float returnValue()
{
	return 2.;
}

int main(int argc, char const *argv[])
{

	omp_set_num_threads( NUMT );

	unsigned int someBigNumber = 1000000000;	// if > 4B, use "long unsigned int"

	double time0 = omp_get_wtime( );
	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
		//float tmp = Array[i].value;
		for( unsigned int j = 0; j < someBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + returnValue();
			//tmp += returnValue();
		}
		//Array[i].value = tmp;
	}
	double time1 = omp_get_wtime( );
	double performanceUnit = 4.*(double)someBigNumber/(time1 - time0)/1000000.;
	printf("Performance = %8.2lf Units/Sec\n", performanceUnit);

	return 0;
}