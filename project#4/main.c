#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int	NowYear;		// 2016 - 2021
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int		NowNumDeer;		// number of deer in the current population
int 	NowNumTiger;	// number of tiger in the current population

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =	0.5;

const float AVG_PRECIP_PER_MONTH =	6.0;
const float AMP_PRECIP_PER_MONTH =	6.0;
const float RANDOM_PRECIP =			2.0;

const float AVG_TEMP =			50.0;
const float AMP_TEMP =			20.0;
const float RANDOM_TEMP =		10.0;

const float MIDTEMP = 	35.0;
const float MIDPRECIP = 8.0;

float
Ranf( float low, float high )
{
        float r = (float) rand( );               // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(low,high) );
}

void GrainDeer()
{
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		int tempNumDeer = NowNumDeer;
		int tempNumTiger = NowNumTiger;

		if( tempNumDeer < NowHeight )
			tempNumDeer ++;
		else
			tempNumDeer --;

		tempNumDeer -= tempNumTiger/2;

		if (tempNumDeer < 0)
			tempNumDeer = 0;
		// DoneComputing barrier:
		#pragma omp barrier
		NowNumDeer = tempNumDeer;

		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
	}
}

void Grain()
{
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		float tempFactor = expf(-powf((NowTemp-MIDTEMP)/10.,2.));
		float precipFactor = expf(-powf((NowPrecip-MIDPRECIP)/10.,2.));
		float tempHeight = NowHeight;
		tempHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		if(tempHeight < 0.)
 			tempHeight = 0.;
		// DoneComputing barrier:
		#pragma omp barrier
		NowHeight = tempHeight;

		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
	}
}

void Watcher()
{
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:

		// DoneComputing barrier:
		#pragma omp barrier

		// DoneAssigning barrier:
		#pragma omp barrier
		printf("%d/%d\t%f\t%f\t%f\t%d\t%d\n", NowYear, NowMonth+1, (5./9.)*(NowTemp-32), NowPrecip, NowHeight*2.54, NowNumDeer, NowNumTiger);
		NowMonth ++;
		if (NowMonth == 12)
		{
			NowMonth = 0;
			NowYear ++;
		}
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	
		if( NowPrecip < 0. )
			NowPrecip = 0.;
		// DonePrinting barrier:
		#pragma omp barrier
	}
}

void DeerTiger()
{
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		float tempNumDeer = NowNumDeer;
		float tempNumTiger = NowNumTiger;
		float tigerFactor = tempNumDeer - tempNumTiger/2;
		if(tempNumDeer >= 5)
		{
			if(tigerFactor > 2)
				tempNumTiger ++;
			else
				tempNumTiger --;
		}
		else
			tempNumTiger -= 2;

		if(tempNumTiger < 0)
			tempNumTiger = 0;
		// DoneComputing barrier:
		#pragma omp barrier
		NowNumTiger = tempNumTiger;

		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
}
}

int main(int argc, char const *argv[])
{
	// starting date and time:
	NowMonth =    0;
	NowYear  = 2016;

	// starting state (feel free to change this if you want):
	NowNumDeer = 7;
	NowHeight =  20.;
	NowNumTiger = 1;

	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	omp_set_num_threads( 4 );
	printf("Date\tTemperature(°C)\tPrecipitation\tGrain(cm)\tDeer\tTiger\n");
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}

		#pragma omp section
		{
			DeerTiger( );
		}
	}       // implied barrier -- all functions must return in order to allow any of them to get past here
}