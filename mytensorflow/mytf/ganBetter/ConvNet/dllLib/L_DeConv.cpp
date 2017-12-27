#include <windows.h>
#include <process.h>

//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  


struct ArgList
{
	HANDLE* doneEvent;

	int start;
	int end;
	int out_sx;
	int out_sy;
	int out_depth;
	int sx;
	int sy;
	int pad;
	int stride;
	int in_sx;
	int in_sy;
	int in_depth;
	float* p_in_act_dw;
	float* p_out_act_dw;
	float* p_filters_w;
	float* p_bias_w;

};



void core(
	int filterSize,
	int od,
	int out_x,
	int out_y,
	int outY,
	int outX,

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
	){
	//float a = 0.0f;
	int odfs = od * filterSize;
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

					for (int d = 0; d < in_depth; d += 8)
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


DWORD WINAPI splitX(LPVOID lpParam){
	ArgList* argList = (ArgList*)lpParam;

	int filterSize = argList->sx * argList->sy * argList->in_depth;
	for (int out_x = argList->start; out_x < argList->end; out_x++)
	{
		int outX = out_x * argList->out_depth;
		for (int out_y = 0; out_y < argList->out_sy; out_y++)
		{
			int outY = out_y * argList->out_sx* argList->out_depth;
			for (int od = 0; od < argList->out_depth; od++)
			{
				core(
					filterSize,
					od,
					out_x,
					out_y,
					outY,
					outX,

					argList->sx,
					argList->sy,
					argList->pad,
					argList->stride,
					argList->in_sx,
					argList->in_sy,
					argList->in_depth,
					argList->p_in_act_dw,
					argList->p_out_act_dw,
					argList->p_filters_w,
					argList->p_bias_w
					);
			}
		}
	}

	SetEvent(*(argList->doneEvent));

	return 0;
}


DWORD WINAPI splitY(LPVOID lpParam){
	ArgList* argList = (ArgList*)lpParam;

	int filterSize = argList->sx * argList->sy * argList->in_depth;
	for (int out_y = argList->start; out_y < argList->end; out_y++)
	{
		int outY = out_y * argList->out_sx* argList->out_depth;
		for (int out_x = 0; out_x < argList->out_sx; out_x++)
		{
			int outX = out_x * argList->out_depth;
			for (int od = 0; od < argList->out_depth; od++)
			{
				core(
					filterSize,
					od,
					out_x,
					out_y,
					outY,
					outX,

					argList->sx,
					argList->sy,
					argList->pad,
					argList->stride,
					argList->in_sx,
					argList->in_sy,
					argList->in_depth,
					argList->p_in_act_dw,
					argList->p_out_act_dw,
					argList->p_filters_w,
					argList->p_bias_w
					);
			}
		}
	}

	SetEvent(*(argList->doneEvent));

	return 0;
}

DWORD WINAPI splitD(LPVOID lpParam){
	ArgList* argList = (ArgList*)lpParam;

	int filterSize = argList->sx * argList->sy * argList->in_depth;
	for (int od = argList->start; od < argList->end; od++)
	{
		for (int out_y = 0; out_y < argList->out_sy; out_y++)
		{
			int outY = out_y * argList->out_sx* argList->out_depth;
			for (int out_x = 0; out_x < argList->out_sx; out_x++)
			{
				int outX = out_x * argList->out_depth;
				core(
					filterSize,
					od,
					out_x,
					out_y,
					outY,
					outX,

					argList->sx,
					argList->sy,
					argList->pad,
					argList->stride,
					argList->in_sx,
					argList->in_sy,
					argList->in_depth,
					argList->p_in_act_dw,
					argList->p_out_act_dw,
					argList->p_filters_w,
					argList->p_bias_w
					);
			}
		}
	}

	SetEvent(*(argList->doneEvent));

	return 0;
}




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
	HANDLE* doneEventList;
	doneEventList = new HANDLE[4];

	//TODO regular by pi
	int __dim = 0;
	if (out_sx % 4 == 0){
		__dim = out_sx;
	}
	else if (out_sy % 4 == 0){
		__dim = out_sy;
	}
	else if (out_depth % 4 == 0){
		__dim = out_depth;
	}

	ArgList** argList = new ArgList*[4];
	for (int i = 0; i < 4; i++)
	{
		argList[i] = new ArgList();
		doneEventList[i] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		argList[i]->doneEvent = &doneEventList[i];
		argList[i]->start = i*__dim / 4;
		argList[i]->end = (i + 1)*__dim / 4;
		argList[i]->out_sx = out_sx;
		argList[i]->out_sy = out_sy;
		argList[i]->out_depth = out_depth;
		argList[i]->sx = sx;
		argList[i]->sy = sy;
		argList[i]->pad = pad;
		argList[i]->stride = stride;
		argList[i]->in_sx = in_sx;
		argList[i]->in_sy = in_sy;
		argList[i]->in_depth = in_depth;
		argList[i]->p_in_act_dw = p_in_act_dw;
		argList[i]->p_out_act_dw = p_out_act_dw;
		argList[i]->p_filters_w = p_filters_w;
		argList[i]->p_bias_w = p_bias_w;
	}

	if (out_sx % 4 == 0){
		::CreateThread(NULL, 0, splitX, argList[0], 0, NULL);
		::CreateThread(NULL, 0, splitX, argList[1], 0, NULL);
		::CreateThread(NULL, 0, splitX, argList[2], 0, NULL);
		::CreateThread(NULL, 0, splitX, argList[3], 0, NULL);
	}
	else if (out_sy % 4 == 0){
		::CreateThread(NULL, 0, splitY, argList[0], 0, NULL);
		::CreateThread(NULL, 0, splitY, argList[1], 0, NULL);
		::CreateThread(NULL, 0, splitY, argList[2], 0, NULL);
		::CreateThread(NULL, 0, splitY, argList[3], 0, NULL);
	}
	else if (out_depth % 4 == 0){
		::CreateThread(NULL, 0, splitD, argList[0], 0, NULL);
		::CreateThread(NULL, 0, splitD, argList[1], 0, NULL);
		::CreateThread(NULL, 0, splitD, argList[2], 0, NULL);
		::CreateThread(NULL, 0, splitD, argList[3], 0, NULL);
	}




	WaitForMultipleObjects(4, doneEventList, TRUE, INFINITE);

	delete doneEventList;

	delete argList[0];
	delete argList[1];
	delete argList[2];
	delete argList[3];

	delete argList;
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

