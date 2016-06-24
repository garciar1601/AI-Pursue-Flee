#ifndef FILEIO_H
#define FILEIO_H
#include <string>
class FileIO{
public:
	static std::string file2String(const char* filePath);
};

#endif