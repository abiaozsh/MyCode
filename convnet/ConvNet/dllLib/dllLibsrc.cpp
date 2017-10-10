#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <CL/cl.h>

#include "basic.h"
//#include "oclobject.hpp"

int testval = 0;

using namespace std;
using std::vector;


struct OpenCLBasic
{
public:
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
};

cl_int arraySize = 1024 * 1024;
extern "C" __declspec(dllexport) int getArraySize()//int size
{
	return arraySize;
}

cl_float* p_input = NULL;
extern "C" __declspec(dllexport) cl_float* allocfloat()//int size
{
	//extern __m256  __cdecl _mm256_load_ps(float const *);
	//extern void    __cdecl _mm256_store_ps(float *, __m256);

	//cl_float* p_input = NULL;

	cl_uint dev_alignment = 4096;// zeroCopyPtrAlignment(oclobjects.device);
	size_t aligned_size = zeroCopySizeAlignment(sizeof(cl_float)* arraySize);

	p_input = (cl_float*)aligned_malloc(aligned_size, dev_alignment);

	return p_input;
}

char* _print(char* str, int val){
	char a[64];
	sprintf(a, "%d", val);
	char* b = a;
	while ((*b) != 0){
		*str = *b;
		b++;
		str++;
	}
	return str;
}
char* _print(char* str, const char* val){
	const char* b = val;
	while ((*b) != 0){
		*str = *b;
		b++;
		str++;
	}
	return str;
}
char* _print(char* str, char* val){
	char* b = val;
	while ((*b) != 0){
		*str = *b;
		b++;
		str++;
	}
	return str;
}

extern "C" __declspec(dllexport) char* _getPlatforms()
{
	char* mem = (char*)malloc(4096);
	char* ret = mem;

	cl_uint num_of_platforms = 0;
	clGetPlatformIDs(0, 0, &num_of_platforms);

	ret = _print(ret, "platforms:[");
	ret = _print(ret, num_of_platforms);
	ret = _print(ret, "],\r\n");


	vector<cl_platform_id> platforms(num_of_platforms);
	clGetPlatformIDs(num_of_platforms, &platforms[0], 0);

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{
		size_t platform_name_length = 0;
		clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			0,
			&platform_name_length
			);

		vector<char> platform_name(platform_name_length);
		clGetPlatformInfo(
			platforms[i],//cl_platform_id    platform ,
			CL_PLATFORM_NAME,//cl_platform_info  param_name ,
			platform_name_length,//size_t            param_value_size ,
			&platform_name[0],//void *            param_value ,
			0//size_t *          param_value_size_ret 
			);

		ret = _print(ret, "platform");
		ret = _print(ret, i);
		ret = _print(ret, ":[");
		ret = _print(ret, &platform_name[0]);
		ret = _print(ret, "],\r\n");
	}

	*ret = 0;
	return mem;
}


extern "C" __declspec(dllexport) wchar_t* testsss()//int size
{
	wchar_t* mem = (wchar_t*)malloc(4096);
	wchar_t* ret = mem;

	mem[0] = '\4f60';
	mem[1] = '\597d';
	mem[2] = 'a';
	mem[3] = 'b';
	mem[4] = '\0';

	return mem;
}

extern "C" __declspec(dllexport) char* _getDevices(int platform)
{
	char* mem = (char*)malloc(4096);
	char* ret = mem;

	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	clGetPlatformIDs(0, 0, &num_of_platforms);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	clGetPlatformIDs(num_of_platforms, &platforms[0], 0);

	cl_device_type  device_type = 0;
	device_type |= CL_DEVICE_TYPE_ALL;

	cl_uint num_of_devices = 0;
	clGetDeviceIDs(
		platforms[platform],
		device_type,
		0,
		0,
		&num_of_devices
		);


	ret = _print(ret, "devices:[");
	ret = _print(ret, num_of_devices);
	ret = _print(ret, "],\r\n");

	vector<cl_device_id> devices(num_of_devices);

	clGetDeviceIDs(
		platforms[platform],
		device_type,
		num_of_devices,
		&devices[0],
		0
		);

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		// Get the length for the i-th device name
		size_t device_name_length = 0;
		clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			0,
			0,
			&device_name_length
			);

		// Get the name itself for the i-th device
		// use vector for automatic memory management
		vector<char> device_name(device_name_length);
		clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			device_name_length,
			&device_name[0],
			0
			);

		ret = _print(ret, "device");
		ret = _print(ret, i);
		ret = _print(ret, ":[");
		ret = _print(ret, &device_name[0]);
		ret = _print(ret, "],\r\n");

	}

	*ret = 0;
	return mem;
}


cl_platform_id selectPlatform(cl_uint selected_platform_index)
{
	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	clGetPlatformIDs(0, 0, &num_of_platforms);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	clGetPlatformIDs(num_of_platforms, &platforms[0], 0);

	return platforms[selected_platform_index];
}

cl_device_id selectDevice(OpenCLBasic* oclobjects, cl_uint selected_device_index)//, const string& device_type_name
{

	// List devices of a given type only
	//cl_device_type device_type = parseDeviceType("all");
	cl_device_type  device_type = 0;
	device_type |= CL_DEVICE_TYPE_ALL;

	cl_uint num_of_devices = 0;
	clGetDeviceIDs(
		(*oclobjects).platform,
		device_type,
		0,
		0,
		&num_of_devices
		);

	vector<cl_device_id> devices(num_of_devices);

	clGetDeviceIDs(
		(*oclobjects).platform,
		device_type,
		num_of_devices,
		&devices[0],
		0
		);

	return devices[selected_device_index];
}

cl_context createContext(OpenCLBasic* oclobjects)//, const cl_context_properties* additional_context_props
{

	size_t number_of_additional_props = 0;
	//if (additional_context_props)
	//{
	//	// count all additional props including terminating 0
	//	while (additional_context_props[number_of_additional_props++]);
	//	number_of_additional_props--;   // now exclude terminating 0
	//}

	// allocate enough space for platform and all additional props if any
	//std::vector<cl_context_properties> context_props(
	//	2 + // for CL_CONTEXT_PLATFORM and platform itself
	//	number_of_additional_props +
	//	1   // for terminating zero
	//	);

	// allocate enough space for platform and all additional props if any
	std::vector<cl_context_properties> context_props(
		2 + // for CL_CONTEXT_PLATFORM and platform itself
		number_of_additional_props +
		1   // for terminating zero
		);

	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = cl_context_properties((*oclobjects).platform);


	context_props.back() = 0;

	cl_int err = 0;
	//clCreateContext(const cl_context_properties * /* properties */,
	//	cl_uint                 /* num_devices */,
	//	const cl_device_id *    /* devices */,
	//	void (CL_CALLBACK * /* pfn_notify */)(const char *, const void *, size_t, void *),
	//	void *                  /* user_data */,
	//	cl_int *                /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

	cl_context context = clCreateContext(&context_props[0], 1, &(oclobjects->device), 0, 0, &err);
	string s = opencl_error_to_str(err);
	return context;
}

cl_command_queue createQueue(OpenCLBasic* oclobjects, cl_command_queue_properties queue_properties)
{
	using namespace std;

	cl_int err = 0;
	cl_command_queue queue = clCreateCommandQueue((*oclobjects).context, (*oclobjects).device, queue_properties, &err);
	//SAMPLE_CHECK_ERRORS(err);
	return queue;
}

OpenCLBasic oclobjects;

extern "C" __declspec(dllexport) void init(int Platform, int Device)//0,1
{
	oclobjects.platform = selectPlatform(Platform);
	oclobjects.device = selectDevice(&oclobjects, Device);
	oclobjects.context = createContext(&oclobjects);
	oclobjects.queue = createQueue(&oclobjects, 0);

	testval = 12;
}

extern "C" __declspec(dllexport) void Final()
{
	clReleaseCommandQueue(oclobjects.queue);
	clReleaseContext(oclobjects.context);
	//clReleaseDevice(cl_device_id ) CL_API_SUFFIX__VERSION_1_2;

}


cl_program _createAndBuildProgram(
	const std::vector<char>& program_text_prepared,
	cl_context context,
	size_t num_of_devices,
	const cl_device_id* devices,
	const string& build_options
	)
{
	int ss = testval;
	// Create OpenCL program and build it
	const char* raw_text = &program_text_prepared[0];
	cl_int err;
	// TODO Using prepared length and not terminating by 0 is better way?
	cl_program program = clCreateProgramWithSource(context, 1, &raw_text, 0, &err);
	SAMPLE_CHECK_ERRORS(err);

	err = clBuildProgram(program, (cl_uint)num_of_devices, devices, build_options.c_str(), 0, 0);

	if (err == CL_BUILD_PROGRAM_FAILURE)
	{
		for (size_t i = 0; i < num_of_devices; ++i)
		{
			size_t log_length = 0;
			err = clGetProgramBuildInfo(
				program,
				devices[i],
				CL_PROGRAM_BUILD_LOG,
				0,
				0,
				&log_length
				);
			SAMPLE_CHECK_ERRORS(err);

			vector<char> log(log_length);

			err = clGetProgramBuildInfo(
				program,
				devices[i],
				CL_PROGRAM_BUILD_LOG,
				log_length,
				&log[0],
				0
				);
			SAMPLE_CHECK_ERRORS(err);

			throw Error(
				"Error happened during the build of OpenCL program.\n"
				"Build log:\n" +
				string(&log[0])
				);
		}
	}

	SAMPLE_CHECK_ERRORS(err);

	return program;
}


void readFile(const std::wstring& file_name, vector<char>& data)
{
	using namespace std;

	// Read program from a file

	// First, determine where file exists; look at two places:
	//   - current/default directory; also suitable for full paths
	//   - directory where executable is placed
#ifdef __linux__
	//Store current locale and set default locale
	CTYPELocaleHelper locale_helper;

	ifstream file(
		wstringToString(file_name).c_str(),
		ios_base::ate | ios_base::binary
		);
#else
	ifstream file(
		file_name.c_str(),
		ios_base::ate | ios_base::binary
		);
#endif

	if (!file)
	{
		// There are no file at current/default directory or absolute
		// path. Try to open it relatively from the directory where
		// executable binary is placed.


		cerr
			<< "[ WARNING ] Unable to load OpenCL source code file "
			<< inquotes(wstringToString(file_name)) << " at "
			<< "the default location.\nTrying to open the file "
			<< "from the directory with executable...";

		file.clear();

#ifdef __linux__
		std::string dir = exe_dir();
		file.open(
			(dir + wstringToString(file_name)).c_str(),
			ios_base::ate | ios_base::binary
			);

		if (!file)
		{
			cerr << " FAILED\n";
			throw Error(
				"Cannot open file " + inquotes(dir + wstringToString(file_name))
				);
		}
		else
		{
			cerr << " OK\n";
		}
		cerr << "Full file path is " << inquotes(dir + wstringToString(file_name)) << "\n";
#else
		std::wstring dir = exe_dir_w();
		file.open(
			(dir + file_name).c_str(),
			ios_base::ate | ios_base::binary
			);

		if (!file)
		{
			cerr << " FAILED\n";
			throw Error(
				"Cannot open file " + wstringToString(dir + file_name)
				);
		}
		else
		{
			cerr << " OK\n";
		}
		cerr << "Full file path is " << wstringToString(inquotes_w(dir + file_name)) << "\n";
#endif

	}

	// Second, determine the file length
	std::streamoff file_length = file.tellg();

	if (file_length == -1)
	{
		throw Error(
			"Cannot determine the length of file " +
			wstringToString(inquotes_w(file_name))
			);
	}

	file.seekg(0, ios_base::beg);   // go to the file beginning
	data.resize(static_cast<size_t>(file_length));
	file.read(&data[0], file_length);
}

void readProgramFile(const std::wstring& program_file_name, vector<char>& program_text_prepared)
{
	readFile(program_file_name, program_text_prepared);
	program_text_prepared.push_back(0); // terminatig zero

}

cl_program getProgram(OpenCLBasic& oclobjects, const std::wstring& program_file_name)
{
	cl_program program;

	const string& program_text = "";
	const string& build_options = "";

	// use vector for automatic memory management
	vector<char> program_text_prepared;

	if (!program_file_name.empty())
	{
		readProgramFile(program_file_name, program_text_prepared);
	}
	else
	{
		program_text_prepared.resize(program_text.length() + 1);  // +1 for terminating zero
		copy(program_text.begin(), program_text.end(), program_text_prepared.begin());
	}

	program = _createAndBuildProgram(program_text_prepared, oclobjects.context, 1, &oclobjects.device, build_options);
	return program;
}


cl_kernel _CreateKernel(cl_program program, const string& kernel_name)
{
	using namespace std;

	cl_int err = 0;
	cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &err);

	return kernel;
}


extern "C" __declspec(dllexport) cl_kernel getKernel(const wchar_t* filename,const char* skernel)
{
	//kernel = clCreateKernel(program, kernel_name.c_str(), &err);

	//OpenCLProgramOneKernel executable(temp, L"ConvNet.cl", "", "test1");
	cl_program program = getProgram(oclobjects, filename);//L"ConvNet.cl"

	cl_kernel kernel = _CreateKernel(program, skernel);//, "", "test1"

	return kernel;
}

extern "C" __declspec(dllexport) void runKernel(cl_kernel kernel, int threads, int _params, int* param)
{
	int arraySize = 64;

	cl_int err = CL_SUCCESS;
	cl_mem cl_input_buffer =
		clCreateBuffer
		(
		oclobjects.context,
		CL_MEM_USE_HOST_PTR,
		zeroCopySizeAlignment(sizeof(cl_int)* arraySize, oclobjects.device),
		p_input,
		&err
		);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_input_buffer);

	for (int i = 0; i < _params; i++){
		clSetKernelArg(kernel, 1 + i, sizeof(cl_uint), (void *)&param[i]);
	}

	size_t global_work_size[1] = { threads };

	clEnqueueNDRangeKernel(
		oclobjects.queue,
		kernel,
		1, /* work_dim */
		NULL, /* global_work_offset */
		global_work_size, /* global_work_size */
		NULL, /* local_work_size */
		0, /* num_events_in_wait_list */
		NULL, /* event_wait_list */
		NULL);/* event */


	clFinish(oclobjects.queue);


	void* tmp_ptr = NULL;
	tmp_ptr = clEnqueueMapBuffer(oclobjects.queue, cl_input_buffer, true, CL_MAP_READ, 0, sizeof(cl_int)* arraySize, 0, NULL, NULL, &err);
	SAMPLE_CHECK_ERRORS(err);
	if (tmp_ptr != p_input)
	{
		throw Error("clEnqueueMapBuffer failed to return original pointer\n");
	}

	err = clFinish(oclobjects.queue);
	SAMPLE_CHECK_ERRORS(err);

	err = clEnqueueUnmapMemObject(oclobjects.queue, cl_input_buffer, tmp_ptr, 0, NULL, NULL);
	SAMPLE_CHECK_ERRORS(err);

	err = clReleaseMemObject(cl_input_buffer);
	SAMPLE_CHECK_ERRORS(err);


	//clReleaseKernel(kernel);
}



/*#include <xmmintrin.h>
#include <emmintrin.h>
#include <windows.h>
#include <process.h>
#include <immintrin.h>//AVX(include wmmintrin.h)

extern "C" __declspec(dllexport) void test(float* arry1, float* arry2)
{
//extern __m256  __cdecl _mm256_load_ps(float const *);
//extern void    __cdecl _mm256_store_ps(float *, __m256);

__m256 a = _mm256_loadu_ps(arry1);
__m256 b = _mm256_loadu_ps(arry2);
a = _mm256_add_ps(a,b);
_mm256_storeu_ps(arry1, a);

}


extern "C" __declspec(dllexport) void fwd(int out_depth, int num_inputs, float* A_w, float* V_w, float* filters_w, float* biases_w)
{
for (int i = 0; i < out_depth; i++)
{
float a = 0.0f;
int iw = i * num_inputs;
for (int d = 0; d < num_inputs; d++)
{
a += V_w[d] * filters_w[iw + d]; // for efficiency use Vols directly for now
}
a += biases_w[i];
A_w[i] = a;
}
}
*/

/*
DWORD WINAPI sin_part4T(LPVOID lpParam){
ArgList* argList = (ArgList*)lpParam;

int limit = argList->Precision;

while(true)
{

for(int idx = 0; idx < argList->Precision; idx++)
{
argList->num2[idx] = argList->num_a[idx];
}
argList->Snum2 = argList->Snum_a;
argList->Enum2 = argList->Enum_a;

//num2=a/b
divi(limit, argList->num2, &argList->Snum2, &argList->Enum2, argList->num_b, argList->Snum_b, argList->Enum_b, argList->r_value, argList->b);

//result+=num2
add(argList->Precision, argList->Num_Array, &argList->Sign, &argList->Exponent, argList->num2, argList->Snum2, argList->Enum2);

limit = argList->Precision + argList->Enum2;

if (limit < 1)
{
break;
}

if(limit > argList->Precision) limit = argList->Precision;
if(limit <= 0) limit = 1;

//a=a*x^8
mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x8, argList->Snum_x8, argList->Enum_x8, argList->b);

//b=b*(i)*(i+1)*(i+2)*(i+3)*(i+4)*(i+5)*(i+6)*(i+7)
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
//i++
incr(argList->num_i, &argList->Enum_i);
//b=b*i
mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
}

argList->flag = 1;

SetEvent(*(argList->doneEvent));

return 0;
}




extern "C" __declspec(dllexport) int calc(int* arry,int wid,int hei,double viswid,double vishei,double visx,double visy)
{
HANDLE* doneEventList;
doneEventList = new HANDLE[4];

::CreateThread(NULL, 0, sin_part4T, argList3, 0, NULL);
::CreateThread(NULL, 0, sin_part4T, argList5, 0, NULL);
::CreateThread(NULL, 0, sin_part4T, argList7, 0, NULL);
::CreateThread(NULL, 0, sin_part4T, argList9, 0, NULL);

WaitForMultipleObjects(4, doneEventList, TRUE,INFINITE);

add(Precision, Num_Array, pSign, pExponent, argList3->Num_Array, -argList3->Sign, argList3->Exponent); //负半
add(Precision, Num_Array, pSign, pExponent, argList5->Num_Array,  argList5->Sign, argList5->Exponent);
add(Precision, Num_Array, pSign, pExponent, argList7->Num_Array, -argList7->Sign, argList7->Exponent); //负半
add(Precision, Num_Array, pSign, pExponent, argList9->Num_Array,  argList9->Sign, argList9->Exponent);

::CloseHandle(doneEventList[0]);
::CloseHandle(doneEventList[1]);
::CloseHandle(doneEventList[2]);
::CloseHandle(doneEventList[3]);

delete doneEventList;

double dReaC;
double dImaC;
double dImaC2;

int maxi = 255;
double maxm = 1.7976931348623158e+308;

dReaC = (viswid) / ((double)wid - 1)*2;
dImaC = (vishei) / ((double)hei - 1);
dImaC2 = dImaC*2;

ArgList* argList = new ArgList();
InitializeCriticalSection(&(argList->cs1));
{
argList->flag = 0;
argList->arry = arry;
argList->wid = wid;
argList->hei = hei;
argList->viswid = viswid;
argList->vishei = vishei;
argList->visx = visx;
argList->visy = visy;
argList->dReaC = dReaC;
argList->dImaC = dImaC;
argList->dImaC2 = dImaC2;
argList->maxi = maxi;
}

_beginthread(calc_part1, 0, argList);

int index=0;
double ReaC = visx;

for(int i = 0 ; i < wid ; i+=2 , ReaC += dReaC)
{
__m128d ReaC2;ReaC2.m128d_f64[0]=ReaC;ReaC2.m128d_f64[1]=ReaC;

double ImaC = visy;
for(int j = 0; j < hei ; j+=2 , ImaC += dImaC2)
{
__m128d ReaZ;ReaZ.m128d_f64[0]=0;ReaZ.m128d_f64[1]=0;
__m128d ImaZ;ImaZ.m128d_f64[0]=0;ImaZ.m128d_f64[1]=0;
__m128d ReaZ2;ReaZ2.m128d_f64[0]=0;ReaZ2.m128d_f64[1]=0;
__m128d ImaZ2;ImaZ2.m128d_f64[0]=0;ImaZ2.m128d_f64[1]=0;
__m128d num2;num2.m128d_f64[0]=2;num2.m128d_f64[1]=2;
__m128d ImaC2;ImaC2.m128d_f64[0]=ImaC;ImaC2.m128d_f64[1]=ImaC+dImaC;

int clr1 = 1;
int clr2 = 1;

while(true)
{
__m128d result = _mm_add_pd(ReaZ2, ImaZ2);
int res1 = result.m128d_f64[0] < maxm ? 1 : 0;
int res2 = result.m128d_f64[1] < maxm ? 1 : 0;

if(clr1 < maxi && res1 || clr2 < maxi && res2)
{
ReaZ2 = _mm_mul_pd(ReaZ, ReaZ);
ImaZ2 = _mm_mul_pd(ImaZ, ImaZ);
ImaZ = _mm_add_pd(_mm_mul_pd(_mm_mul_pd(num2, ImaZ), ReaZ), ImaC2);
ReaZ = _mm_add_pd(_mm_sub_pd(ReaZ2, ImaZ2), ReaC2);
clr1+=res1;
clr2+=res2;
}
else
{
break;
}
}

*(arry + index++) = clr1;
*(arry + index++) = clr2;
}
index+=wid;
}

while(argList->flag==0)
{
Sleep(10);

EnterCriticalSection(&(argList->cs1));

LeaveCriticalSection(&(argList->cs1));
}

DeleteCriticalSection(&(argList->cs1));

delete argList;

return 0;
}

*/