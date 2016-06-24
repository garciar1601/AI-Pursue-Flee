#define GLM_FORCE_RADIANS
#include <Rendering\Renderer.h>
#include <Qt\qdebug.h>
#include <fstream>
#include <gtx\transform.hpp>
#include <Rendering\UniformDataTypes.h>

using std::ifstream;
using std::string;

Renderer* Renderer::instance = 0;

bool Renderer::initialize(){
	if(instance != 0){
		return false;
	}
	instance = new Renderer;
	instance->show();
	return true;
}

void Renderer::initializeGL(){
	glewInit();
	setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	numRenderables = 0;
	numPassInfos = 0;
	numTextureInfos = 0;
}

bool Renderer::shutdown(){
	if(instance == 0){
		return false;
	}
	delete instance;
	instance = 0;
	return true;
}

GeometryInfo* Renderer::addGeometry(
	void* verts, GLuint vertexDataSize, 
	void* indices, GLuint numIndices, GLuint indexDataSize,
	GLuint indiceDataType, GLuint indexingMode, VertexLayoutInfo* vertexLayoutInfo)
{
	GeometryInfo* ret = getAvailableGeometryInfo();

	BufferInfo* vertexBuffer = getAvailableBuffer(vertexDataSize);
	sendDataToBuffer(vertexBuffer, verts, vertexDataSize);
	GLuint vertexByteOffset = vertexBuffer->nextAvailableByte;
	vertexBuffer->nextAvailableByte += vertexDataSize;

	BufferInfo* indexBuffer = getAvailableBuffer(indexDataSize);
	sendDataToBuffer(indexBuffer, indices, indexDataSize);;
	GLuint indexByteOffset = indexBuffer->nextAvailableByte;
	indexBuffer->nextAvailableByte += indexDataSize;

	ret->vertexArrayObjectID = setupVertexArrayObject(
		vertexLayoutInfo, vertexBuffer->glBufferID,
		vertexByteOffset, indexBuffer->glBufferID);

	ret->indexByteOffset = indexByteOffset;
	ret->indexingMode = indexingMode;
	ret->isAvailable = false;
	ret->numIndices = numIndices;
	ret->indiceDataType = indiceDataType;

	return ret;
}

GeometryInfo* Renderer::getAvailableGeometryInfo(){
	static int currentSlot = 0;

	int i = currentSlot;

	if(i < MAX_GEOMETRIES){
		currentSlot++;
		return &geometries[i];
	}
	qDebug() << "No available geometry slots...";
	exit(1);
}

BufferInfo* Renderer::getAvailableBuffer(GLsizeiptr neededSize){
	BufferInfo* ret = findBufferWithEnoughRoom(neededSize);
	if(ret != 0){
		return ret;
	}
	return allocateNewBuffer();
}

BufferInfo* Renderer::findBufferWithEnoughRoom(GLsizeiptr neededSize){
	for(unsigned int i = 0; i < MAX_BUFFER_INFOS; i++){
		if(buffers[i].hasBuffer && buffers[i].getAvailableSize() >= neededSize){
			return buffers + i;
		}
	}
	return 0;
}

BufferInfo* Renderer::allocateNewBuffer(){
	BufferInfo* b = findUnusedBuffer();

	glGenBuffers(1, &b->glBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, b->glBufferID);
	glBufferData(GL_ARRAY_BUFFER, BufferInfo::MAX_BUFFER_SIZE, 0, GL_STATIC_DRAW);
	b->hasBuffer = true;
	b->nextAvailableByte = 0;

	return b;
}

BufferInfo* Renderer::findUnusedBuffer(){
	for(unsigned int i = 0; i < MAX_BUFFER_INFOS; i++){
		if(!buffers[i].hasBuffer){
			return buffers + i;
		}
	}
	qDebug() << "Error, failed to find available buffer...";
	exit(1);
}

void Renderer::sendDataToBuffer(BufferInfo* buffer, void* data, GLsizeiptr size){
	glBindBuffer(GL_ARRAY_BUFFER, buffer->glBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, buffer->nextAvailableByte, size, data);
}

ShaderInfo* Renderer::getAvailableShaderInfo(){
	static int currentSlot = 0;

	int i = currentSlot;

	if(i < MAX_SHADER_INFOS){
		currentSlot++;
		return &shaders[i];
	}

	qDebug() << "No available shader info slots...";
	exit(1);
}

Renderable* Renderer::getAvailableRenderable(){
	static int currentSlot = 0;

	int i = currentSlot;

	if(i < MAX_RENDERABLES){
		currentSlot++;
		return &renderables[i];
	}
	qDebug() << "No available renderable slots...";
	exit(1);
}

ShaderInfo* Renderer::addShader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath){
	ShaderInfo* ret = getAvailableShaderInfo();

	ret->vertexShaderID = compileShader(vertexShaderFilePath, GL_VERTEX_SHADER);
	ret->fragmentShaderID = compileShader(fragmentShaderFilePath, GL_FRAGMENT_SHADER);

	checkCompileStatus(ret->vertexShaderID);
	checkCompileStatus(ret->fragmentShaderID);

	ret->programID = linkProgram(ret->vertexShaderID, ret->fragmentShaderID);

	return ret;
}

GLuint Renderer::compileShader(const char* code, GLenum shaderType){
	GLuint ret = glCreateShader(shaderType);

	const char* adaptor[1];
	adaptor[0] = code;
	glShaderSource(ret, 1, adaptor, 0);
	glCompileShader(ret);

	return ret;
}
	
void Renderer::checkCompileStatus(GLuint shaderID){
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE){
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLint shaderType;
		glGetShaderiv(shaderID, GL_SHADER_TYPE, &shaderType);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID, logLength, &bitBucket, buffer);
		qDebug() << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile error..." << buffer;
		delete [] buffer;
		exit(0);
	}
}
	
GLuint Renderer::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID){
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	return programID;
}
	
void Renderer::checkLinkStatus(GLuint programID){
	GLint compileStatus;
	glGetProgramiv(programID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE){
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetProgramInfoLog(programID, logLength, &bitBucket, buffer);
		qDebug() << "Program failed to link..." << buffer;
		delete [] buffer;
		exit(0);
	}
}

Renderable* Renderer::addRenderable(GeometryInfo* what, ShaderInfo* how, const glm::mat4& where, TextureInfo* texture){
	Renderable* ret = getAvailableRenderable();

	ret->what = what;
	ret->how = how;
	ret->where = where;
	ret->texture = texture;

	numRenderables++;

	return ret;
}

VertexLayoutInfo* Renderer::getAvailableVertexLayoutInfo(){
	for(unsigned int i = 0; i < MAX_VERTEX_LAYOUT_INFOS; i++){
		if(vertexLayouts[i].isAvailable){
			return vertexLayouts + i;
		}
	}
	ASSERT(false);
	return 0;
}

VertexLayoutInfo* Renderer::addVertexLayoutInfo(GLuint* sizes, GLuint numAttributes, GLuint stride){
	VertexLayoutInfo* ret = getAvailableVertexLayoutInfo();
	ASSERT(numAttributes <= VertexLayoutInfo::MAX_VERTEX_ATTRIBUTES);
	memcpy(ret->attributeSizes, sizes, numAttributes * sizeof(*sizes));
	ret->numAttributes = numAttributes;
	ret->stride = stride;
	ret->isAvailable = false;
	return ret;
}

GLuint Renderer::setupVertexArrayObject(VertexLayoutInfo* vertexLayoutInfo, GLuint vertexBufferID, GLuint vertexBufferDataOffset, GLuint indexBufferID){
	GLuint ret;
	glGenVertexArrays(1, &ret);
	glBindVertexArray(ret);
	GLuint currentOffset = 0;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	for(unsigned int j = 0; j < vertexLayoutInfo->numAttributes; j++){
		glEnableVertexAttribArray(j);
		glVertexAttribPointer(j, vertexLayoutInfo->attributeSizes[j],
		GL_FLOAT, GL_FALSE, vertexLayoutInfo->stride,
		(void*)(vertexBufferDataOffset + currentOffset));
		currentOffset += vertexLayoutInfo->attributeSizes[j] *sizeof(float);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBindVertexArray(0);
	return ret;
}

UniformInfo* Renderer::addUniformToRenderable(void* uniformData, const char* uniformName, GLuint dataType, Renderable* renderable){
	ASSERT(renderable->numUniforms < renderable->MAX_UNIFORM_INFOS);
	UniformInfo* ret = renderable->uniformInfos + renderable->numUniforms++;
	ret->data = uniformData;
	ret->name = uniformName;
	ret->dataType = dataType;
	return ret;
}

void Renderer::setupUniforms(Renderable* renderable){
	for(unsigned int i = 0; i < renderable->numUniforms; i++){
		GLint uniformLocation = glGetUniformLocation(renderable->how->programID, renderable->uniformInfos[i].name);
		if(uniformLocation != -1){
			if(renderable->uniformInfos[i].dataType == FLOAT){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				float uniform = newData[0];
				glUniform1f(uniformLocation, uniform);
			}
			else if(renderable->uniformInfos[i].dataType == VEC2){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				glm::vec2 uniform(newData[0], newData[1]);
				glUniform2fv(uniformLocation, 1, &uniform[0]);
			}
			else if(renderable->uniformInfos[i].dataType == VEC3){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				glm::vec3 uniform(newData[0], newData[1], newData[2]);
				glUniform3fv(uniformLocation, 1, &uniform[0]);
			}
			else if(renderable->uniformInfos[i].dataType == VEC4){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				glm::vec4 uniform(newData[0], newData[1], newData[2], newData[3]);
				glUniform4fv(uniformLocation, 1, &uniform[0]);
			}
			else if(renderable->uniformInfos[i].dataType == MAT3){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				glm::mat3 uniform(newData[0], newData[1], newData[2], newData[3], newData[4], newData[5], newData[6], newData[7], newData[8]);
				glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &uniform[0][0]);
			}
			else if(renderable->uniformInfos[i].dataType == MAT4){
				float *newData = reinterpret_cast<float*>(renderable->uniformInfos[i].data); 
				glm::mat4 uniform(newData[0], newData[1], newData[2], newData[3], newData[4], newData[5], newData[6], newData[7], 
								  newData[8], newData[9], newData[10], newData[11], newData[12], newData[13], newData[14], newData[15]);
				glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &uniform[0][0]);
			}
			else if(renderable->uniformInfos[i].dataType == UINT){
				GLuint newData = *reinterpret_cast<GLuint*>(renderable->uniformInfos[i].data);
				glUniform1i(uniformLocation, newData);
			}
		}
	}
}

void Renderer::setupConventionalUniforms(Renderable* renderable, PassInfo* passInfo){
	GLint mvpUniformLocation = glGetUniformLocation(renderable->how->programID, "mvp");
	if(mvpUniformLocation != -1){
		glm::mat4 mvp =
			glm::perspective(glm::radians(60.0f), ((float)width())/height(), 0.1f, 150.0f) *
			passInfo->camera.getWorldToViewMatrix() *
			renderable->where;
		glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	}
	GLint texUniform = glGetUniformLocation(renderable->how->programID, "meTexture");
	if(texUniform != -1){
		glUniform1i(texUniform, renderable->texture->location);
	}
	GLint modelToWorldUniform = glGetUniformLocation(renderable->how->programID, "modelToWorld");
	if(modelToWorldUniform != -1){
		glm::mat4 modelToWorld = renderable->where;
		glUniformMatrix4fv(modelToWorldUniform, 1, GL_FALSE, &modelToWorld[0][0]);
	}
}

void Renderer::doPass(PassInfo* passInfo){
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	for(unsigned int i = 0; i< passInfo->numRenderables; i++){
		Renderable* r = renderables + i;
		if(! r->visible) continue;
		
		GeometryInfo* g = r->what;
		
		glUseProgram(r->how->programID);
		
		glBindVertexArray(g->vertexArrayObjectID);

		setupConventionalUniforms(r, passInfo);
		setupUniforms(r);

		glDrawElements(
			g->indexingMode, g->numIndices,
			g->indiceDataType, (void*)(g->indexByteOffset));
	}
}

PassInfo* Renderer::addPassInfo(){
	ASSERT(numPassInfos < MAX_PASS_INFOS);
	return passInfos + numPassInfos++;
}

TextureInfo* Renderer::addTexture(const char* imagePath, const char* imageType){
	ASSERT(numTextureInfos < MAX_TEXTURE_INFOS);
	TextureInfo* ret = textureInfos + numTextureInfos;
	ret->location = numTextureInfos;
	ret->textureLocation = GL_TEXTURE0 + numTextureInfos++;
	glActiveTexture(ret->textureLocation);
	QImage image = QGLWidget::convertToGLFormat(QImage(imagePath, imageType));
	GLuint meTextureID;
	glGenTextures(1, &meTextureID);
	glBindTexture(GL_TEXTURE_2D, meTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return ret;
}

void Renderer::paintGL(){
	for(unsigned int i = 0; i < numPassInfos; i++){
		doPass(passInfos + i);
	}
}

void Renderer::draw(){
	repaint();
}