#include <omp.h>
#include <stdio.h>
#include <math.h>
#include "simd.p5.h"

#define NUMTRIES         100

void NormMul(float *A, float *B, float *C, int len)
{
        for( int i = 0; i < len; i++ )
        {
                C[i] = A[i] * B[i];
        }
}

void NormMulSum(float *A, float *B, int len)
{
        float sumres = 0.;
        for( int i = 0; i < len; i++ )
        {
                sumres += A[i] + B[i];
        }
}

int
main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        float *A = new float[ARRAYSIZE];
        float *B = new float[ARRAYSIZE];
        float *C = new float[ARRAYSIZE];

	fprintf( stderr, "Array Size = %d \n", ARRAYSIZE);

        double maxmmultsSIMDmul = 0.;
        double summmultsSIMDmul = 0.;
        double maxmmultsNORMmul = 0.;
        double summmultsNORMmul = 0.;
        double maxmmultsSIMDmulsum = 0.;
        double summmultsSIMDmulsum = 0.;
        double maxmmultsNORMmulsum = 0.;
        double summmultsNORMmulsum = 0.;

        double time0, time1, time2, time3, time4;
        double mmults1, mmults2, mmults3, mmults4;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                time0 = omp_get_wtime( );

                SimdMul( A, B, C, ARRAYSIZE);

                time1 = omp_get_wtime( );

                NormMul( A, B, C, ARRAYSIZE);

                time2 = omp_get_wtime( );

                SimdMulSum( A, B, ARRAYSIZE);

                time3 = omp_get_wtime( );

                NormMulSum( A, B, ARRAYSIZE);

                time4 = omp_get_wtime( );

                mmults1 = (double)ARRAYSIZE/(time1-time0)/1000000.;
                summmultsSIMDmul += mmults1;
                if( mmults1 > maxmmultsSIMDmul )
                        maxmmultsSIMDmul = mmults1;

                mmults2 = (double)ARRAYSIZE/(time2-time1)/1000000.;
                summmultsNORMmul += mmults2;
                if( mmults2 > maxmmultsNORMmul )
                        maxmmultsNORMmul = mmults2;

                mmults3 = (double)ARRAYSIZE/(time3-time2)/1000000.;
                summmultsSIMDmulsum += mmults3;
                if( mmults3 > maxmmultsSIMDmulsum )
                        maxmmultsSIMDmulsum = mmults3;

                mmults4 = (double)ARRAYSIZE/(time4-time3)/1000000.;
                summmultsNORMmulsum += mmults4;
                if( mmults4 > maxmmultsNORMmulsum )
                        maxmmultsNORMmulsum = mmults4;
        }
        printf( "   Peak Performance for SimdMul = %8.2lf MegaCals/Sec\n", maxmmultsSIMDmul );
        printf( "Average Performance for SimdMul = %8.2lf MegaCals/Sec\n", summmultsSIMDmul/(double)NUMTRIES );

        printf( "   Peak Performance for NormMul = %8.2lf MegaCals/Sec\n", maxmmultsNORMmul );
        printf( "Average Performance for NormMul = %8.2lf MegaCals/Sec\n", summmultsNORMmul/(double)NUMTRIES );

        printf( "   Peak Performance for SimdMulSum = %8.2lf MegaCals/Sec\n", maxmmultsSIMDmulsum );
        printf( "Average Performance for SimdMulSum = %8.2lf MegaCals/Sec\n", summmultsSIMDmulsum/(double)NUMTRIES );

        printf( "   Peak Performance for NormMulSum = %8.2lf MegaCals/Sec\n", maxmmultsNORMmulsum );
        printf( "Average Performance for NormMulSum = %8.2lf MegaCals/Sec\n", summmultsNORMmulsum/(double)NUMTRIES );

        return 0;
}
