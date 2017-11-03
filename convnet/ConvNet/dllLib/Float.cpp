#include <CL/cl.h>
#include "basic.h"

extern "C" __declspec(dllexport) cl_float* allocfloat(int size)
{
	//extern __m256  __cdecl _mm256_load_ps(float const *);
	//extern void    __cdecl _mm256_store_ps(float *, __m256);

	//cl_float* p_input = NULL;

	cl_uint dev_alignment = 4096;// zeroCopyPtrAlignment(oclobjects.device);
	size_t aligned_size = zeroCopySizeAlignment(sizeof(cl_float)* size);

	cl_float* p_input = (cl_float*)aligned_malloc(aligned_size, dev_alignment);

	return p_input;
}

extern "C" __declspec(dllexport) void freefloat(cl_float* point)
{
	aligned_free(point);
}

extern "C" __declspec(dllexport) cl_mem allocCLMEM(OpenCLBasic* oclobjects, cl_float* point, int size){
	cl_int err = CL_SUCCESS;
	cl_mem p_cl_mem =
		clCreateBuffer
		(
		oclobjects->context,
		CL_MEM_USE_HOST_PTR,
		zeroCopySizeAlignment(sizeof(cl_int)* size, oclobjects->device),
		point,
		&err
		);
	return p_cl_mem;
}

extern "C" __declspec(dllexport) void freeCLMEM(OpenCLBasic* oclobjects, cl_float* point, cl_mem p_cl_mem)
{
	clEnqueueUnmapMemObject(oclobjects->queue, p_cl_mem, point, 0, NULL, NULL);

	clReleaseMemObject(p_cl_mem);
}

extern "C" __declspec(dllexport) void copyToCLMEM(OpenCLBasic* oclobjects, const cl_float* point, cl_mem p_cl_mem, int size)
{
	//extern CL_API_ENTRY cl_int CL_API_CALL
	//	clEnqueueWriteBuffer(cl_command_queue   /* command_queue */,
	//		cl_mem             /* buffer */,
	//		cl_bool            /* blocking_write */,
	//		size_t             /* offset */,
	//		size_t             /* size */,
	//		const void *       /* ptr */,
	//		cl_uint            /* num_events_in_wait_list */,
	//		const cl_event *   /* event_wait_list */,
	//		cl_event *         /* event */) CL_API_SUFFIX__VERSION_1_0;
	clEnqueueWriteBuffer(
		oclobjects->queue,
		p_cl_mem,
		true,//完成后再返回
		0,
		sizeof(cl_int)* size,
		point,
		0,
		NULL,
		NULL
		);
	clFinish(oclobjects->queue);
}

extern "C" __declspec(dllexport) void copyFromCLMEM(OpenCLBasic* oclobjects, cl_float* point, cl_mem p_cl_mem, int size)
{
	//extern CL_API_ENTRY cl_int CL_API_CALL
	//	clEnqueueReadBuffer(cl_command_queue    /* command_queue */,
	//		cl_mem              /* buffer */,
	//		cl_bool             /* blocking_read */,
	//		size_t              /* offset */,
	//		size_t              /* size */,
	//		void *              /* ptr */,
	//		cl_uint             /* num_events_in_wait_list */,
	//		const cl_event *    /* event_wait_list */,
	//		cl_event *          /* event */) CL_API_SUFFIX__VERSION_1_0;
	clEnqueueReadBuffer(
		oclobjects->queue,
		p_cl_mem,
		true,//完成后再返回
		0,
		sizeof(cl_int)* size,
		point,
		0,
		NULL,
		NULL
		);
	clFinish(oclobjects->queue);
}
