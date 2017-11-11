
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

extern "C" __declspec(dllexport) int init(int device)
{
	return cudaSetDevice(device);
}