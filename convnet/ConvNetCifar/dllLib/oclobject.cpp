// Copyright (c) 2009-2013 Intel Corporation
// All rights reserved.
//
// WARRANTY DISCLAIMER
//
// THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
// MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Intel Corporation is the author of the Materials, and requests that all
// problem reports or change requests be submitted to it directly


#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <CL/cl.h>

#include "oclobject.hpp"
#include "basic.h"

using std::cerr;
using std::vector;



void readFile (const std::wstring& file_name, vector<char>& data)
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

    if(!file)
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

        if(!file)
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
        cerr << "Full file path is " << inquotes(dir + wstringToString(file_name)) <<"\n";
#else
        std::wstring dir = exe_dir_w();
        file.open(
            (dir + file_name).c_str(),
            ios_base::ate | ios_base::binary
        );

        if(!file)
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
        cerr << "Full file path is " << wstringToString(inquotes_w(dir + file_name)) <<"\n";
#endif

    }

    // Second, determine the file length
    std::streamoff file_length = file.tellg();

    if(file_length == -1)
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

void readProgramFile (const std::wstring& program_file_name, vector<char>& program_text_prepared)
{
    readFile (program_file_name, program_text_prepared);
    program_text_prepared.push_back(0); // terminatig zero

}

cl_program createAndBuildProgram (
    const std::vector<char>& program_text_prepared,
    cl_context context,
    size_t num_of_devices,
    const cl_device_id* devices,
    const string& build_options
)
{
    // Create OpenCL program and build it
    const char* raw_text = &program_text_prepared[0];
    cl_int err;
    // TODO Using prepared length and not terminating by 0 is better way?
    cl_program program = clCreateProgramWithSource(context, 1, &raw_text, 0, &err);
    SAMPLE_CHECK_ERRORS(err);

    err = clBuildProgram(program, (cl_uint)num_of_devices, devices, build_options.c_str(), 0, 0);

    if(err == CL_BUILD_PROGRAM_FAILURE)
    {
        for(size_t i = 0; i < num_of_devices; ++i)
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

OpenCLProgram::OpenCLProgram (
    OpenCLBasic& oclobjects,
    const std::wstring& program_file_name,
    const string& program_text,
    const string& build_options
) :
    program(0)
{
    using namespace std;

    if(!program_file_name.empty() && !program_text.empty())
    {
        throw Error(
            "Both program file name and program text are specified. "
            "Should be one of them only."
        );
    }

    if(program_file_name.empty() && program_text.empty())
    {
        throw Error(
            "Neither of program file name or program text are specified. "
            "One of them is required."
        );
    }

    assert(program_file_name.empty() + program_text.empty() == 1);

    // use vector for automatic memory management
    vector<char> program_text_prepared;

    if(!program_file_name.empty())
    {
        readProgramFile(program_file_name, program_text_prepared);
    }
    else
    {
        program_text_prepared.resize(program_text.length() + 1);  // +1 for terminating zero
        copy(program_text.begin(), program_text.end(), program_text_prepared.begin());
    }

    program = createAndBuildProgram(program_text_prepared, oclobjects.context, 1, &oclobjects.device, build_options);
}


OpenCLProgram::~OpenCLProgram ()
{
    try
    {
        if(program)
        {
            clReleaseProgram(program);
        }
    }
    catch(...)
    {
        destructorException();
    }
}

OpenCLProgramOneKernel::OpenCLProgramOneKernel (
    OpenCLBasic& oclobjects,
    const std::wstring& program_file_name,
    const string& program_text,
    const string& kernel_name,
    const string& build_options
) :
    OpenCLProgram(oclobjects, program_file_name, program_text, build_options),
    kernel(0)
{
    using namespace std;

    cl_int err = 0;
    kernel = clCreateKernel(program, kernel_name.c_str(), &err);
    SAMPLE_CHECK_ERRORS(err);
}


OpenCLProgramOneKernel::~OpenCLProgramOneKernel ()
{
    try
    {
        if(kernel)
        {
            clReleaseKernel(kernel);
        }
    }
    catch(...)
    {
        destructorException();
    }
}

OpenCLProgramMultipleKernels::OpenCLProgramMultipleKernels (
    OpenCLBasic& oclobjects,
    const std::wstring& program_file_name,
    const string& program_text,
    const string& build_options
) :
    OpenCLProgram(oclobjects, program_file_name, program_text, build_options)
{
}

OpenCLProgramMultipleKernels::~OpenCLProgramMultipleKernels ()
{
    try
    {
        for(KernelMap::iterator it = kMap.begin(); it != kMap.end() ; ++it)
        {
            cl_kernel krnl = it->second;
            if(krnl)
            {
                clReleaseKernel(krnl);
            }
        }
    }
    catch(...)
    {
        destructorException();
    }
}

cl_kernel OpenCLProgramMultipleKernels::operator[](const std::string& kernel_name)
{
    using namespace std;

    cl_kernel krnl = 0;
    KernelMap::iterator it = kMap.find(kernel_name);

    if(kMap.end() == it)    //this kernel hasn't been used yet
    {
        cl_int err = 0;
        krnl = clCreateKernel(program, kernel_name.c_str(), &err);
        SAMPLE_CHECK_ERRORS(err);

        kMap[kernel_name] = krnl;
        return krnl;
    }
    else
        return it->second;
}

cl_device_type parseDeviceType (const string& device_type_name)
{
    cl_device_type  device_type = 0;
    for(size_t pos=0,next=0; next != string::npos; pos = next+1)
    {
        next = device_type_name.find_first_of("+|",pos);
        size_t substr_len = (next!=string::npos)?(next-pos):(string::npos);
        string name = device_type_name.substr(pos,substr_len);
        if(
            name == "all" ||
            name == "ALL" ||
            name == "CL_DEVICE_TYPE_ALL"
        )
        {
            device_type |= CL_DEVICE_TYPE_ALL;
            continue;
        }

        if(
            name == "default" ||
            name == "DEFAULT" ||
            name == "CL_DEVICE_TYPE_DEFAULT"
        )
        {
            device_type |= CL_DEVICE_TYPE_DEFAULT;
            continue;
        }

        if(
            name == "cpu" ||
            name == "CPU" ||
            name == "CL_DEVICE_TYPE_CPU"
        )
        {
            device_type |= CL_DEVICE_TYPE_CPU;
            continue;
        }

        if(
            name == "gpu" ||
            name == "GPU" ||
            name == "CL_DEVICE_TYPE_GPU"
        )
        {
            device_type |= CL_DEVICE_TYPE_GPU;
            continue;
        }

        if(
            name == "acc" ||
            name == "ACC" ||
            name == "accelerator" ||
            name == "ACCELERATOR" ||
            name == "CL_DEVICE_TYPE_ACCELERATOR"
        )
        {
            device_type |= CL_DEVICE_TYPE_ACCELERATOR;
            continue;
        }

        throw Error(
            "Cannot recognize " + device_type_name + " as a device type"
        );
    }
    return device_type;
}
