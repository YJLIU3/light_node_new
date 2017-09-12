#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <CL\cl.h>
using namespace std;

#define C 3*10^8

float x = 0;
float y = 0;
int nodes = 0;
int number_of_location = nodes*(nodes - 1)*(nodes - 2) / 6;


bool GetFileData(const char* fname, string& str)
{
	FILE* fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("no found file\n");
		return false;
	}

	int n = 0;
	while (feof(fp) == 0)
	{
		str += fgetc(fp);
	}

	return true;
}

int CL_init()
{
	string code_file;

	if (false == GetFileData("combination.cl", code_file))
		return 0;

	char* buf_code = new char[code_file.size()];
	strcpy(buf_code, code_file.c_str());
	buf_code[code_file.size() - 1] = NULL;

	//声明CL所需变量。
	cl_device_id device;
	cl_platform_id platform_id = NULL;
	cl_context context;
	cl_command_queue cmdQueue;
	cl_mem buffer_input, buffer_output;
	cl_program program;
	cl_kernel kernel = NULL;

	size_t globalWorkSize[1];
	globalWorkSize[0] = number_of_location;
	
	cl_int err;

	int *buf_locat_element = new int[number_of_location];
	int *buf_out_element = new int[number_of_location];

	size_t datasize = sizeof(int)*number_of_location;
	for (int i = 0; i < number_of_location; i++)
	{
		buf_locat_element[i] = i;
	}

	err = clGetPlatformIDs(1, &platform_id, NULL);

	if (err != CL_SUCCESS)
	{
		printf("clGetPlatformIDs error\n"); 
		return 0;
	}
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	cmdQueue = clCreateCommandQueue(context, device, 0, NULL);
	buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, NULL);
	buffer_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY,3*number_of_location, NULL, NULL);

	//step 5:将数据上传到缓冲区
	clEnqueueWriteBuffer(cmdQueue, buffer_input, CL_FALSE, 0, datasize, buf_locat_element, 0, NULL, NULL);

	//step 6:加载编译代码,创建内核调用函数

	program = clCreateProgramWithSource(context, 1,(const char**)&buf_code,NULL, NULL);

	clBuildProgram(program, 1, &device, NULL, NULL, NULL);
}



