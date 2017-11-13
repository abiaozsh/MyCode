#include "cuda_runtime.h"
#include "device_launch_parameters.h"

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

extern "C" __declspec(dllexport) int CUDA_FCFWD(
	int out_depth,
	int num_inputs,
	const float* p_in_act_w,
	const float* p_filters_w,
	const float* p_bias_w,
	float* p_out_act_w
	){

	K_FCFWD << <out_depth/1,1 >> >(
		out_depth,
		num_inputs,
		p_in_act_w,
		p_filters_w,
		p_bias_w,
		p_out_act_w
	);

	return cudaGetLastError();

}
