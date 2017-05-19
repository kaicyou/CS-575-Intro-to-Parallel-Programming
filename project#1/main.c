#include <omp.h>
#include <stdio.h>
#include <math.h>

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.

#define NUMTRIES	10

float Height( int, int );

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMS-1
{
	float u = (float)iu / (float)(NUMS-1);
	float v = (float)iv / (float)(NUMS-1);

	// the basis functions:
	
	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	// finally, we get to compute something:

	float height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
			+ bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
			+ bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
			+ bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

	return height;
}

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
        	fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
       	 	return 1;
	#endif	

	omp_set_num_threads( NUMT );
        fprintf( stderr, "Using %d threads\n", NUMT );
        fprintf( stderr, "Using %d subdivisions\n", NUMS );

	// the area of a single full-sized tile:
    double maxmheights = 0.;
    double summheights = 0.;
	float fullTileArea = (  ( (XMAX-XMIN)/(float)(NUMS-1.) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1.) )  );
	float sumVolum = 0.;
	double runningTime = 0.;
	//float volume;
	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();

		//#pragma omp parallel for reduction(+:sumVolum),private(volume)
		#pragma omp parallel for reduction(+:sumVolum)
		for (int i = 0; i < NUMS*NUMS; i++)
		{
			int iu = i % NUMS;
			int iv = i / NUMS;
			float TrueArea = fullTileArea;
			if ((iu==0)||(iu==(NUMS-1)))
			{
				TrueArea = TrueArea / 2.;
			}
			if ((iv==0)||(iv==(NUMS-1)))
			{
				TrueArea = TrueArea / 2.;
			}
			//volume = Height(iu,iv) * TrueArea;
			//sumVolum = sumVolum + volume;
			sumVolum += Height(iu,iv) * TrueArea;
		}
		double time1 = omp_get_wtime();
		double mheights = (double)(NUMS*NUMS)/(time1-time0)/1000000.;
                summheights += mheights;
                if( mheights > maxmheights )
                        maxmheights = mheights;
        runningTime  += ((time1-time0)*1000000.);
	}
	printf( "Average Executing Time = %8.2fms\n", runningTime / (double)NUMTRIES);
	printf( "	Sum of Volume = %8.2lf\n", sumVolum/(double)NUMTRIES );
	printf( "   Peak Performance = %8.2lf MegaHeights/Sec\n", maxmheights );
    printf( "Average Performance = %8.2lf MegaHeights/Sec\n", summheights/(double)NUMTRIES );

    return 0;
}
