
void* aligned_malloc(size_t size, size_t alignment)
{

	if (alignment < sizeof(void*))
	{
		alignment = sizeof(void*);
	}

	// allocate extra memory and convert to size_t to perform calculations
	char* orig = new char[size + alignment + sizeof(void*)];
	// calculate an aligned position in the allocated region
	// assumption: (size_t)orig does not lose lower bits
	char* aligned =
		orig + (
		(((size_t)orig + alignment + sizeof(void*)) & ~(alignment - 1)) -
		(size_t)orig
		);
	// save the original pointer to use it in aligned_free
	*((char**)aligned - 1) = orig;
	return aligned;
}

size_t zeroCopySizeAlignment(size_t requiredSize)
{
	// Please refer to Intel Zero Copy Tutorial and OpenCL Performance Guide
	// The following statement rounds requiredSize up to the next 64-byte boundary
	return requiredSize + (~requiredSize + 1) % 64;   // or even shorter: requiredSize + (-requiredSize) % 64
}


void aligned_free(void *aligned)
{
	if (!aligned)return; // behaves as delete: calling with 0 is NOP
	delete[] * ((char**)aligned - 1);
}




extern "C" __declspec(dllexport) float* allocfloat(int size)
{
	//extern __m256  __cdecl _mm256_load_ps(float const *);
	//extern void    __cdecl _mm256_store_ps(float *, __m256);

	//cl_float* p_input = NULL;

	int dev_alignment = 64;// zeroCopyPtrAlignment(oclobjects.device);
	//cl_uint dev_alignment = 256;// zeroCopyPtrAlignment(oclobjects.device);
	size_t aligned_size = zeroCopySizeAlignment(sizeof(float)* size);

	float* p_input = (float*)aligned_malloc(aligned_size, dev_alignment);

	return p_input;
}

extern "C" __declspec(dllexport) void freefloat(float* point)
{
	aligned_free(point);
}
