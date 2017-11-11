#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <CL/cl.h>

//#include <mmintrin.h> //MMX  
//#include <xmmintrin.h> //SSE(include mmintrin.h)  
//#include <emmintrin.h> //SSE2(include xmmintrin.h)  
//#include <pmmintrin.h> //SSE3(include emmintrin.h)  
//#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
//#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
//#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <wmmintrin.h>//AES(include nmmintrin.h)  #include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  

#include "basic.h"

#define SSEAccl

cl_program getProgram(OpenCLBasic* oclobjects, const char* raw_text);
cl_kernel _CreateKernel(cl_program program, const string& kernel_name);

extern "C" __declspec(dllexport) cl_kernel GK_FCFWD(OpenCLBasic* oclobjects)
{
	const char* raw_text = "__kernel void mykernal(\r\n\
const int out_depth,\r\n\
const int num_inputs,\r\n\
__global float* p_in_act_w,\r\n\
__global float* p_filters_w,\r\n\
__global float* p_bias_w,\r\n\
__global float* p_out_act_w\r\n\
){\r\n\
int i = get_global_id(0);\r\n\
float a = 0.0f;\r\n\
int i_num_inputs = i * num_inputs;\r\n\
for (int d = 0; d < num_inputs; d++)\r\n\
{\r\n\
a += p_in_act_w[d] * p_filters_w[i + d*out_depth];\r\n\
}\r\n\
a += p_bias_w[i];\r\n\
p_out_act_w[i] = a;\r\n\
}";

//	const char* raw_text = "__kernel void mykernal(\r\n\
//const int out_depth,\r\n\
//const int num_inputs,\r\n\
//__global float4* p_in_act_w,\r\n\
//__global float4* p_filters_w,\r\n\
//__global float* p_bias_w,\r\n\
//__global float* p_out_act_w\r\n\
//){\r\n\
////int i = get_group_id(0);//get_global_id(0);\r\n\
////int j = get_local_id(0);\r\n\
//float4 a = (float4)(0.0f);\r\n\
//int idx = get_global_id(0);//(i*256+j);\r\n\
//int i_num_inputs = idx * num_inputs; \r\n\
//for (int d = 0; d < num_inputs; d++)\r\n\
//{\r\n\
//a += p_in_act_w[d] * p_filters_w[idx + d*out_depth];\r\n\
//}\r\n\
//p_out_act_w[idx] = a.x + a.y + a.z + a.w + p_bias_w[idx];\r\n\
//}";

	cl_program program = getProgram(oclobjects, raw_text);

	cl_kernel kernel = _CreateKernel(program, "mykernal");

	return kernel;
}

extern "C" __declspec(dllexport) int RK_FCFWD(
	OpenCLBasic* oclobjects,
	cl_kernel kernel,

	int x,
	int y,
	int out_depth,
	int num_inputs,
	cl_mem p_in_act_w,
	cl_mem p_filters_w,
	cl_mem p_bias_w,
	cl_mem p_out_act_w
	){

	cl_int err = CL_SUCCESS;

	int _num_inputs = num_inputs;//
	clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *)&out_depth);
	clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *)&_num_inputs);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&p_in_act_w);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&p_filters_w);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&p_bias_w);
	clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&p_out_act_w);

	size_t global_work_size[1] = { out_depth };//

	//size_t local_work_size[1] = { 256 };//

	//extern CL_API_ENTRY cl_int CL_API_CALL
	//	clEnqueueNDRangeKernel(
	//	cl_command_queue /* command_queue */,
	//	cl_kernel        /* kernel */,
	//	cl_uint          /* work_dim */,
	//	const size_t *   /* global_work_offset */,
	//	const size_t *   /* global_work_size */,
	//	const size_t *   /* local_work_size */,
	//	cl_uint          /* num_events_in_wait_list */,
	//	const cl_event * /* event_wait_list */,
	//	cl_event *       /* event */) CL_API_SUFFIX__VERSION_1_0;

	err = clEnqueueNDRangeKernel(oclobjects->queue, kernel, 1, 0, global_work_size, NULL, 0, NULL, NULL);

	clFinish(oclobjects->queue);

	return err;
}

extern "C" __declspec(dllexport) void FCFWD(
	int out_depth,
	int num_inputs,
	cl_float* p_in_act_w,
	cl_float* p_filters_w,
	cl_float* p_bias_w,
	cl_float* p_out_act_w
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

			for (int d = 0; d < num_inputs; d+=8)
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

extern "C" __declspec(dllexport) cl_kernel GK_FCBWD(OpenCLBasic* oclobjects)
{
	const char* raw_text = "__kernel void mykernal(\r\n\
const int out_depth,\r\n\
const int num_inputs,\r\n\
__global float* p_in_act_w,\r\n\
__global float* p_filters_w,\r\n\
__global float* p_in_act_dw,\r\n\
__global float* p_out_act_dw,\r\n\
__global float* p_filters_dw,\r\n\
__global float* p_bias_dw\r\n\
){\r\n\
int idx = get_global_id(0);\r\n\
//for (int i = 0; i < out_depth; i++)\r\n\
int i = idx;\r\n\
//{\r\n\
int i_num_inputs = i * num_inputs;\r\n\
//float chain_grad = out_act.dw[i];\r\n\
float chain_grad = p_out_act_dw[i];\r\n\
for (int d = 0; d < num_inputs; d++)\r\n\
{\r\n\
p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad;\r\n\
p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad;\r\n\
}\r\n\
p_bias_dw[i] += chain_grad;\r\n\
//}\r\n\
}";

	cl_program program = getProgram(oclobjects, raw_text);

	cl_kernel kernel = _CreateKernel(program, "mykernal");

	return kernel;
}

extern "C" __declspec(dllexport) int RK_FCBWD(
	OpenCLBasic* oclobjects,
	cl_kernel kernel,

	int out_depth,
	int num_inputs,
	cl_mem p_in_act_w,
	cl_mem p_filters_w,
	cl_mem p_in_act_dw,
	cl_mem p_out_act_dw,
	cl_mem p_filters_dw,
	cl_mem p_bias_dw
	){

	cl_int err = CL_SUCCESS;

	clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *)&out_depth);
	clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *)&num_inputs);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&p_in_act_w);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&p_filters_w);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&p_in_act_dw);
	clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&p_out_act_dw);
	clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&p_filters_dw);
	clSetKernelArg(kernel, 7, sizeof(cl_mem), (void *)&p_bias_dw);

	size_t global_work_size[1] = { out_depth };

	err = clEnqueueNDRangeKernel(oclobjects->queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	clFinish(oclobjects->queue);

	return err;
}

extern "C" __declspec(dllexport) void FCBWD(
	int out_depth,
	int num_inputs,
	cl_float* p_in_act_w,
	cl_float* p_filters_w,
	cl_float* p_in_act_dw,
	cl_float* p_out_act_dw,
	cl_float* p_filters_dw,
	cl_float* p_bias_dw
	)
{
	for (int i = 0; i < num_inputs; i++)
	{
		p_in_act_dw[i] = 0;
	}
#ifdef SSEAccl
	if (num_inputs % 8 == 0)
	{
		for (int i = 0; i < out_depth; i++)
		{
			int i_num_inputs = i * num_inputs;
			float chain_grad = p_out_act_dw[i];
			__m256 cg = _mm256_set1_ps(chain_grad);
			for (int d = 0; d < num_inputs; d += 8)
			{
				//p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
				__m256 v_in = _mm256_load_ps(p_in_act_dw + d);
				__m256 v_filters = _mm256_load_ps(p_filters_w + i_num_inputs + d);
				v_filters = _mm256_mul_ps(v_filters, cg);
				v_in = _mm256_add_ps(v_in, v_filters);
				_mm256_store_ps(p_in_act_dw + d, v_in);

				//p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
				__m256 v_fw = _mm256_load_ps(p_filters_dw + i_num_inputs + d);
				__m256 v_in_w = _mm256_load_ps(p_in_act_w + d);
				v_in_w = _mm256_mul_ps(v_in_w, cg);
				v_fw = _mm256_add_ps(v_fw, v_in_w);
				_mm256_store_ps(p_filters_dw + i_num_inputs + d, v_fw);

			}
			p_bias_dw[i] += chain_grad;
		}
	}
	else
#endif
	{
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
}

