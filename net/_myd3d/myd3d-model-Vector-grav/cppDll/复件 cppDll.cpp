//#include <emmintrin.h>
#include <xmmintrin.h>
#include <math.h>
#include <windows.h>
#include <process.h>

struct PSpot
{
	__m128 pos;
	__m128 heading;

	float qmass;
	float mass;
	float process;
	float x1;

	float x2;
	float x3;
	float x4;
	float x5;
};

float Length(__m128 vec)
{
	__m128 result = _mm_mul_ps(vec, vec); // x2, y2, z2, w2
	__m128 result1a = _mm_shuffle_ps(result, result, _MM_SHUFFLE(2, 3, 0, 1)); // y2, x2, w2, z2,
	__m128 result1 = _mm_add_ps(result, result1a); // x2+y2, y2+z2, z2+w2, w2+z2
	__m128 result2a = _mm_shuffle_ps(result1, result1, _MM_SHUFFLE(0, 1, 2, 3)); // w2+z2, z2+w2, y2+z2, x2+y2
	__m128 result2 = _mm_add_ps(result1, result2a); // (x2+y2) + (w2+z2), ...
	result2 = _mm_sqrt_ss(result2);
	return result2.m128_f32[0];
}

const float G = 0.01f;

void CalcGravity(int sp, PSpot* allSpot,int length)
{
	__m128 force;
	force = _mm_set_ps1(0);

	for(int i=0;i<length;i++)
	{
		if (i != sp && allSpot[i].mass > 0)
		{
			__m128 diff;
			diff = _mm_sub_ps(allSpot[i].pos,allSpot[sp].pos);

			float r = Length(diff);

			if (r < (allSpot[sp].qmass + allSpot[i].qmass) * 0.01)
			{
				if (allSpot[i].mass > allSpot[sp].mass)
				{
					allSpot[i].heading = 
						_mm_add_ps(
							allSpot[i].heading,
							_mm_mul_ps(
								_mm_sub_ps(allSpot[sp].heading, allSpot[i].heading),
								_mm_set_ps1(allSpot[sp].mass / (allSpot[sp].mass + allSpot[i].mass))
							)
						);
					allSpot[i].mass += allSpot[sp].mass;
					allSpot[i].qmass = pow(allSpot[i].mass, 0.33333f);
					allSpot[sp].mass = 0;
				}
				else
				{
					allSpot[sp].heading = 
						_mm_add_ps(
							allSpot[sp].heading,
							_mm_mul_ps(
								_mm_sub_ps(allSpot[i].heading, allSpot[sp].heading),
								_mm_set_ps1(allSpot[i].mass / (allSpot[sp].mass + allSpot[i].mass))
							)
						);
					allSpot[sp].mass += allSpot[i].mass;
					allSpot[sp].qmass = pow(allSpot[sp].mass, 0.33333f);
					allSpot[i].mass = 0;
				}
				return;
			}

			float f = (G * allSpot[sp].mass * allSpot[i].mass) / (r * r * r);
			force = _mm_add_ps(force,_mm_mul_ps(diff,_mm_set_ps1(f)));
		}
	}
	force = _mm_mul_ps(force,_mm_set_ps1(1 / allSpot[sp].mass));

	float forcef = Length(force);

	if (forcef > 0)
	{
		float gate = 0.001f;
		float step = gate / forcef;

		if (allSpot[sp].process + step < 1)
		{
			allSpot[sp].process += step;
		}
		else
		{
			step = 1 - allSpot[sp].process;
			allSpot[sp].process = 1;
		}

		allSpot[sp].heading = _mm_add_ps(allSpot[sp].heading,_mm_mul_ps(force,_mm_set_ps1(step)));
		allSpot[sp].pos = _mm_add_ps(allSpot[sp].pos, _mm_mul_ps(allSpot[sp].heading,_mm_set_ps1(step)));
	}
	else
	{
		allSpot[sp].pos = _mm_add_ps(allSpot[sp].pos, allSpot[sp].heading);
		allSpot[sp].process = 1;
	}
}

void ProcessGravity1(PSpot* spotList,int length)
{
	while (true)
	{
		float process = 1;
		int sp = -1;
		for(int i=0;i<length;i++)
		{
			if (spotList[i].mass > 0 && spotList[i].process < process)
			{
				process = spotList[i].process;
				sp = i;
			}
		}
		if (sp != -1)
		{
			CalcGravity(sp, spotList, length);
		}
		else
		{
			break;
		}
	}
}

extern "C" __declspec(dllexport) void ProcessGravity(
		float* posx,
		float* posy,
		float* posz,
		float* headingx,
		float* headingy,
		float* headingz,
		float* qmass,
		float* mass,
		int length
	)
{
	PSpot* spotList = new PSpot[length];
	for(int i=0;i<length;i++)
	{
		spotList[i].pos.m128_f32[0] = posx[i];
		spotList[i].pos.m128_f32[1] = posy[i];
		spotList[i].pos.m128_f32[2] = posz[i];
		spotList[i].pos.m128_f32[3] = 0;

		spotList[i].heading.m128_f32[0] = headingx[i];
		spotList[i].heading.m128_f32[1] = headingy[i];
		spotList[i].heading.m128_f32[2] = headingz[i];
		spotList[i].heading.m128_f32[3] = 0;
		
		spotList[i].qmass = qmass[i];
		spotList[i].mass = mass[i];
		spotList[i].process = 0;
	}

	ProcessGravity1(spotList, length);

	for(int i=0;i<length;i++)
	{
		posx[i] = spotList[i].pos.m128_f32[0];
		posy[i] = spotList[i].pos.m128_f32[1];
		posz[i] = spotList[i].pos.m128_f32[2];

		headingx[i] = spotList[i].heading.m128_f32[0];
		headingy[i] = spotList[i].heading.m128_f32[1];
		headingz[i] = spotList[i].heading.m128_f32[2];
		
		qmass[i] = spotList[i].qmass;
		mass[i] = spotList[i].mass;
	}

	delete spotList;
}
