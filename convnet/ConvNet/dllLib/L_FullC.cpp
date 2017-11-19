
//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  

extern "C" __declspec(dllexport) void SSE_FCFWD(
	int out_depth,
	int num_inputs,
	float* p_in_act_w,
	float* p_filters_w,
	float* p_bias_w,
	float* p_out_act_w
	)
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

extern "C" __declspec(dllexport) void FCFWD(
	int out_depth,
	int num_inputs,
	float* p_in_act_w,
	float* p_filters_w,
	float* p_bias_w,
	float* p_out_act_w
	)
{
	if (out_depth < -10){
		__m256 a = _mm256_setzero_ps();
		a = _mm256_hadd_ps(a, a);
		p_out_act_w[0] = a.m256_f32[0] + a.m256_f32[4] + p_in_act_w[0];
	}
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


extern "C" __declspec(dllexport) void SSE_FCBWD(
	int out_depth,
	int num_inputs,

	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_out_act_dw,

	float* p_in_act_dw,
	float* p_filters_dw,
	float* p_bias_dw
	)
{
	for (int i = 0; i < num_inputs; i++)
	{
		p_in_act_dw[i] = 0;
	}
	for (int i = 0; i < out_depth; i++)
	{
		int i_num_inputs = i * num_inputs;
		float chain_grad = p_out_act_dw[i];
		__m256 cg = _mm256_set1_ps(chain_grad);
		for (int d = 0; d < num_inputs; d += 8)
		{
			//p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
			__m256 v_filters = _mm256_load_ps(p_filters_w + i_num_inputs + d);
			v_filters = _mm256_mul_ps(v_filters, cg);
			__m256 v_in = _mm256_load_ps(p_in_act_dw + d);
			v_in = _mm256_add_ps(v_in, v_filters);
			_mm256_store_ps(p_in_act_dw + d, v_in);
			//}
			//for (int d = 0; d < num_inputs; d += 8)
			//{
			//p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
			__m256 v_in_w = _mm256_load_ps(p_in_act_w + d);
			v_in_w = _mm256_mul_ps(v_in_w, cg);
			__m256 v_fw = _mm256_load_ps(p_filters_dw + i_num_inputs + d);
			v_fw = _mm256_add_ps(v_fw, v_in_w);
			_mm256_store_ps(p_filters_dw + i_num_inputs + d, v_fw);
		}
		p_bias_dw[i] += chain_grad;
	}
}

extern "C" __declspec(dllexport) void SSEx_FCBWD(
	int out_depth,
	int num_inputs,

	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_out_act_dw,

	float* p_in_act_dw,
	float* p_filters_dw,
	float* p_bias_dw
	)
{

	for (int d = 0; d < num_inputs; d++)
	{
		float f_in_act_dw = 0.0f;

		int i_num_inputs = 0;
		for (int i = 0; i < out_depth; i++)
		{
			float chain_grad = p_out_act_dw[i];
			f_in_act_dw += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
			i_num_inputs += num_inputs;
		}
		p_in_act_dw[d] = f_in_act_dw;
	}


	int i_num_inputs = 0;
	for (int i = 0; i < out_depth; i++)
	{
		float chain_grad = p_out_act_dw[i];
		for (int d = 0; d < num_inputs; d++)
		{
			p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
		}
		i_num_inputs += num_inputs;
	}

	for (int i = 0; i < out_depth; i++)
	{
		int i_num_inputs = i * num_inputs;
		float chain_grad = p_out_act_dw[i];
		p_bias_dw[i] += chain_grad;
	}
}


extern "C" __declspec(dllexport) void FCBWD(
	int out_depth,
	int num_inputs,

	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_out_act_dw,

	float* p_in_act_dw,
	float* p_filters_dw,
	float* p_bias_dw
	)
{
	if (out_depth < -10){
		__m256 a = _mm256_setzero_ps();
		a = _mm256_hadd_ps(a, a);
		p_in_act_dw[0] = a.m256_f32[0] + a.m256_f32[4] + p_in_act_w[0];
	}

	for (int i = 0; i < num_inputs; i++)
	{
		p_in_act_dw[i] = 0;
	}
	for (int i = 0; i < out_depth; i++)
	{
		int i_num_inputs = i * num_inputs;
		float chain_grad = p_out_act_dw[i];
		for (int d = 0; d < num_inputs; d++)
		{
			p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
			p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
		}
		p_bias_dw[i] += chain_grad;
	}
}

