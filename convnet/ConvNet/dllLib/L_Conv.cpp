
//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  

extern "C" __declspec(dllexport) void SSE_CVFWD(
	int stride,
	int padx,
	int pady,
	int sx,
	int sy,
	int in_sx,
	int in_sy,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	float* p_filters_w,
	float* p_in_act_w,
	float* p_bias_w,
	float* p_out_act_w
	)
{
	int filterSize = sx * sy * in_depth;

	for (int out_y = 0; out_y < out_sy; out_y++)
	{
		int frame_y = out_y * stride - pady;
		int out_act_sx_out_y = out_sx * out_y;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int frame_x = out_x * stride - padx;
			// convolve centered at this particular location
			for (int d = 0; d < out_depth; d++)
			{
				//Vol f = this.filters[d];
				int filterIdx = d * filterSize;
				//float a = 0.0f;
				__m256 a = _mm256_setzero_ps();

				for (int fy = 0; fy < sy; fy++)
				{
					int oy = frame_y + fy; // coordinates in the original input array coordinates
					int oy2 = (in_sx * oy);
					int fy2 = (fy * sx);
					for (int fx = 0; fx < sx; fx++)
					{
						int ox = frame_x + fx;
						if (oy >= 0 && oy < in_sy && ox >= 0 && ox < in_sx)
						{
							int fidx = (fy2 + fx) * in_depth + filterIdx;
							int Vidx = (oy2 + ox) * in_depth;
							for (int fd = 0; fd < in_depth; fd += 8)
							{
								// avoid function call overhead (x2) for efficiency, compromise modularity :(
								//a += p_in_act_w[Vidx + fd] * p_filters_w[fidx + fd];

								__m256 v_in = _mm256_load_ps(p_in_act_w + Vidx + fd);
								__m256 v_filters = _mm256_load_ps(p_filters_w + fidx + fd);
								v_in = _mm256_mul_ps(v_in, v_filters);
								a = _mm256_add_ps(a, v_in);
							}
						}
					}
				}
				a = _mm256_hadd_ps(a, a);//8->4
				a = _mm256_hadd_ps(a, a);//4->2

				//p_out_act_w[(out_act_sx_out_y + out_x) * out_depth + d] = a + p_bias_w[d];
				p_out_act_w[(out_act_sx_out_y + out_x) * out_depth + d] = a.m256_f32[0] + a.m256_f32[4] + p_bias_w[d];
			}
		}
	}

}
extern "C" __declspec(dllexport) void CVFWD(
	int stride,
	int padx,
	int pady,
	int sx,
	int sy,
	int in_sx,
	int in_sy,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	float* p_filters_w,
	float* p_in_act_w,
	float* p_bias_w,
	float* p_out_act_w
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
		int frame_y = out_y * stride - pady;
		int out_act_sx_out_y = out_sx * out_y;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int frame_x = out_x * stride - padx;
			// convolve centered at this particular location
			for (int d = 0; d < out_depth; d++)
			{
				//Vol f = this.filters[d];
				int filterIdx = d * filterSize;
				float a = 0.0f;
				for (int fy = 0; fy < sy; fy++)
				{
					int oy = frame_y + fy; // coordinates in the original input array coordinates
					int oy2 = (in_sx * oy);
					int fy2 = (fy * sx);
					for (int fx = 0; fx < sx; fx++)
					{
						int ox = frame_x + fx;
						if (oy >= 0 && oy < in_sy && ox >= 0 && ox < in_sx)
						{
							int fidx = (fy2 + fx) * in_depth + filterIdx;
							int Vidx = (oy2 + ox) * in_depth;
							for (int fd = 0; fd < in_depth; fd++)
							{
								// avoid function call overhead (x2) for efficiency, compromise modularity :(
								a += p_filters_w[fidx + fd] * p_in_act_w[Vidx + fd];
							}
						}
					}
				}
				a += p_bias_w[d];
				p_out_act_w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
			}
		}
	}
}
