#define GLM_FORCE_RADIANS
#include <MeGame.h>
#include <Qt\qapplication.h>
#include <Qt\qpoint.h>
#include <Qt\qdebug.h>
#include <Rendering\Renderer.h>
#include <ShapeGenerator.h>
#include <IO\FileIO.h>
#include <gtx\transform.hpp>
#include <Rendering\PassInfo.h>
#include <Camera.h>
#include <Rendering\UniformDataTypes.h>
#include <Debug\DebugTools\DebugMenu.h>
#include <Menu.h>
#include <QtGui\qvboxlayout>
#include <Rendering\ShapeCreator.h>
#include <Debug\DebugTools\DebugShapes.h>
#include <Timer.h>
#include <fstream>
#include <iostream>
#include <gtx\rotate_vector.hpp>
#include <Node.h>
#include <FastDelegate.h>
#include <memory>
#include <Qt\qfiledialog.h>
#include <Logger.h>
#include <time.h>


Timer timer;
float fps;
float specularExponent;
TextureInfo* shapesTexture;
TextureInfo* flatTexture;
TextureInfo* color;
TextureInfo* diffuse;
bool pastNormalMap;
bool normalMap;
bool pastDiffuseOn;
bool diffuseOn;
bool pastSpecularOn;
bool specularOn;
glm::vec3 diffuseColor;
glm::vec3 specularColor;
float rotationX;
float rotationY;
glm::mat4 rotation;
float wait = 0;

bool MeGame::initialize(){
	if(!renderer.initialize()){
		return false;
	}
	GLuint sizes[] = {3, 4, 3, 2 };
	VertexLayoutInfo* vertexLayoutInfo = renderer.addVertexLayoutInfo(sizes, ARRAYSIZE(sizes), Neumont::Vertex::STRIDE);
	GLfloat vectorFloats[] = {
		0.0f, 0.0f, 0.0f, //0
		0.0f, 0.0f, 0.0f,

		0.0f, 0.75f, 0.0f, //1
		0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, //2
		0.0f, 0.0f, 0.0f
	};

	unsigned short vectorIndices[] =
	{
		0, 1, 2
	};

	charGroups = 10;
	enableKill = true;

	lineGeo = renderer.addGeometry(vectorFloats, sizeof(vectorFloats), 
		vectorIndices, sizeof(vectorIndices) / sizeof(vectorIndices[0]), sizeof(vectorIndices), GL_UNSIGNED_SHORT, 
		GL_LINES, vertexLayoutInfo);
	srand(time(NULL));
	onPath = false;
	numCharacters = 0;
	beta = 0.0f;
	speed = 15.0f;
	numLinks = 0;
	sceneDataSlot = 0;
	nodeSystem.numNodes = 0;
	prevNodeSize = 1.0f;
	nodeSize = 1.0f;
	nodesShowing = true;
	linksShowing = true;
	std::string v = std::string(FileIO::file2String("ShaderCode/VertexPassThroughShader.glsl"));
	std::string f = std::string(FileIO::file2String("ShaderCode/FragPassThroughShader.glsl"));
	passThroughShader = renderer.addShader(v.c_str(), f.c_str());
	v = std::string(FileIO::file2String("ShaderCode/VertexNodeShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragNodeShader.glsl"));
	nodeShader = renderer.addShader(v.c_str(), f.c_str());
	v = std::string(FileIO::file2String("ShaderCode/VertexLinkShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragLinkShader.glsl"));
	linkShader = renderer.addShader(v.c_str(), f.c_str());
	v = std::string(FileIO::file2String("ShaderCode/VertexObstructedShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragObstructedShader.glsl"));
	obstructedShader = renderer.addShader(v.c_str(), f.c_str());
	v = std::string(FileIO::file2String("ShaderCode/VertexTargetShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragTargetShader.glsl"));
	targetShader = renderer.addShader(v.c_str(), f.c_str());
	//glClearColor(0.5f, 0.75f, 0.95f, 1.0f);
	normalMap = true;
	pastNormalMap = true;
	specularOn = true;
	pastSpecularOn = true;
	diffuseOn = true;
	pastDiffuseOn = true;
	doRendererStuff();
	fps = 1 / timer.Interval();	
	thePass->camera.setPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	//readConfigFile("Config.txt");
	debugStuff();	
	//displayDebug();
	//determineTargetNode();
	connect(&pumper, SIGNAL(timeout()), this, SLOT(newFrame()));
	return true;
}

void MeGame::showHideNodes(){
	if(nodesShowing){
		for(int i = 0; i < nodeSystem.numNodes; i++){
			nodeRenderbales[i]->visible = false;
		}
		nodesShowing = false;
	}
	else{
		for(int i = 0; i < nodeSystem.numNodes; i++){
			nodeRenderbales[i]->visible = true;
		}
		nodesShowing = true;
	}
}

void MeGame::showHideLinks(){
	if(linksShowing){
		for(int i = 0; i < numLinks; i++){
			linkRenderables[i]->visible = false;
		}
		linksShowing = false;
	}
	else{
		for(int i = 0; i < numLinks; i++){
			linkRenderables[i]->visible = true;
		}
		linksShowing = true;
	}
}

void MeGame::readNodes(){
	std::ifstream input(nodeFilepath.c_str(), std::ios::binary | std::ios::in);
	if(input){
		input.seekg(0, std::ios::end);
		unsigned int numBytes = input.tellg();
		input.seekg(0, std::ios::beg);
		char* bytes = new char[numBytes];
		input.read(bytes, numBytes);
		string message = "Nodes Read: " + std::to_string(numBytes) + " bytes";
		LOG(Info, message.c_str());
		
		input.close();

		nodeSize = *reinterpret_cast<float*>(bytes);
		nodeSystem = *reinterpret_cast<NodeSystem*>(bytes + sizeof(float));
		GLuint sizes[] = {3, 4, 3, 2 };
		VertexLayoutInfo* vertexLayoutInfo = renderer.addVertexLayoutInfo(sizes, ARRAYSIZE(sizes), Neumont::Vertex::STRIDE);
		for(int i = 0; i < nodeSystem.numNodes; i++){
			Node node = nodeSystem.nodes[i];
			Neumont::ShapeData data = Neumont::ShapeGenerator::makeSphere(20);
			GeometryInfo* geometry =
				renderer.addGeometry(
					data.verts, data.vertexBufferSize(),
					data.indices, data.numIndices, data.indexBufferSize(),
					GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

			Renderable* renderable = renderer.addRenderable(geometry, nodeShader, glm::translate(node.position) * glm::scale(glm::vec3(nodeSize, nodeSize, nodeSize)));

			thePass->addRenderable(renderable);
			nodeRenderbales[i] = renderable;
		}
		for(int i = 0; i < nodeSystem.numNodes; i++){
			Node node = nodeSystem.nodes[i];
			for(int i2 = 0; i2 < node.numConnections; i2++){
				if(isLinkNotObstructed(i, node.connections[i2])){
					makeLine(node.position, nodeSystem.nodes[node.connections[i2]].position);
				}
				else{
					makeLine(node.position, nodeSystem.nodes[node.connections[i2]].position);
					linkRenderables[numLinks-1]->how = obstructedShader;
					nodeRenderbales[i]->how = obstructedShader;
					nodeRenderbales[node.connections[i2]]->how = obstructedShader;
				}
			}
		}
	}
}

void MeGame::readConfigFile(string filepath){
	std::ifstream config(filepath);
	string line;
	while(std::getline(config, line)){
		if(line.empty()){
			continue;
		}
		if(line.compare("LevelFile:") == 0){
			std::getline(config, line);
			std::ifstream test(line);
			if(test){
				test.close();
				GLuint sizes[] = {3, 4, 3, 2 };
				VertexLayoutInfo* vertexLayoutInfo = renderer.addVertexLayoutInfo(sizes, ARRAYSIZE(sizes), Neumont::Vertex::STRIDE);
				Neumont::ShapeData data = readMyBinaryFile(line.c_str());

				sceneData[sceneDataSlot] = data;
				sceneDataSlot++;
	
				GeometryInfo* geometry =
					renderer.addGeometry(
						data.verts, data.vertexBufferSize(),
						data.indices, data.numIndices, data.indexBufferSize(),
						GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

				std::string v = std::string(FileIO::file2String("ShaderCode/VertexTextureShader.glsl"));
				std::string f = std::string(FileIO::file2String("ShaderCode/FragTextureShader.glsl"));
				ShaderInfo* shader = renderer.addShader(v.c_str(), f.c_str());

				TextureInfo* hillsTex = renderer.addTexture("stone.png", "PNG");
	
				Renderable* renderable = renderer.addRenderable(geometry, shader, glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)), hillsTex);

				renderer.addUniformToRenderable(new glm::mat4(glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f))), "rotation", MAT4, renderable);
				renderer.addUniformToRenderable(&lightCubePos, "diffuseLightPos", VEC3, renderable);
				renderer.addUniformToRenderable(&ambient, "ambient", VEC3, renderable);

				thePass->addRenderable(renderable);
				LOG(Info, "Scene Data Read");
			}
		}
		else if(line.compare("NodeSystem:") == 0){
			std::getline(config, line);
			nodeFilepath = line;
			readNodes();
		}
	}
	config.close();
}

void MeGame::runGameLoop(){
	pumper.start();
	LOG(Info, "Game Loop Started");
}

bool MeGame::shutdown(){
	bool ret = true;
	ret &= renderer.shutdown();
	LOG(Info, "Renderer Shut Down");
	debugMenu.close();
	LOG(Info, "Debug Menu Closed");
	END_LOG;
	pumper.stop();
	return ret;
}

void MeGame::debugStuff(){
	// HIPPO
	debugMenu.initialize(new QVBoxLayout);
	//debugMenu.button("Show/Hide Nodes", fastdelegate::MakeDelegate(this, &MeGame::showHideNodes));
	//debugMenu.button("Show/Hide Links", fastdelegate::MakeDelegate(this, &MeGame::showHideLinks));
	debugMenu.move(0, 0);
	LOG(Info, "Debug Menu Initialized");
}

void MeGame::displayDebug(){
	debugMenu.display();
}

void MeGame::newFrame(){
	fps = 1 / timer.Interval();
	thePass->renderables[0]->where = glm::translate(lightCubePos) * glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	updateCamera();
	thePass->renderables[1]->where = glm::translate(thePass->camera.position + thePass->camera.viewDirection * 0.1f) * glm::scale(glm::vec3(0.001f, 0.001f, 0.001f));
	//character->where = glm::translate(charPos) * glm::orientation(glm::normalize(currentNode.position - charPos), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(0.025f, 0.025f, 0.02f));
	//lerpCharacter(1/fps);
	updateCharacters(1/fps);
	for(int i = 0; i < numCharacters; i++){
		if(characters[i].isAlive){
			characters[i].renderable->where = glm::translate(characters[i].pos) * glm::orientation(characters[i].direction, glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(glm::vec3(1.0f, 3.0f, 1.0f));
		}
	}
	renderer.draw();
	debugMenu.update();
	if(wait > 0.0f){
		wait -= 1 / fps;
	}
	if(GetAsyncKeyState(VK_ESCAPE)){
		shutdown();
	}
}

void MeGame::updateCharacters(float dt){
	for(int i = 0; i < numCharacters; i++){
		Character* character = &characters[i];
		if(character->isAlive){
			switch(character->currentState){
				case Character::SEEK:
					seek(character, dt);
					break;
				case Character::FLEE:
					flee(character, dt);
					break;
				case Character::FOLLOW:
					follow(character, dt);
					break;
			}
		}
	}
}

void MeGame::seek(Character* character, float dt){
	character->pos += character->velocity * dt;
	if(character->pos.x <= -40.0f){
		character->pos.x = -40.0f;
		character->velocity.x = -character->velocity.x;
		character->direction = glm::normalize(character->velocity);
	}
	if(character->pos.x >= 40.0f){
		character->pos.x = 40.0f;
		character->velocity.x = -character->velocity.x;
		character->direction = glm::normalize(character->velocity);
	}
	if(character->pos.y <= -40.0f){
		character->pos.y = -40.0f;
		character->velocity.y = -character->velocity.y;
		character->direction = glm::normalize(character->velocity);
	}
	if(character->pos.y >= 0.0f){
		character->pos.y = 0.0f;
		character->velocity.y = -character->velocity.y;
		character->direction = glm::normalize(character->velocity);
	}
	if(character->pos.z <= -40.0f){
		character->pos.z = -40.0f;
		character->velocity.z = -character->velocity.z;
		character->direction = glm::normalize(character->velocity);
	}
	if(character->pos.z >= 40.0f){
		character->pos.z = 40.0f;
		character->velocity.z = -character->velocity.z;
		character->direction = glm::normalize(character->velocity);
	}
	for(int i = 0; i < numCharacters; i++){
		if(characters[i].isAlive){
			if(character->type == Character::ROCK){
				if(characters[i].type == Character::PAPER){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
			else if(character->type == Character::PAPER){
				if(characters[i].type == Character::SCISSORS){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
			else if(character->type == Character::SCISSORS){
				if(characters[i].type == Character::ROCK){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
		}
	}
	if(character->currentState != Character::FLEE){
		for(int i = 0; i < numCharacters; i++){
			if(characters[i].isAlive){
				if(character->type == Character::ROCK){
					if(characters[i].type == Character::SCISSORS){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < 20.0f){
							character->currentState = Character::FOLLOW;
						}
					}
				}
				else if(character->type == Character::PAPER){
					if(characters[i].type == Character::ROCK){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < 20.0f){
							character->currentState = Character::FOLLOW;
						}
					}
				}
				else if(character->type == Character::SCISSORS){
					if(characters[i].type == Character::PAPER){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < 20.0f){
							character->currentState = Character::FOLLOW;
						}
					}
				}
			}
		}
	}
}

void MeGame::flee(Character* character, float dt){
	character->pos += character->velocity * dt;
	Character* closestCharacter = NULL;
	float closestDistance = 1e30f;
	for(int i = 0; i < numCharacters; i++){
		if(characters[i].isAlive){
			if(character->type == Character::ROCK){
				if(characters[i].type == Character::PAPER){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < closestDistance){
						closestDistance = distance;
						closestCharacter = &characters[i];
					}
				}
			}
			else if(character->type == Character::PAPER){
				if(characters[i].type == Character::SCISSORS){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < closestDistance){
						closestDistance = distance;
						closestCharacter = &characters[i];
					}
				}
			}
			else if(character->type == Character::SCISSORS){
				if(characters[i].type == Character::ROCK){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < closestDistance){
						closestDistance = distance;
						closestCharacter = &characters[i];
					}
				}
			}
		}
	}

	if(closestDistance >= 26.0f){
		character->currentState = Character::SEEK;
		character->velocity = getRandomDirection() * 10.0f;
		character->direction = glm::normalize(character->velocity);
	}

	else if(closestDistance <= 2.0f && enableKill){
			character->isAlive = false;
			character->renderable->visible = false;
	}

	else{
		character->velocity = glm::normalize(character->pos - closestCharacter->pos) * 30.0f;
		character->direction = glm::normalize(character->velocity);
		if(character->pos.x <= -40.0f){
			character->pos.x = -40.0f;
			character->velocity.x = -character->velocity.x;
		}
		if(character->pos.x >= 40.0f){
			character->pos.x = 40.0f;
			character->velocity.x = -character->velocity.x;
		}
		if(character->pos.y <= -40.0f){
			character->pos.y = -40.0f;
			character->velocity.y = -character->velocity.y;
		}
		if(character->pos.y >= 0.0f){
			character->pos.y = 0.0f;
			character->velocity.y = -character->velocity.y;
		}
		if(character->pos.z <= -40.0f){
			character->pos.z = -40.0f;
			character->velocity.z = -character->velocity.z;
		}
		if(character->pos.z >= 40.0f){
			character->pos.z = 40.0f;
			character->velocity.z = -character->velocity.z;
		}
	}
}

void MeGame::follow(Character* character, float dt){
	character->pos += character->velocity * dt;
	for(int i = 0; i < numCharacters; i++){
		if(characters[i].isAlive){
			if(character->type == Character::ROCK){
				if(characters[i].type == Character::PAPER){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
			else if(character->type == Character::PAPER){
				if(characters[i].type == Character::SCISSORS){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
			else if(character->type == Character::SCISSORS){
				if(characters[i].type == Character::ROCK){
					float distance = glm::length(character->pos - characters[i].pos);
					if(distance < 15.0f){
						character->currentState = Character::FLEE;
					}
				}
			}
		}
	}
	if(character->currentState != Character::FLEE){
		Character* closestCharacter = NULL;
		float closestDistance = 1e30f;
		for(int i = 0; i < numCharacters; i++){
			if(characters[i].isAlive){
				if(character->type == Character::ROCK){
					if(characters[i].type == Character::SCISSORS){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < closestDistance){
							closestDistance = distance;
							closestCharacter = &characters[i];
						}
					}
				}
				else if(character->type == Character::PAPER){
					if(characters[i].type == Character::ROCK){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < closestDistance){
							closestDistance = distance;
							closestCharacter = &characters[i];
						}
					}
				}
				else if(character->type == Character::SCISSORS){
					if(characters[i].type == Character::PAPER){
						float distance = glm::length(character->pos - characters[i].pos);
						if(distance < closestDistance){
							closestDistance = distance;
							closestCharacter = &characters[i];
						}
					}
				}
			}
		}
		if(closestDistance >= 22.0f){
			character->currentState = Character::SEEK;
			character->velocity = getRandomDirection() * 10.0f;
			character->direction = glm::normalize(character->velocity);
		}
		else{
			character->velocity = glm::normalize(closestCharacter->pos - character->pos) * 21.0f;
			character->direction = glm::normalize(character->velocity);
		}
	}
}

Neumont::ShapeData MeGame::readMyBinaryFile(const char* filePath){
	std::ifstream input(
		filePath,
		std::ios::binary | std::ios::in);

	input.seekg(0, std::ios::end);
	unsigned int numBytes = input.tellg();
	input.seekg(0, std::ios::beg);
	char* bytes = new char[numBytes];
	input.read(bytes, numBytes);
	input.close();

	Neumont::ShapeData ret;

	char* vertexBase = bytes + 2 * sizeof(unsigned int);

	ret.numVerts = *reinterpret_cast<unsigned int*>(bytes);
	ret.numIndices = reinterpret_cast<unsigned int*>(bytes)[1];

	char* indexBase = vertexBase + ret.vertexBufferSize();
	ret.verts = reinterpret_cast<Neumont::Vertex*>(vertexBase);
	ret.indices = reinterpret_cast<unsigned short*>(indexBase);

	calcTangents(ret);
	return ret;
}

void MeGame::lerp(glm::vec3 start, glm::vec3 end, float dt){
	/*charPos.x = ((1 - beta) * start.x) + (beta * end.x);
	charPos.y = ((1 - beta) * start.y) + (beta * end.y);
	charPos.z = ((1 - beta) * start.z) + (beta * end.z);

	beta += speed/glm::length(start - end) * dt;*/
	start;
	end;
	dt;
}

void MeGame::lerpCharacter(float dt){
	if(!onPath){
		if(beta >= 1.0f){
			onPath = true;
			determineTargetNode();
			beta = 0.0f;
		}
		else{
			lerp(prevPos, currentNode.position, dt);
		}
	}
	else{
		if(beta >= 1.0f){
			if(currentNode.id == targetNode.id){
				determineTargetNode();
				beta = 0.0f;
			}
			else{
				determineCurrentNode();
				beta = 0.0f;
			}
		}
		else{
			lerp(previousNode.position, currentNode.position, dt);
		}
	}
}

void MeGame::determineTargetNode(){
	/*if(!onPath){
		prevPos = charPos;
		float minLength = 1e30f;
		for(int i = 0; i < nodeSystem.numNodes; i++){
			float length = glm::length(charPos - nodeSystem.nodes[i].position);
			if(length < minLength){
				currentNode = nodeSystem.nodes[i];
				targetNode = nodeSystem.nodes[i];
			}
		}
	}
	else{
		bool nodeNotFound = true;
		while(nodeNotFound){		
			int loc = rand() % nodeSystem.numNodes;
			int id = loc;
			if(id != currentNode.id){
				nodeRenderbales[targetNode.id]->how = nodeShader;
				targetNode = nodeSystem.nodes[id];
				nodeRenderbales[targetNode.id]->how = linkShader;
				aStar.aStar(currentNode.id, id, nodeSystem);
				previousNode = nodeSystem.nodes[aStar.pathList[aStar.currentNode]];
				aStar.currentNode++;
				currentNode = nodeSystem.nodes[aStar.pathList[aStar.currentNode]];
				aStar.currentNode++;
				nodeNotFound = false;
			}
		}
	}*/
}

void MeGame::determineCurrentNode(){
	previousNode = currentNode;
	currentNode = nodeSystem.nodes[aStar.pathList[aStar.currentNode]];
	aStar.currentNode++;
}

void MeGame::calcTangents(Neumont::ShapeData& shapeData){
	glm::vec3 *tan1 = new glm::vec3[shapeData.numVerts * 2];
	glm::vec3 *tan2 = tan1 + shapeData.numVerts;
	memset(tan1, 0, shapeData.numVerts * sizeof(glm::vec3) * 2);

	uint triangleCount = shapeData.numIndices / 3;

	for (uint a = 0; a < triangleCount; a++)
	{
		uint triBase = a * 3;
		uint i1 = shapeData.indices[triBase + 0];
		uint i2 = shapeData.indices[triBase + 1];
		uint i3 = shapeData.indices[triBase + 2];
		

		const Neumont::Vertex& v1 = shapeData.verts[i1];
		const Neumont::Vertex& v2 = shapeData.verts[i2];
		const Neumont::Vertex& v3 = shapeData.verts[i3];

		float x1 = v2.position.x - v1.position.x;
		float x2 = v3.position.x - v1.position.x;
		float y1 = v2.position.y - v1.position.y;
		float y2 = v3.position.y - v1.position.y;
		float z1 = v2.position.z - v1.position.z;
		float z2 = v3.position.z - v1.position.z;

		float s1 = v2.uv.x - v1.uv.x;
		float s2 = v3.uv.x - v1.uv.x;
		float t1 = v2.uv.y - v1.uv.y;
		float t2 = v3.uv.y - v1.uv.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
		(t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
		(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
		}

		for (uint a = 0; a < shapeData.numVerts; a++)
		{
		Neumont::Vertex& vert = shapeData.verts[a];
		const glm::vec3& n = vert.normal;
		const glm::vec3& t = tan1[a];

		// Gram-Schmidt orthogonalize
		vert.color = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 1.0f);

		// Calculate handedness
		/////vert.tangent.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
	}

	delete[] tan1;
}

void MeGame::doRendererStuff(){
	GLuint sizes[] = {3, 4, 3, 2 };
	VertexLayoutInfo* vertexLayoutInfo = renderer.addVertexLayoutInfo(sizes, ARRAYSIZE(sizes), Neumont::Vertex::STRIDE);

	thePass = renderer.addPassInfo();
	
	ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	specularExponent = 50;
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	Neumont::ShapeData data = Neumont::ShapeGenerator::makeCube();
	GeometryInfo* geometry =
		renderer.addGeometry(
			data.verts, data.vertexBufferSize(),
			data.indices, data.numIndices, data.indexBufferSize(),
			GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	ShaderInfo* shader = passThroughShader;

	Renderable* renderable = renderer.addRenderable(geometry, shader, glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.5f)) * glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)));
	thePass->addRenderable(renderable);

	lightCubePos = glm::vec3(50.0f, 50.0f, -50.0f);

	data = Neumont::ShapeGenerator::makeSphere(20);
	geometry =
		renderer.addGeometry(
			data.verts, data.vertexBufferSize(),
			data.indices, data.numIndices, data.indexBufferSize(),
			GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	std::string v = std::string(FileIO::file2String("ShaderCode/VertexCursorShader.glsl"));
	std::string f = std::string(FileIO::file2String("ShaderCode/FragCursorShader.glsl"));
	shader = renderer.addShader(v.c_str(), f.c_str());

	renderable = renderer.addRenderable(geometry, shader, glm::translate(thePass->camera.position + thePass->camera.viewDirection * 0.1f) * glm::scale(glm::vec3(0.001f, 0.001f, 0.001f)));
	thePass->addRenderable(renderable);

	data = Neumont::ShapeGenerator::makeCube();
	geometry =
		renderer.addGeometry(
			data.verts, data.vertexBufferSize(),
			data.indices, data.numIndices, data.indexBufferSize(),
			GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	v = std::string(FileIO::file2String("ShaderCode/VertexTextureShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragTextureShader.glsl"));

	shader = renderer.addShader(v.c_str(), f.c_str());

	TextureInfo* hillsTex = renderer.addTexture("stone.png", "PNG");

	renderable = renderer.addRenderable(geometry, shader, glm::translate(glm::vec3(0.0f, -86.0f, 0.0f)) * glm::scale(glm::vec3(43.0f, 43.0f, 43.0f)), hillsTex);

	renderer.addUniformToRenderable(new glm::mat4(glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f))), "rotation", MAT4, renderable);
	renderer.addUniformToRenderable(&lightCubePos, "diffuseLightPos", VEC3, renderable);
	renderer.addUniformToRenderable(&ambient, "ambient", VEC3, renderable);
	thePass->addRenderable(renderable);

	/*data = readMyBinaryFile("../OBJ Converter/bin/Debug/Godzilla.bin");

	geometry =
		renderer.addGeometry(
			data.verts, data.vertexBufferSize(),
			data.indices, data.numIndices, data.indexBufferSize(),
			GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	v = std::string(FileIO::file2String("ShaderCode/VertexNormalMapShader.glsl"));
	f = std::string(FileIO::file2String("ShaderCode/FragNormalMapShader.glsl"));
	shader = renderer.addShader(v.c_str(), f.c_str());
	
	ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	shapesTexture = renderer.addTexture("Godzilla_N.png", "PNG");
	flatTexture = renderer.addTexture("Flat.png", "PNG");
	color = renderer.addTexture("Godzilla_I.png", "PNG");
	diffuse = renderer.addTexture("Godzilla_D.png", "PNG");
	
	specularExponent = 50;
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	charPos = glm::vec3();
	
	renderable = renderer.addRenderable(geometry, shader, glm::translate(charPos) *  glm::scale(glm::vec3(0.025f, 0.025f, 0.02f)), shapesTexture);
	textureCubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationX = 0.0f;
	rotationY = 0.0f;
	rotation = glm::mat4();
	renderer.addUniformToRenderable(&diffuse->location, "diffuse", UINT, renderable);
	renderer.addUniformToRenderable(&color->location, "color", UINT, renderable);
	renderer.addUniformToRenderable(&rotation, "rotation", MAT4, renderable);
	renderer.addUniformToRenderable(&thePass->camera.position, "cameraPos", VEC3, renderable);
	renderer.addUniformToRenderable(&specularExponent, "specularExponent", FLOAT, renderable);
	renderer.addUniformToRenderable(&diffuseColor, "diffuseColor", VEC3, renderable);
	renderer.addUniformToRenderable(&specularColor, "specularColor", VEC3, renderable);
	renderer.addUniformToRenderable(&lightCubePos, "diffuseLightPos", VEC3, renderable);
	renderer.addUniformToRenderable(&ambient, "ambient", VEC3, renderable);

	thePass->addRenderable(renderable);
	character = renderable;*/

	data = readMyBinaryFile("../OBJ Converter/bin/Debug/Pyramid.bin");
	geometry =
		renderer.addGeometry(
			data.verts, data.vertexBufferSize(),
			data.indices, data.numIndices, data.indexBufferSize(),
			GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	characters = new Character[3 * charGroups];
	for(int i = 0; i < charGroups; i++)
	{
		shader = obstructedShader;

		renderable = renderer.addRenderable(geometry, shader, glm::scale(glm::vec3(1.0f, 3.0f, 1.0f)));
		thePass->addRenderable(renderable);
	
		characters[numCharacters].renderable = renderable;
		characters[numCharacters].type = characters->ROCK;
		characters[numCharacters].currentState = characters->SEEK;
		characters[numCharacters].pos = glm::vec3(30.0f, -20.0f, 40.0f);
		characters[numCharacters].velocity = getRandomDirection() * 10.0f;
		characters[numCharacters].direction = glm::normalize(characters[numCharacters].velocity);
		characters[numCharacters].isAlive = true;

		numCharacters++;

		shader = nodeShader;

		renderable = renderer.addRenderable(geometry, shader, glm::scale(glm::vec3(1.0f, 3.0f, 1.0f)));
		thePass->addRenderable(renderable);

		characters[numCharacters].renderable = renderable;
		characters[numCharacters].type = characters->PAPER;
		characters[numCharacters].currentState = characters->SEEK;
		characters[numCharacters].pos = glm::vec3(30.0f, -20.0f, -40.0f);
		characters[numCharacters].velocity = getRandomDirection() * 10.0f;
		characters[numCharacters].direction = glm::normalize(characters[numCharacters].velocity);
		characters[numCharacters].isAlive = true;

		numCharacters++;

		shader = linkShader;

		renderable = renderer.addRenderable(geometry, shader, glm::scale(glm::vec3(1.0f, 3.0f, 1.0f)));
		thePass->addRenderable(renderable);

		characters[numCharacters].renderable = renderable;
		characters[numCharacters].type = characters->SCISSORS;
		characters[numCharacters].currentState = characters->SEEK;
		characters[numCharacters].pos = glm::vec3(-30.0f, -20.0f, 0.0f);
		characters[numCharacters].velocity = getRandomDirection() * 10.0f;
		characters[numCharacters].direction = glm::normalize(characters[numCharacters].velocity);
		characters[numCharacters].isAlive = true;

		numCharacters++;
	}
}

bool MeGame::isLinkNotObstructed(int node1, int node2){
	float newMinTOne =  1e30f;
	float newMinTTwo =  1e30f;
	float newMinTThree =  1e30f;
	Node n = nodeSystem.nodes[node1];
	glm::vec3 node2Position = nodeSystem.nodes[node2].position;
	glm::vec3 origin = n.position;
	glm::vec3 direction = node2Position - n.position;
	glm::vec3 reverseDirection = n.position - node2Position;
	glm::vec3 n1LeftPos = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction)) * nodeSize + origin;
	glm::vec3 n1RightPos = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction)) * -nodeSize + origin;

	glm::vec3 n2LeftPos = glm::normalize(glm::cross(glm::vec3(0, 1, 0), reverseDirection)) * -nodeSize + node2Position;
	glm::vec3 n2RightPos = glm::normalize(glm::cross(glm::vec3(0, 1, 0), reverseDirection)) * nodeSize + node2Position;
	for(int i = 0; i < sceneDataSlot; i++){
		Neumont::ShapeData data = sceneData[i];

		uint triangleCount = data.numIndices / 3;

		for (uint a = 0; a < triangleCount; a++)
		{
			uint triBase = a * 3;
			Neumont::Vertex& v1 = data.verts[data.indices[triBase + 0]];
			Neumont::Vertex& v2 = data.verts[data.indices[triBase + 1]];
			Neumont::Vertex& v3 = data.verts[data.indices[triBase + 2]];
			float t = rayTriangleIntersect(origin, direction, v1.position, v2.position, v3.position);
			if(t < newMinTOne){
				newMinTOne = t;
			}
			t = rayTriangleIntersect(n1LeftPos, direction, v1.position, v2.position, v3.position);
			if(t < newMinTTwo){
				newMinTTwo = t;
			}
			t = rayTriangleIntersect(n1RightPos, direction, v1.position, v2.position, v3.position);
			if(t < newMinTThree){
				newMinTThree = t;
			}
		}
	}
	glm::vec3 positionOne = origin + (direction * newMinTOne);
	glm::vec3 positionTwo = n1LeftPos + (direction * newMinTTwo);
	glm::vec3 positionThree = n1RightPos + (direction * newMinTThree);
	float lengthOne = glm::length(positionOne - origin);
	float lengthTwo = glm::length(positionTwo - n1LeftPos);
	float lengthThree = glm::length(positionThree - n1RightPos);
	if(lengthOne > glm::length(node2Position - origin) && lengthTwo > glm::length(n2LeftPos - n1LeftPos) && lengthThree > glm::length(n2RightPos - n1RightPos)){
		return true;
	}
	return false;
}

void MeGame::makeLine(glm::vec3 start, glm::vec3 stop){
	float scale = glm::length(stop - start);
	glm::vec3 direction = glm::normalize(stop - start);
	Renderable* renderable = renderer.addRenderable(lineGeo, linkShader, glm::translate(start) * glm::orientation(direction, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(scale, scale, scale)));

	if(!linksShowing){
		renderable->visible = false;
	}

	thePass->addRenderable(renderable);
	linkRenderables[numLinks] = renderable;
	numLinks++;
}

float MeGame::rayTriangleIntersect(glm::vec3 &rayOrg, glm::vec3 &rayDelta, glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2){
	const float noIntersection = 1e30f;
	glm::vec3 edge1 = p1 - p0;
	glm::vec3 edge2 = p2 - p1;

	glm::vec3 normal = glm::cross(edge1, edge2);

	float dot = glm::dot(normal, rayDelta);

	if(!(dot < 0.0f)){
		return noIntersection;
	}

	float d = glm::dot(normal, p0);

	float t = d - glm::dot(normal, rayOrg);

	if(!(t <= 0.0f)){
		return noIntersection;
	}

	t /= dot;
	
	glm::vec3 p = rayOrg + (rayDelta * t);
	float u0, u1, u2;
	float v0, v1, v2;
	if(fabs(normal.x) > fabs(normal.y)){
		if(fabs(normal.x) > fabs(normal.z)){
			u0 = p.y - p0.y;
			u1 = p1.y - p0.y;
			u2 = p2.y - p0.y;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else {
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}
	else{
		if(fabs(normal.y) > fabs(normal.z)){
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}

	float temp = u1 * v2 - v1 * u2;
	if(!(temp != 0.0f)){
		return noIntersection;
	}
	temp = 1.0f / temp;

	float alpha = (u0 * v2 - v0 * u2) * temp;
	if(!(alpha >= 0.0f)){
		return noIntersection;
	}

	float beta = (u1 * v0 - v1 * u0) * temp;
	if(!(beta >= 0.0f)){
		return noIntersection;
	}

	float gamma = 1.0f - alpha - beta;
	if(!(gamma >= 0.0f)){
		return noIntersection;
	}

	return t;
}

void MeGame::updateCamera(){
	updateCameraFromMouse();
	updateCameraFromKeyborad();
}

void MeGame::updateCameraFromMouse(){
	if((QApplication::mouseButtons() & Qt::LeftButton) == 0){
		return;
	}
	QPoint p = renderer.mapFromGlobal(QCursor::pos());
	if(p.x() < 0 || p.y() < 0 || p.x() > renderer.width() || p.y() > renderer.height()){
		return;
	}
	thePass->camera.mouseUpdate(glm::vec2(p.x(), p.y()));
}

void MeGame::updateCameraFromKeyborad(){
	Camera& camera = thePass->camera;
	if(GetAsyncKeyState('W'))
		camera.moveForward();
	if(GetAsyncKeyState('S'))
		camera.moveBackward();
	if(GetAsyncKeyState('A'))
		camera.strafeLeft();
	if(GetAsyncKeyState('D'))
		camera.strafeRight();
	if(GetAsyncKeyState('R'))
		camera.moveUp();
	if(GetAsyncKeyState('F'))
		camera.moveDown();
	if(GetAsyncKeyState(VK_OEM_3))
		displayDebug();
}

glm::vec3 MeGame::getRandomDirection(){
	int x = (rand() % 20) - 10;
	int y = (rand() % 20) - 10;
	int z = (rand() % 20) - 10;
	glm::vec3 direction = glm::normalize(glm::vec3(x, y, z));
	return direction;
}