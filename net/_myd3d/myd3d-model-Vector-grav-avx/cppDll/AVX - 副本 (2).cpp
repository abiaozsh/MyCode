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
	__m256d result = _mm256_mul_pd(vec, vec); // x2, y2, z2, w2
	__m256d result1a = _mm256_permute2f128_pd(result, result, 1); // y2, x2, w2, z2,
	__m256d result1 = _mm256_add_pd(result, result1a); // x2+y2, y2+z2, z2+w2, w2+z2
	__m256d result2a = _mm256_permute_pd(_mm256_permute2f128_pd(result1, result1, 1),5); // w2+z2, z2+w2, y2+z2, x2+y2
	__m256d result2 = _mm256_add_pd(result1, result2a); // (x2+y2) + (w2+z2), ...
	result2 = _mm256_sqrt_pd(result2);
	return result2.m256d_f64[0];
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




















/*
a	2301	333	444	111	222	1	1	switch>><< _mm256_permute2f128_pd(__m256d m1, __m256d m2, 1)
c	1032	222	111	444	333	5	101	switch><>< _mm256_shuffle_pd(__m256d m1, __m256d m2, const int select)/_mm256_permute_pd(__m256d m1, 5)

(2, 3, 0, 1)
(0, 1, 2, 3)


0	0	0	0	136	10001000
0	0	111	222	8	1000
0	0	333	444	24	11000
111	222	0	0	128	10000000
111	222	111	222	0	0
111	222	333	444	16	10000
333	444	0	0	129	10000001
333	444	333	444	17	10001

111	111	333	333	0	0
111	111	333	444	8	1000
111	111	444	333	4	100
111	111	444	444	12	1100
111	222	333	333	2	10
111	222	333	444	10	1010
111	222	444	333	6	110
111	222	444	444	14	1110
222	111	333	333	1	1
222	111	333	444	9	1001
222	111	444	333	5	101
222	111	444	444	13	1101
222	222	333	333	3	11
222	222	333	444	11	1011
222	222	444	333	7	111
222	222	444	444	15	1111


*/


extern "C" __declspec(dllexport) void test(double* a)
{
	__m256d v;
	v.m256d_f64[0] = 111;
	v.m256d_f64[1] = 222;
	v.m256d_f64[2] = 333;
	v.m256d_f64[3] = 444;

	int i = 0;
	
	{__m256d res = _mm256_permute2f128_pd(v, v, 0);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 1);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 2);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 3);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 4);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 5);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 6);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 7);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 8);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 9);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 10);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 11);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 12);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 13);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 14);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 15);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 16);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 17);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 18);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 19);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 20);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 21);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 22);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 23);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 24);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 25);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 26);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 27);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 28);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 29);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 30);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 31);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 32);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 33);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 34);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 35);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 36);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 37);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 38);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 39);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 40);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 41);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 42);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 43);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 44);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 45);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 46);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 47);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 48);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 49);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 50);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 51);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 52);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 53);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 54);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 55);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 56);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 57);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 58);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 59);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 60);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 61);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 62);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 63);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 64);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 65);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 66);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 67);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 68);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 69);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 70);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 71);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 72);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 73);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 74);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 75);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 76);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 77);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 78);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 79);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 80);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 81);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 82);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 83);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 84);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 85);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 86);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 87);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 88);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 89);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 90);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 91);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 92);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 93);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 94);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 95);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 96);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 97);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 98);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 99);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 100);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 101);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 102);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 103);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 104);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 105);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 106);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 107);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 108);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 109);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 110);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 111);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 112);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 113);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 114);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 115);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 116);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 117);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 118);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 119);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 120);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 121);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 122);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 123);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 124);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 125);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 126);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 127);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 128);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 129);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 130);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 131);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 132);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 133);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 134);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 135);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 136);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 137);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 138);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 139);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 140);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 141);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 142);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 143);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 144);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 145);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 146);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 147);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 148);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 149);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 150);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 151);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 152);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 153);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 154);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 155);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 156);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 157);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 158);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 159);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 160);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 161);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 162);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 163);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 164);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 165);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 166);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 167);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 168);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 169);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 170);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 171);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 172);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 173);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 174);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 175);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 176);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 177);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 178);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 179);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 180);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 181);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 182);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 183);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 184);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 185);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 186);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 187);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 188);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 189);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 190);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 191);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 192);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 193);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 194);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 195);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 196);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 197);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 198);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 199);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 200);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 201);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 202);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 203);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 204);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 205);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 206);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 207);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 208);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 209);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 210);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 211);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 212);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 213);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 214);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 215);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 216);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 217);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 218);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 219);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 220);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 221);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 222);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 223);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 224);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 225);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 226);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 227);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 228);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 229);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 230);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 231);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 232);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 233);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 234);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 235);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 236);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 237);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 238);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 239);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 240);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 241);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 242);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 243);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 244);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 245);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 246);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 247);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 248);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 249);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 250);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 251);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 252);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 253);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 254);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute2f128_pd(v, v, 255);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	
	//_mm256_permute_pd
/*
	{__m256d res = _mm256_permute_pd(v, 12);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 1);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 2);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 3);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 4);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 5);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 6);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 7);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 8);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 9);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 10);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 11);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 12);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 13);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 14);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 15);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 16);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 17);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 18);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 19);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 20);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 21);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 22);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 23);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 24);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 25);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 26);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 27);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 28);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 29);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 30);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 31);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 32);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 33);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 34);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 35);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 36);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 37);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 38);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 39);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 40);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 41);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 42);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 43);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 44);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 45);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 46);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 47);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 48);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 49);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 50);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 51);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 52);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 53);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 54);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 55);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 56);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 57);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 58);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 59);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 60);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 61);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 62);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 63);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 64);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 65);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 66);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 67);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 68);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 69);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 70);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 71);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 72);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 73);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 74);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 75);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 76);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 77);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 78);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 79);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 80);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 81);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 82);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 83);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 84);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 85);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 86);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 87);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 88);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 89);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 90);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 91);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 92);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 93);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 94);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 95);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 96);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 97);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 98);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 99);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 100);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 101);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 102);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 103);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 104);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 105);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 106);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 107);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 108);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 109);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 110);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 111);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 112);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 113);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 114);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 115);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 116);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 117);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 118);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 119);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 120);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 121);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 122);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 123);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 124);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 125);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 126);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 127);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 128);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 129);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 130);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 131);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 132);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 133);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 134);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 135);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 136);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 137);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 138);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 139);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 140);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 141);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 142);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 143);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 144);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 145);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 146);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 147);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 148);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 149);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 150);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 151);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 152);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 153);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 154);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 155);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 156);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 157);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 158);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 159);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 160);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 161);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 162);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 163);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 164);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 165);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 166);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 167);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 168);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 169);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 170);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 171);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 172);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 173);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 174);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 175);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 176);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 177);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 178);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 179);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 180);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 181);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 182);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 183);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 184);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 185);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 186);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 187);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 188);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 189);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 190);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 191);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 192);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 193);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 194);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 195);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 196);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 197);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 198);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 199);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 200);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 201);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 202);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 203);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 204);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 205);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 206);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 207);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 208);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 209);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 210);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 211);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 212);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 213);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 214);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 215);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 216);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 217);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 218);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 219);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 220);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 221);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 222);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 223);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 224);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 225);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 226);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 227);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 228);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 229);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 230);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 231);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 232);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 233);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 234);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 235);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 236);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 237);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 238);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 239);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 240);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 241);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 242);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 243);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 244);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 245);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 246);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 247);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 248);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 249);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 250);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 251);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 252);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 253);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 254);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	{__m256d res = _mm256_permute_pd(v, 255);a[i++] = res.m256d_f64[0];a[i++] = res.m256d_f64[1];a[i++] = res.m256d_f64[2];a[i++] = res.m256d_f64[3];}
	*/
}
