#ifndef UNIFORM_INFO_H
#define UNIFORM_INFO_H

struct UniformInfo{
	void* data;
	const char* name;
	unsigned int dataType;
	UniformInfo() : data(0) {}
};

#endif