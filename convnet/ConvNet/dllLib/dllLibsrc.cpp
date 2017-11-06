#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <CL/cl.h>
#include "basic.h"
using namespace std;
using std::vector;

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
int idx = get_global_id(0);\r\n\
//for (int i = 0; i < out_depth; i++)\r\n\
int i = idx;\r\n\
//{\r\n\
float a = 0.0f;\r\n\
int i_num_inputs = i * num_inputs;\r\n\
for (int d = 0; d < num_inputs; d++)\r\n\
{\r\n\
a += p_in_act_w[d] * p_filters_w[i_num_inputs + d]; // for efficiency use Vols directly for now\r\n\
}\r\n\
a += p_bias_w[i];\r\n\
p_out_act_w[i] = a;\r\n\
//}\r\n\
}";

	cl_program program = getProgram(oclobjects, raw_text);

	cl_kernel kernel = _CreateKernel(program, "mykernal");

	return kernel;
}

extern "C" __declspec(dllexport) int RK_FCFWD(
	OpenCLBasic* oclobjects,
	cl_kernel kernel,

	int out_depth,
	int num_inputs,
	cl_mem p_in_act_w,
	cl_mem p_filters_w,
	cl_mem p_bias_w,
	cl_mem p_out_act_w
	){

	cl_int err = CL_SUCCESS;

	clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *)&out_depth);
	clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *)&num_inputs);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&p_in_act_w);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&p_filters_w);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&p_bias_w);
	clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&p_out_act_w);

	size_t global_work_size[1] = { out_depth };

	err = clEnqueueNDRangeKernel(oclobjects->queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

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
	// compute gradient wrt weights and data
	for (int i = 0; i < out_depth; i++)
	{
		int i_num_inputs = i * num_inputs;
		//float chain_grad = out_act.dw[i];
		float chain_grad = p_out_act_dw[i];
		for (int d = 0; d < num_inputs; d++)
		{
			p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
			p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
		}
		p_bias_dw[i] += chain_grad;
	}

}



extern "C" __declspec(dllexport) void CVFWD(
	int stride,
	int pad,
	int sx,
	int sy,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	int filterSize,
	int in_sx,
	int in_sy,
	cl_float* p_filters_w,
	cl_float* p_in_act_w,
	cl_float* p_bias_w,
	cl_float* p_out_act_w
	)
{

	for (int out_y = 0; out_y < out_sy; out_y++)
	{
		int frame_y = out_y * stride - pad;
		int out_act_sx_out_y = out_sx * out_y;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int frame_x = out_x * stride - pad;
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


extern "C" __declspec(dllexport) void CVBWD(
	int stride,
	int pad,
	int sx,
	int sy,
	int in_size,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	int filterSize,
	int in_act_sx,
	int in_act_sy,
	cl_float* p_filters_w,
	cl_float* p_in_act_w,
	cl_float* p_in_act_dw,
	cl_float* p_out_act_dw,
	cl_float* p_bias_dw,
	cl_float* p_filters_dw
	)
{

	for (int i = 0; i < in_size; i++)
	{
		p_in_act_dw[i] = 0;
	}

	for (int out_y = 0; out_y < out_sy; out_y++)
	{
		int y = out_y * stride - pad;
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int x = out_x * stride - pad;
			for (int d = 0; d < out_depth; d++)
			{
				//Vol f = this.filters[d];
				int filterIdx = d * filterSize;
				// convolve centered at this particular location
				//float chain_grad = this.out_act.get_grad(out_x, out_y, d); // gradient from above, from chain rule
				float chain_grad = p_out_act_dw[((out_sx * out_y) + out_x) * out_depth + d];
				for (int fy = 0; fy < sy; fy++)
				{
					int oy = y + fy; // coordinates in the original input array coordinates
					int oy2 = (in_act_sx * oy);
					int fy2 = (sx * fy);
					for (int fx = 0; fx < sx; fx++)
					{
						int ox = x + fx;
						if (oy >= 0 && oy < in_act_sy && ox >= 0 && ox < in_act_sx)
						{
							int fidx = (fy2 + fx) * in_depth + filterIdx;
							int Vidx = (oy2 + ox) * in_depth;
							for (int fd = 0; fd < in_depth; fd++)
							{
								// avoid function call overhead (x2) for efficiency, compromise modularity :(
								p_filters_dw[fidx + fd] += p_in_act_w[Vidx + fd] * chain_grad;
								p_in_act_dw[Vidx + fd] += p_filters_w[fidx + fd] * chain_grad;
							}
						}
					}
				}
				p_bias_dw[d] += chain_grad;
			}
		}
	}
}
