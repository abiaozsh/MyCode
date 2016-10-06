#include <emmintrin.h>
#include <math.h>
#include <windows.h>
#include <process.h>
#include <malloc.h>

struct PSpot
{
	__m128d pos1;
	__m128d pos2;

	__m128d heading1;
	__m128d heading2;

	double qmass;
	double mass;
	double process;
	int idx;
	int x1;

	double x2;
	double x3;
	double x4;
	double x5;
};

__inline __m128d Length(__m128d vec1,__m128d vec2)
{
	__m128d result1 = _mm_mul_pd(vec1, vec1);
	__m128d result2 = _mm_mul_sd(vec2, vec2);
	__m128d result3 = _mm_shuffle_pd(result1, result1, 1);
	__m128d result4 = _mm_add_sd(result1, result2);
	__m128d result5 = _mm_add_sd(result4, result3);
	__m128d result6 = _mm_sqrt_sd(vec1, result5);
	return result6;
}

const double G = 0.01;

void CalcGravity(int sp, PSpot* allSpot,int* length)
{
	__m128d force1 = _mm_set1_pd(0);
	__m128d force2 = _mm_set1_pd(0);
	
	PSpot* spotSp = &allSpot[sp];

	for(int i=0;i<sp;i++)
	{
		__m128d diff1 = _mm_sub_pd(allSpot[i].pos1, spotSp->pos1);
		__m128d diff2 = _mm_sub_sd(allSpot[i].pos2, spotSp->pos2);

		__m128d r = Length(diff1, diff2);

		if (r.m128d_f64[0]*2 < (spotSp->qmass + allSpot[i].qmass))
		{
			if (allSpot[i].mass > spotSp->mass)
			{
				allSpot[i].heading1 = 
					_mm_add_pd(
						allSpot[i].heading1,
						_mm_mul_pd(
							_mm_sub_pd(spotSp->heading1, allSpot[i].heading1),
							_mm_set1_pd(spotSp->mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				allSpot[i].heading2 = 
					_mm_add_sd(
						allSpot[i].heading2,
						_mm_mul_sd(
							_mm_sub_sd(spotSp->heading2, allSpot[i].heading2),
							_mm_set1_pd(spotSp->mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				allSpot[i].mass += spotSp->mass;
				allSpot[i].qmass = pow(allSpot[i].mass, 0.33333);
				spotSp->mass = 0;

				(*length)--;
				PSpot temp;
				temp = allSpot[sp];
				allSpot[sp] = allSpot[*length];
				allSpot[*length] = temp;
				return;
			}
			else
			{
				spotSp->heading1 = 
					_mm_add_pd(
						spotSp->heading1,
						_mm_mul_pd(
							_mm_sub_pd(allSpot[i].heading1, spotSp->heading1),
							_mm_set1_pd(allSpot[i].mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				spotSp->heading2 = 
					_mm_add_sd(
						spotSp->heading2,
						_mm_mul_sd(
							_mm_sub_sd(allSpot[i].heading2, spotSp->heading2),
							_mm_set1_pd(allSpot[i].mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				spotSp->mass += allSpot[i].mass;
				spotSp->qmass = pow(spotSp->mass, 0.33333);
				allSpot[i].mass = 0;

				(*length)--;
				PSpot temp;
				temp = allSpot[i];
				allSpot[i] = allSpot[*length];
				allSpot[*length] = temp;
				return;
			}
		}

		//float f = (G * spotSp->mass * allSpot[i].mass) / (r.m128d_f64[0] * r.m128d_f64[0] * r.m128d_f64[0]);

		__m128d r1 = r;
		r1.m128d_f64[1] = G;
		__m128d r2 = r;
		r2.m128d_f64[1] = spotSp->mass;
		__m128d r3 = r;
		r3.m128d_f64[1] = allSpot[i].mass;
		__m128d r4 = _mm_mul_pd(_mm_mul_pd(r1, r2), r3);
		__m128d r5 = _mm_shuffle_pd(r4, r4, 3);
		r4 = _mm_shuffle_pd(r4, r4, 0);
		__m128d r6 = _mm_div_pd(r5, r4);

		force1 = _mm_add_pd(force1,_mm_mul_pd(diff1, r6));
		force2 = _mm_add_sd(force2,_mm_mul_sd(diff2, r6));
	}

	for(int i=sp+1;i<*length;i++)
	{
		__m128d diff1 = _mm_sub_pd(allSpot[i].pos1, spotSp->pos1);
		__m128d diff2 = _mm_sub_sd(allSpot[i].pos2, spotSp->pos2);

		__m128d r = Length(diff1, diff2);

		if (r.m128d_f64[0]*2 < (spotSp->qmass + allSpot[i].qmass))
		{
			if (allSpot[i].mass > spotSp->mass)
			{
				allSpot[i].heading1 = 
					_mm_add_pd(
						allSpot[i].heading1,
						_mm_mul_pd(
							_mm_sub_pd(spotSp->heading1, allSpot[i].heading1),
							_mm_set1_pd(spotSp->mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				allSpot[i].heading2 = 
					_mm_add_sd(
						allSpot[i].heading2,
						_mm_mul_sd(
							_mm_sub_sd(spotSp->heading2, allSpot[i].heading2),
							_mm_set1_pd(spotSp->mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				allSpot[i].mass += spotSp->mass;
				allSpot[i].qmass = pow(allSpot[i].mass, 0.33333);
				spotSp->mass = 0;

				(*length)--;
				PSpot temp;
				temp = allSpot[sp];
				allSpot[sp] = allSpot[*length];
				allSpot[*length] = temp;
				return;
			}
			else
			{
				spotSp->heading1 = 
					_mm_add_pd(
						spotSp->heading1,
						_mm_mul_pd(
							_mm_sub_pd(allSpot[i].heading1, spotSp->heading1),
							_mm_set1_pd(allSpot[i].mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				spotSp->heading2 = 
					_mm_add_sd(
						spotSp->heading2,
						_mm_mul_sd(
							_mm_sub_sd(allSpot[i].heading2, spotSp->heading2),
							_mm_set1_pd(allSpot[i].mass / (spotSp->mass + allSpot[i].mass))
						)
					);

				spotSp->mass += allSpot[i].mass;
				spotSp->qmass = pow(spotSp->mass, 0.33333);
				allSpot[i].mass = 0;

				(*length)--;
				PSpot temp;
				temp = allSpot[i];
				allSpot[i] = allSpot[*length];
				allSpot[*length] = temp;
				return;
			}
		}

		//float f = (G * spotSp->mass * allSpot[i].mass) / (r.m128d_f64[0] * r.m128d_f64[0] * r.m128d_f64[0]);

		__m128d r1 = r;
		r1.m128d_f64[1] = G;
		__m128d r2 = r;
		r2.m128d_f64[1] = spotSp->mass;
		__m128d r3 = r;
		r3.m128d_f64[1] = allSpot[i].mass;
		__m128d r4 = _mm_mul_pd(_mm_mul_pd(r1, r2), r3);
		__m128d r5 = _mm_shuffle_pd(r4, r4, 3);
		r4 = _mm_shuffle_pd(r4, r4, 0);
		__m128d r6 = _mm_div_pd(r5, r4);

		force1 = _mm_add_pd(force1,_mm_mul_pd(diff1, r6));
		force2 = _mm_add_sd(force2,_mm_mul_sd(diff2, r6));
	}

	force1 = _mm_div_pd(force1, _mm_set1_pd(spotSp->mass));
	force2 = _mm_div_sd(force2, _mm_set1_pd(spotSp->mass));

	__m128d forcef = Length(force1, force2);

	if (forcef.m128d_f64[0] > 0)
	{
		double gate = 0.001f;
		double step = gate / forcef.m128d_f64[0];

		if (spotSp->process + step < 1)
		{
			spotSp->process += step;
		}
		else
		{
			step = 1 - spotSp->process;
			spotSp->process = 1;
		}

		__m128d stepd = _mm_set1_pd(step);

		spotSp->heading1 = _mm_add_pd(spotSp->heading1,_mm_mul_pd(force1,stepd));
		spotSp->heading2 = _mm_add_sd(spotSp->heading2,_mm_mul_sd(force2,stepd));

		spotSp->pos1 = _mm_add_pd(spotSp->pos1, _mm_mul_pd(spotSp->heading1,stepd));
		spotSp->pos2 = _mm_add_sd(spotSp->pos2, _mm_mul_sd(spotSp->heading2,stepd));
	}
	else
	{
		spotSp->pos1 = _mm_add_pd(spotSp->pos1, spotSp->heading1);
		spotSp->pos2 = _mm_add_sd(spotSp->pos2, spotSp->heading2);
		spotSp->process = 1;
	}
}

void ProcessGravity1(PSpot* spotList,int length)
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
			CalcGravity(sp, spotList, &len);
		}
		else
		{
			break;
		}
	}
}

extern "C" __declspec(dllexport) void ProcessGravity(
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
	PSpot* spotList = (PSpot*)_aligned_malloc(sizeof(PSpot)*length,16);

	for(int i=0;i<length;i++)
	{
		spotList[i].pos1.m128d_f64[0] = posx[i];
		spotList[i].pos1.m128d_f64[1] = posy[i];
		spotList[i].pos2.m128d_f64[0] = posz[i];

		spotList[i].heading1.m128d_f64[0] = headingx[i];
		spotList[i].heading1.m128d_f64[1] = headingy[i];
		spotList[i].heading2.m128d_f64[0] = headingz[i];
		
		spotList[i].qmass = qmass[i];
		spotList[i].mass = mass[i];
		spotList[i].idx = idx[i];
		spotList[i].process = 0;
	}

	ProcessGravity1(spotList, length);

	for(int i=0;i<length;i++)
	{
		posx[i] = spotList[i].pos1.m128d_f64[0];
		posy[i] = spotList[i].pos1.m128d_f64[1];
		posz[i] = spotList[i].pos2.m128d_f64[0];

		headingx[i] = spotList[i].heading1.m128d_f64[0];
		headingy[i] = spotList[i].heading1.m128d_f64[1];
		headingz[i] = spotList[i].heading2.m128d_f64[0];
		
		qmass[i] = spotList[i].qmass;
		mass[i] = spotList[i].mass;
		idx[i] = spotList[i].idx;
	}

	_aligned_free(spotList);
}
