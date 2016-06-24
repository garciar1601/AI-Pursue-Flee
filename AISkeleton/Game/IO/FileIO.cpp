#include <IO\FileIO.h>
#include <fstream>
#include <Qt\qdebug.h>

using std::ifstream;

std::string FileIO::file2String(const char* filePath){
	ifstream meInput(filePath);
	if(! meInput.good()){
		qDebug() << "File failed to load... " << filePath;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}
