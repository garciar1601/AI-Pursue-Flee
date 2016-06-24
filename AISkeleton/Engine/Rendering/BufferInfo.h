#ifndef BUFFERINFO_H
#define BUFFERINFO_H
struct BufferInfo
{
	static const GLuint MAX_BUFFER_SIZE = 0x200000;
	
	GLuint glBufferID;
	GLuint nextAvailableByte;
	bool hasBuffer;

	BufferInfo() : hasBuffer(false), glBufferID(0), nextAvailableByte(0){}

	GLint getAvailableSize() const {return MAX_BUFFER_SIZE - nextAvailableByte; }
};

#endif