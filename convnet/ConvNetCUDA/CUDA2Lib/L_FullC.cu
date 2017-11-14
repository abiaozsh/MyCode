#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define blockSize 1
//#define blockSize 16

__global__ void K_FCFWD(
	int out_depth,
	int num_inputs,
	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_bias_w,
	float* p_out_act_w
	){
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	float a = 0.0f;
	int i_num_inputs = i * num_inputs;
	for (int d = 0; d < num_inputs; d++)
	{
		a += p_in_act_w[d] * p_filters_w[i_num_inputs + d];
	}
	a += p_bias_w[i];
	p_out_act_w[i] = a;
}
/*
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

*/
extern "C" __declspec(dllexport) int CUDA_FCFWD(
	int out_depth,
	int num_inputs,
	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_bias_w,
	float* p_out_act_w
	){

	K_FCFWD << <out_depth / blockSize, blockSize >> >(
		out_depth,
		num_inputs,
		p_in_act_w,
		p_filters_w,
		p_bias_w,
		p_out_act_w
		);

	return cudaGetLastError();

}




__global__ void K_FCBWD(
	int out_depth,
	int num_inputs,

	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_out_act_dw,

	float* p_in_act_dw,
	float* p_filters_dw,
	float* p_bias_dw
	){
	int d = blockDim.x * blockIdx.x + threadIdx.x;
	
	p_in_act_dw[d] = 0;
	int i_num_inputs = 0;
	float a = 0.0f;
	for (int i = 0; i < out_depth; i++)
	{
		float chain_grad = p_out_act_dw[i];
		a += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data
		//}
		//for (int d = 0; d < num_inputs; d++)
		//{
		p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params

		i_num_inputs += num_inputs;
	}
	p_in_act_dw[d] = a;
}

__global__ void K_FCBWD2(
	int out_depth,
	int num_inputs,

	const float* p_out_act_dw,

	float* p_bias_dw
	){

	int i_num_inputs = 0;
	for (int i = 0; i < out_depth; i++)
	{
		float chain_grad = p_out_act_dw[i];
		p_bias_dw[i] += chain_grad;
		i_num_inputs += num_inputs;
	}
}

extern "C" __declspec(dllexport) int CUDA_FCBWD(
	int out_depth,
	int num_inputs,

	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_out_act_dw,

	float* p_in_act_dw,
	float* p_filters_dw,
	float* p_bias_dw
	){


	K_FCBWD << <out_depth / blockSize, blockSize >> >(
		out_depth,
		num_inputs,

		p_in_act_w,
		p_filters_w,
		p_out_act_dw,

		p_in_act_dw,
		p_filters_dw,
		p_bias_dw
		);

	cudaError_t err = cudaGetLastError();
	if (err != 0)return err;

	K_FCBWD2 << <1, 1 >> >(
		out_depth,
		num_inputs,

		p_out_act_dw,

		p_bias_dw
		);

	return cudaGetLastError();

}
