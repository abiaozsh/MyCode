#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <CL/cl.h>
#include "basic.h"
using namespace std;
using std::vector;

//#include "oclobject.hpp"



//cl_float* p_input;


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
			//a += in_act.w[d] * filters.w[i * num_inputs + d]; // for efficiency use Vols directly for now
			a += p_in_act_w[d] * p_filters_w[i_num_inputs + d]; // for efficiency use Vols directly for now
		}
		//a += bias.w[i];
		a += p_bias_w[i];
		p_out_act_w[i] = a;
	}

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
		//in_act.dw[d] = 0;
		p_in_act_dw[i] = 0;
	}
	// compute gradient wrt weights and data
	for (int i = 0; i < out_depth; i++)
	{
		int i_num_inputs = i * num_inputs;
		//Vol tfi = this.filters[i];
		//int iw = i * this.num_inputs;
		//float chain_grad = out_act.dw[i];
		float chain_grad = p_out_act_dw[i];
		for (int d = 0; d < num_inputs; d++)
		{
			//in_act.dw[d] += filters.w[i * num_inputs + d] * chain_grad; // grad wrt input data
			p_in_act_dw[d] += p_filters_w[i_num_inputs + d] * chain_grad; // grad wrt input data

			//filters.dw[i * num_inputs + d] += in_act.w[d] * chain_grad; // grad wrt params
			p_filters_dw[i_num_inputs + d] += p_in_act_w[d] * chain_grad; // grad wrt params
		}
		//this.bias.dw[i] += chain_grad;
		p_bias_dw[i] += chain_grad;
	}

}



extern "C" __declspec(dllexport) void CVFWD(
	int stride,
	int unstride, 
	int pad, 
	int sx, 
	int sy, 
	int in_depth, 
	int out_sx, 
	int out_sy, 
	int out_depth, 
	int filterSize, 
	int out_act_sx, 
	int in_act_sx,
	int in_act_sy,
	cl_float* p_filters_w,
	cl_float* p_in_act_w,
	cl_float* p_bias_w,
	cl_float* p_out_act_w
	)
{

	if (unstride > 0)
	{
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int frame_x = out_x / unstride - pad;
			for (int out_y = 0; out_y < out_sy; out_y++)
			{
				int frame_y = out_y / unstride - pad;
				int out_act_sx_out_y = out_act_sx * out_y;
				// convolve centered at this particular location
				for (int d = 0; d < out_depth; d++)
				{
					//Vol f = this.filters[d];
					int filterIdx = d * filterSize;
					float a = 0.0f;
					for (int fy = 0; fy < sy; fy++)
					{
						int oy = frame_y + fy; // coordinates in the original input array coordinates
						int oy2 = (in_act_sx * oy);
						int fy2 = (fy * sx);
						for (int fx = 0; fx < sx; fx++)
						{
							int ox = frame_x + fx;
							if (oy >= 0 && oy < in_act_sy && ox >= 0 && ox < in_act_sx)
							{
								int fidx = (fy2 + fx) * in_depth + filterIdx;
								int Vidx = (oy2 + ox) * in_depth;
								for (int fd = 0; fd < in_depth; fd++)
								{
									// avoid function call overhead (x2) for efficiency, compromise modularity :(
									//a += filters.w[fidx + fd] * in_act.w[Vidx + fd];
									a += p_filters_w[fidx + fd] * p_in_act_w[Vidx + fd];
									
								}
							}
						}
					}
					//a += bias.w[d];
					a += p_bias_w[d];
					//out_act.set(out_x, out_y, d, a);
					//out_act.w[(out_act_sx_out_y + out_x) * out_act.depth + d] = a;
					p_out_act_w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
				}
			}
		}
	}
	else
	{
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int frame_x = out_x * stride - pad;
			for (int out_y = 0; out_y < out_sy; out_y++)
			{
				int frame_y = out_y * stride - pad;
				int out_act_sx_out_y = out_act_sx * out_y;
				// convolve centered at this particular location
				for (int d = 0; d < out_depth; d++)
				{
					//Vol f = this.filters[d];
					int filterIdx = d * filterSize;
					float a = 0.0f;
					for (int fy = 0; fy < sy; fy++)
					{
						int oy = frame_y + fy; // coordinates in the original input array coordinates
						int oy2 = (in_act_sx * oy);
						int fy2 = (fy * sx);
						for (int fx = 0; fx < sx; fx++)
						{
							int ox = frame_x + fx;
							if (oy >= 0 && oy < in_act_sy && ox >= 0 && ox < in_act_sx)
							{
								int fidx = (fy2 + fx) * in_depth + filterIdx;
								int Vidx = (oy2 + ox) * in_depth;
								for (int fd = 0; fd < in_depth; fd++)
								{
									// avoid function call overhead (x2) for efficiency, compromise modularity :(
									//a += filters.w[fidx + fd] * in_act.w[Vidx + fd];
									a += p_filters_w[fidx + fd] * p_in_act_w[Vidx + fd];
								}
							}
						}
					}
					//a += bias.w[d];
					a += p_bias_w[d];
					//out_act.set(out_x, out_y, d, a);
					//out_act.w[(out_act_sx_out_y + out_x) * out_act.depth + d] = a;
					p_out_act_w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
				}
			}
		}
	}
}


extern "C" __declspec(dllexport) void CVBWD(
	int stride,
	int unstride,
	int pad,
	int sx,
	int sy,
	int in_size,
	int in_depth,
	int out_sx,
	int out_sy,
	int out_depth,
	int filterSize,
	int out_act_sx,
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
		//in_act.dw[i] = 0;
		p_in_act_dw[i] = 0;
	}

	if (unstride > 0)
	{
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int x = out_x / unstride - pad;
			for (int out_y = 0; out_y < out_sy; out_y++)
			{
				int y = out_y / unstride - pad;
				for (int d = 0; d < out_depth; d++)
				{
					//Vol f = this.filters[d];
					int filterIdx = d * filterSize;
					// convolve centered at this particular location
					//float chain_grad = out_act.get_grad(out_x, out_y, d); // gradient from above, from chain rule
					float chain_grad = p_out_act_dw[((out_act_sx * out_y) + out_x) * out_depth + d];

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
	else
	{
		for (int out_x = 0; out_x < out_sx; out_x++)
		{
			int x = out_x * stride - pad;
			for (int out_y = 0; out_y < out_sy; out_y++)
			{
				int y = out_y * stride - pad;
				for (int d = 0; d < out_depth; d++)
				{
					//Vol f = this.filters[d];
					int filterIdx = d * filterSize;
					// convolve centered at this particular location
					//float chain_grad = this.out_act.get_grad(out_x, out_y, d); // gradient from above, from chain rule
					float chain_grad = p_out_act_dw[((out_act_sx * out_y) + out_x) * out_depth + d];
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
}
