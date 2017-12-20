
//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  


extern "C" __declspec(dllexport) void SSE_UCVFWD(
	int out_sx,
	int out_sy,
	int out_depth,
	int sx,
	int sy,
	int pad,
	int stride,
	int in_sx,
	int in_sy,
	int in_depth,
	float* p_in_act_dw,
	float* p_out_act_dw,
	float* p_filters_w,
	float* p_bias_w
	)
{
	int filterSize = sx * sy * in_depth;

	for (int out_y = 0; out_y < out_sy; out_y++)
	{
		int outY = out_y * out_sx* out_depth;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int outX = out_x * out_depth;
			for (int od = 0; od < out_depth; od++)
			{
				int odfs = od * filterSize;
				//float a = 0.0f;
				__m256 a = _mm256_setzero_ps();
				for (int fy = 0; fy < sy; fy++)
				{
					int in_y = (out_y - fy + pad) / stride;
					if (in_y >= 0 && in_y < in_sy && (out_y - fy + pad) % stride == 0){
						int in_y_x = in_y * in_sx * in_depth;
						int filterY = fy * sx * in_depth;
						for (int fx = 0; fx < sx; fx++)
						{
							int filterX = fx * in_depth;
							int in_x = (out_x - fx + pad) / stride;
							if (in_x >= 0 && in_x < in_sx && (out_x - fx + pad) % stride == 0)
							{
								int in_x_x = in_x * in_depth;

								for (int d = 0; d < in_depth; d+=8)
								{
									//a += p_filters_w[d + filterY + filterX + od* filterSize] * p_in_act_dw[in_y_x + in_x_x + d];
									__m256 v_in = _mm256_load_ps(p_in_act_dw + in_y_x + in_x_x + d);
									//IXYO
									__m256 v_filters = _mm256_load_ps(p_filters_w + d + filterX + filterY + odfs);
									v_in = _mm256_mul_ps(v_in, v_filters);
									a = _mm256_add_ps(a, v_in);
								}
							}
						}
					}
				}
				a = _mm256_hadd_ps(a, a);//8->4
				a = _mm256_hadd_ps(a, a);//4->2
				//p_out_act_dw[outY + outX + od] = a + p_bias_w[od];
				p_out_act_dw[outY + outX + od] = a.m256_f32[0] + a.m256_f32[4] + p_bias_w[od];
			}
		}
	}

}


extern "C" __declspec(dllexport) void UCVFWD(
	int out_sx,
	int out_sy,
	int out_depth,
	int sx,
	int sy,
	int pad,
	int stride,
	int in_sx,
	int in_sy,
	int in_depth,
	float* p_in_act_dw,
	float* p_out_act_dw,
	float* p_filters_w,
	float* p_bias_w
	)
{
	if (in_sx < -10){
		__m256 a = _mm256_setzero_ps();
		a = _mm256_hadd_ps(a, a);
		p_bias_w[0] = a.m256_f32[0] + a.m256_f32[4] + p_bias_w[0];
	}
	int filterSize = sx * sy * in_depth;

	for (int out_y = 0; out_y < out_sy; out_y++)
	{
		int outY = out_y * out_sx* out_depth;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int outX = out_x * out_depth;
			for (int od = 0; od < out_depth; od++)
			{
				float a = 0.0f;
				for (int fy = 0; fy < sy; fy++)
				{
					int in_y = (out_y - fy + pad) / stride;
					if (in_y >= 0 && in_y < in_sy && (out_y - fy + pad) % stride == 0){
						int in_y_x = in_y * in_sx * in_depth;
						int filterY = fy * sx * in_depth;
						for (int fx = 0; fx < sx; fx++)
						{
							int filterX = fx * in_depth;
							int in_x = (out_x - fx + pad) / stride;
							if (in_x >= 0 && in_x < in_sx && (out_x - fx + pad) % stride == 0)
							{
								int in_x_x = in_x * in_depth;

								for (int d = 0; d < in_depth; d++)
								{
									a += p_filters_w[d + filterY + filterX + od* filterSize] * p_in_act_dw[in_y_x + in_x_x + d];
								}
							}
						}
					}
				}
				p_out_act_dw[outY + outX + od] = a + p_bias_w[od];
			}
		}
	}
}

