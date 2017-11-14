
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define blockSize 1

__global__ void K_CVFWD(
	int filterSize,
	int stride,
	int pad,
	int sx,
	int sy,
	int in_sx,
	int in_sy,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	const float* p_filters_w,
	const float* p_in_act_w,
	const float* p_bias_w,
	float* p_out_act_w
	){
	int out_x = blockDim.x * blockIdx.x + threadIdx.x;
	int out_y = blockDim.y * blockIdx.y + threadIdx.y;

	int frame_x = out_x * stride - pad;
	int frame_y = out_y * stride - pad;
	int out_act_sx_out_y = out_sx * out_y;
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
extern "C" __declspec(dllexport) void CUDA_CVFWD(
	int stride,
	int pad,
	int sx,
	int sy,
	int in_sx,
	int in_sy,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	const float* p_filters_w,
	const float* p_in_act_w,
	const float* p_bias_w,
	float* p_out_act_w
	)
{
	int filterSize = sx * sy * in_depth;

	dim3 threads(blockSize, blockSize);
	dim3 grid(out_sx / blockSize, out_sy / blockSize);

	K_CVFWD << < grid, threads >> >(
		filterSize,
		stride,
		pad,
		sx,
		sy,
		in_sx,
		in_sy,
		in_depth,
		out_sx,
		out_sy,
		out_depth,
		p_filters_w,
		p_in_act_w,
		p_bias_w,
		p_out_act_w

		);

	//for (int out_y = 0; out_y < out_sy; out_y++)
	//{
	//	for (int out_x = 0; out_x < out_sx; out_x++)
	//	{
	//	}
	//}
}
