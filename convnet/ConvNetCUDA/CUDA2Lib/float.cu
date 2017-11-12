#include "cuda_runtime.h"
#include "device_launch_parameters.h"


extern "C" __declspec(dllexport) float* allocDeviceMemory(int size)
{
	float* mem;

	cudaError_t cudaStatus = cudaMalloc((void**)&mem, size * sizeof(float));
	if (cudaStatus != cudaSuccess) {
		return 0;
	}
	return mem;
}


extern "C" __declspec(dllexport) void freeDeviceMemory(float* point)
{
	cudaFree(point);
}

extern "C" __declspec(dllexport) int memcpyHostToDevice(float* hostPoint, float* devicePoint, unsigned int size)
{
	return cudaMemcpy(devicePoint, hostPoint, size * sizeof(float), cudaMemcpyHostToDevice);
}

extern "C" __declspec(dllexport) int memcpyDeviceToHost(float* devicePoint, float* hostPoint, unsigned int size)
{
	return cudaMemcpy(hostPoint, devicePoint, size * sizeof(float), cudaMemcpyDeviceToHost);
}

