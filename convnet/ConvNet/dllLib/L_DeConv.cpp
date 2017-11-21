/*
int in_x = blockDim.x * blockIdx.x + threadIdx.x;
int in_y = blockDim.y * blockIdx.y + threadIdx.y;
int fd = blockDim.z * blockIdx.z + threadIdx.z;

float a = 0.0f;
for (int fy = 0; fy < sy; fy++)
{
	for (int fx = 0; fx < sx; fx++)
	{
		for (int d = 0; d < out_depth; d++)
		{
			int out_y = (in_y - fy + pad) / stride;
			int out_x = (in_x - fx + pad) / stride;

			if (out_y >= 0 && out_y < out_sy && out_x >= 0 && out_x < out_sx)
			{
				if ((in_y - fy + pad) % stride == 0 && (in_x - fx + pad) % stride == 0)
				{
					float chain_grad = p_out_act_dw[((out_sx * out_y) + out_x) * out_depth + d];
					int filterIdx = d * filterSize;
					int fidx = (fy * sx + fx) * in_depth + filterIdx;

					a += p_filters_w[fidx + fd] * chain_grad;

				}
			}
		}
	}
}
p_in_act_dw[(in_y * in_sx + in_x) * in_depth + fd] = a;
*/