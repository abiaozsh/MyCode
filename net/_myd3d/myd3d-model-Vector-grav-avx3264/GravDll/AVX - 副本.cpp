#include <immintrin.h>
#include <math.h>
#include <windows.h>
#include <process.h>
#include <malloc.h>

struct PSpotA
{
	__m256d pos;
	__m256d heading;

	double qmass;
	double mass;
	double process;
	int x1;
	int idx;

	double a1;
	double b1;
	double c1;
	double d1;
};

double LengthA(__m256d vec)
{
	__m256d result = _mm256_mul_pd(vec, vec);
	return sqrt(result.m256d_f64[0] + result.m256d_f64[1] + result.m256d_f64[2]);

	//__m256d result = _mm256_mul_pd(vec, vec); // x2, y2, z2, w2
	//__m256d result1a = _mm256_shuffle_pd(result, result, _MM_SHUFFLE(2, 3, 0, 1)); // y2, x2, w2, z2,
	//__m256d result1 = _mm256_add_pd(result, result1a); // x2+y2, y2+z2, z2+w2, w2+z2
	//__m256d result2a = _mm256_shuffle_pd(result1, result1, _MM_SHUFFLE(0, 1, 2, 3)); // w2+z2, z2+w2, y2+z2, x2+y2
	//__m256d result2 = _mm256_add_pd(result1, result2a); // (x2+y2) + (w2+z2), ...
	//result2 = _mm256_sqrt_pd(result2);
	//return result2.m256d_f64[0];
}

const double G = 0.1;

void merge(int sp,PSpotA* spotSp, int i, PSpotA* allSpot, int* length)
{
	if (allSpot[i].mass > allSpot[sp].mass)
	{
		allSpot[i].heading = 
			_mm256_add_pd(
				allSpot[i].heading,
				_mm256_mul_pd(
					_mm256_sub_pd(allSpot[sp].heading, allSpot[i].heading),
					_mm256_set1_pd(allSpot[sp].mass / (allSpot[sp].mass + allSpot[i].mass))
				)
			);
		allSpot[i].mass += allSpot[sp].mass;
		allSpot[i].qmass = pow(allSpot[i].mass, 0.33333);
		allSpot[sp].mass = 0;

		(*length)--;
		PSpotA temp;
		temp = allSpot[sp];
		allSpot[sp] = allSpot[*length];
		allSpot[*length] = temp;
	}
	else
	{
		allSpot[sp].heading = 
			_mm256_add_pd(
				allSpot[sp].heading,
				_mm256_mul_pd(
					_mm256_sub_pd(allSpot[i].heading, allSpot[sp].heading),
					_mm256_set1_pd(allSpot[i].mass / (allSpot[sp].mass + allSpot[i].mass))
				)
			);
		allSpot[sp].mass += allSpot[i].mass;
		allSpot[sp].qmass = pow(allSpot[sp].mass, 0.33333);
		allSpot[i].mass = 0;

		(*length)--;
		PSpotA temp;
		temp = allSpot[i];
		allSpot[i] = allSpot[*length];
		allSpot[*length] = temp;
	}
}
void CalcGravityA(int sp, PSpotA* allSpot, int* length)
{
	__m256d force;
	force = _mm256_set1_pd(0);

	PSpotA* spotSp = &allSpot[sp];

	for(int i = 0;i < sp;i++)
	{
		__m256d diff = _mm256_sub_pd(allSpot[i].pos,allSpot[sp].pos);

		double r = LengthA(diff);

		if (r < (allSpot[sp].qmass + allSpot[i].qmass)*0.1)
		{
			merge(sp, spotSp, i, allSpot, length);
			return;
		}

		double f = (G * allSpot[sp].mass * allSpot[i].mass) / (r * r * r);
		force = _mm256_add_pd(force,_mm256_mul_pd(diff,_mm256_set1_pd(f)));
	}
	for(int i=sp+1;i<*length;i++)
	{
		__m256d diff = _mm256_sub_pd(allSpot[i].pos,allSpot[sp].pos);

		double r = LengthA(diff);

		if (r < (allSpot[sp].qmass + allSpot[i].qmass)*0.1)
		{
			merge(sp, spotSp, i, allSpot, length);
			return;
		}

		double f = (G * allSpot[sp].mass * allSpot[i].mass) / (r * r * r);
		force = _mm256_add_pd(force,_mm256_mul_pd(diff,_mm256_set1_pd(f)));
	}
	force = _mm256_mul_pd(force,_mm256_set1_pd(1 / allSpot[sp].mass));

	double forcef = LengthA(force);

	if (forcef > 0)
	{
		double gate = 0.001;
		double step = gate / forcef;

		if (allSpot[sp].process + step < 1)
		{
			allSpot[sp].process += step;
		}
		else
		{
			step = 1 - allSpot[sp].process;
			allSpot[sp].process = 1;
		}

		allSpot[sp].heading = _mm256_add_pd(allSpot[sp].heading,_mm256_mul_pd(force,_mm256_set1_pd(step)));
		allSpot[sp].pos = _mm256_add_pd(allSpot[sp].pos, _mm256_mul_pd(allSpot[sp].heading,_mm256_set1_pd(step)));
	}
	else
	{
		allSpot[sp].pos = _mm256_add_pd(allSpot[sp].pos, allSpot[sp].heading);
		allSpot[sp].process = 1;
	}
}

void ProcessGravityA(PSpotA* spotList,int length)
{
	int len = length;
	while (true)
	{
		double process = 1;
		int sp = -1;
		for(int i=0;i<len;i++)
		{
			if (spotList[i].mass > 0 && spotList[i].process < process)
			{
				process = spotList[i].process;
				sp = i;
			}
		}
		if (sp != -1)
		{
			CalcGravityA(sp, spotList, &len);
		}
		else
		{
			break;
		}
	}
}

extern "C" __declspec(dllexport) void ProcessGravityAVX(
		double* posx,
		double* posy,
		double* posz,
		double* headingx,
		double* headingy,
		double* headingz,
		double* qmass,
		double* mass,
		int* idx,
		int length
	)
{
	//PSpot* spotList = new PSpot[length];
	PSpotA* spotList = (PSpotA*)_aligned_malloc(sizeof(PSpotA)*length,32);

	for(int i=0;i<length;i++)
	{
		spotList[i].pos.m256d_f64[0] = posx[i];
		spotList[i].pos.m256d_f64[1] = posy[i];
		spotList[i].pos.m256d_f64[2] = posz[i];
		spotList[i].pos.m256d_f64[3] = 0;

		spotList[i].heading.m256d_f64[0] = headingx[i];
		spotList[i].heading.m256d_f64[1] = headingy[i];
		spotList[i].heading.m256d_f64[2] = headingz[i];
		spotList[i].heading.m256d_f64[3] = 0;
		
		spotList[i].qmass = qmass[i];
		spotList[i].mass = mass[i];
		spotList[i].idx = idx[i];
		spotList[i].process = 0;
	}

	ProcessGravityA(spotList, length);

	for(int i=0;i<length;i++)
	{
		posx[i] = spotList[i].pos.m256d_f64[0];
		posy[i] = spotList[i].pos.m256d_f64[1];
		posz[i] = spotList[i].pos.m256d_f64[2];

		headingx[i] = spotList[i].heading.m256d_f64[0];
		headingy[i] = spotList[i].heading.m256d_f64[1];
		headingz[i] = spotList[i].heading.m256d_f64[2];
		
		qmass[i] = spotList[i].qmass;
		mass[i] = spotList[i].mass;
		idx[i] = spotList[i].idx;
	}

	_aligned_free(spotList);
}
