#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <QtOpenGL\qglwidget>
#include <ExportHeader.h>

#include <Rendering\GeometryInfo.h>
#include <Rendering\BufferInfo.h>
#include <Rendering\Renderable.h>
#include <Rendering\ShaderInfo.h>
#include <Rendering\VertexLayoutInfo.h>
#include <Rendering\PassInfo.h>
#include <Rendering\TextureInfo.h>
#include <Rendering\UniformInfo.h>
class Renderer : public QGLWidget
{
	static const unsigned int MAX_VERTEX_LAYOUT_INFOS = 100;
	VertexLayoutInfo vertexLayouts[MAX_VERTEX_LAYOUT_INFOS];

	static const unsigned int MAX_GEOMETRIES = 100;
	GeometryInfo geometries[MAX_GEOMETRIES];

	static const unsigned int MAX_BUFFER_INFOS = 2000;
	BufferInfo buffers[MAX_BUFFER_INFOS];

	static const unsigned int MAX_SHADER_INFOS = 100;
	ShaderInfo shaders[MAX_SHADER_INFOS];

	static const unsigned int MAX_RENDERABLES = 2000;
	Renderable renderables[MAX_RENDERABLES];

	static const unsigned int MAX_PASS_INFOS = 100;
	PassInfo passInfos[MAX_PASS_INFOS];

	static const unsigned int MAX_TEXTURE_INFOS = 100;
	TextureInfo textureInfos[MAX_TEXTURE_INFOS];

	GeometryInfo* getAvailableGeometryInfo();

	BufferInfo* getAvailableBuffer(GLsizeiptr neededSize);
	void sendDataToBuffer(BufferInfo* buffer, void* data, GLsizeiptr size);
	BufferInfo* findBufferWithEnoughRoom(GLsizeiptr neededsize);
	BufferInfo* findUnusedBuffer();
	BufferInfo* allocateNewBuffer();

	ShaderInfo* getAvailableShaderInfo();

	Renderable* getAvailableRenderable();

	VertexLayoutInfo* getAvailableVertexLayoutInfo();

	GLuint setupVertexArrayObject(VertexLayoutInfo* vertexLayoutInfo, GLuint vertexBufferID, GLuint vertexBufferDataOffset, GLuint indexBufferID);

	GLuint compileShader(const char* code, GLenum shaderType);
	void checkCompileStatus(GLuint shaderID);
	GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
	void checkLinkStatus(GLuint programID);

	//void doPass(PassInfo* passInfo);

	static Renderer* instance;
	GLuint numRenderables;
	GLuint numPassInfos;
	GLuint numTextureInfos;

	void doPass(PassInfo* passInfo);
	void setupUniforms(Renderable* renderable);
	void setupConventionalUniforms(Renderable* renderable, PassInfo* passInfo);

	Renderer() {}
	Renderer(const Renderer&);
	Renderer& operator=(const Renderer&);
protected:
	void initializeGL();
	void paintGL();
public:
	ENGINE_SHARED GeometryInfo* addGeometry(void* verts, GLuint vertexDataSize, void* indices, GLuint numIndices, GLuint indexDataSize, GLuint indiceDataType, GLuint indexingMode, VertexLayoutInfo* vertexLayoutInfo);

	ENGINE_SHARED ShaderInfo* addShader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

	ENGINE_SHARED Renderable* addRenderable(GeometryInfo* what, ShaderInfo* how, const glm::mat4& where = glm::mat4(), TextureInfo* texture = 0);

	ENGINE_SHARED VertexLayoutInfo* addVertexLayoutInfo(GLuint* sizes, GLuint numAttributes, GLuint stride);

	ENGINE_SHARED PassInfo* addPassInfo();

	ENGINE_SHARED TextureInfo* addTexture(const char* imagePath, const char* imageType);

	ENGINE_SHARED UniformInfo* addUniformToRenderable(void* uniformData, const char* uniformName, GLuint dataType, Renderable* renderable);

	ENGINE_SHARED static bool initialize();
	ENGINE_SHARED static bool shutdown();

	ENGINE_SHARED void draw();

	ENGINE_SHARED static Renderer& getInstance(){return *instance; }
};

#define renderer Renderer::getInstance()

#endif