
//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  

#define SSEAccl

extern "C" __declspec(dllexport) void FCFWD(
	int out_depth,
	int num_inputs,
	float* p_in_act_w,
	float* p_filters_w,
	float* p_bias_w,
	float* p_out_act_w
	)
{
#ifdef SSEAccl
	if (num_inputs % 8 == 0)
	{
		for (int i = 0; i < out_depth; i++)
		{
			//float a = 0.0f;
			__m256 a = _mm256_setzero_ps();
			int i_num_inputs = i * num_inputs;

			for (int d = 0; d < num_inputs; d += 8)
			{
				//a += p_in_act_w[d] * p_filters_w[i_num_inputs + d]; // for efficiency use Vols directly for now
				__m256 v_in = _mm256_load_ps(p_in_act_w + d);
				__m256 v_filters = _mm256_load_ps(p_filters_w + i_num_inputs + d);
				v_in = _mm256_mul_ps(v_in, v_filters);
				a = _mm256_add_ps(a, v_in);
				//a = _mm256_fmadd_ps(v_in, v_filters, a);
			}
			a = _mm256_hadd_ps(a, a);//8->4
			a = _mm256_hadd_ps(a, a);//4->2

			//a += p_bias_w[i];
			p_out_act_w[i] = a.m256_f32[0] + a.m256_f32[4] + p_bias_w[i];
		}
	}
	else
#endif
	{
		for (int i = 0; i < out_depth; i++)
		{
			float a = 0.0f;
			int i_num_inputs = i * num_inputs;
			for (int d = 0; d < num_inputs; d++)
			{
				a += p_in_act_w[d] * p_filters_w[i_num_inputs + d]; // for efficiency use Vols directly for now
			}
			a += p_bias_w[i];
			p_out_act_w[i] = a;
		}
	}
}
