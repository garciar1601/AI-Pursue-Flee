#ifndef ME_GAME_H
#define ME_GAME_H
#include <glm.hpp>
#include <Qt\qobject.h>
#include <Qt\qtimer.h>
#include <ShapeData.h>
#include <NodeSystem.h>
#include <Rendering\ShaderInfo.h>
#include <Rendering\Renderable.h>
#include <AStar.h>
#include <Character.h>

class PassInfo;

class MeGame : public QObject
{
	Q_OBJECT

		QTimer pumper;

	bool isLinkNotObstructed(int node1, int node2);
	void makeLine(glm::vec3 start, glm::vec3 stop);
	void doRendererStuff();
	void debugStuff();
	void updateCamera();
	void updateCameraFromMouse();
	void updateCameraFromKeyborad();
	void displayDebug();
	void readConfigFile(string filepath);
	void readNodes();
	void showHideNodes();
	void showHideLinks();
	void lerp(glm::vec3 start, glm::vec3 end, float dt);
	void lerpCharacter(float dt);
	void determineCurrentNode();
	void determineTargetNode();
	void calcTangents(Neumont::ShapeData& shapeData);
	float rayTriangleIntersect(glm::vec3 &rayOrg, glm::vec3 &rayDelta, glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2);
	float nodeSize;
	float prevNodeSize;
	Neumont::ShapeData readMyBinaryFile();
	Neumont::ShapeData MeGame::readMyBinaryFile(const char* filePath);
	glm::vec3 getRandomDirection();
	void updateCharacters(float dt);
	void seek(Character* character, float dt);
	void flee(Character* character, float dt);
	void follow(Character* character, float dt);

	int charGroups;
	bool enableKill;
	GeometryInfo* lineGeo;
	AStar aStar;
	NodeSystem nodeSystem;
	bool onPath;
	glm::vec3 prevPos;
	Node previousNode;
	Node currentNode;
	Node targetNode;
	float beta;
	float speed;
	bool nodesShowing;
	bool linksShowing;
	int numLinks;
	string nodeFilepath;
	int sceneDataSlot;
	static const int MAX_LINKS = 2000;
	Renderable* linkRenderables[MAX_LINKS];
	static const int MAX_NODES = 1000;
	Renderable* nodeRenderbales[MAX_NODES];
	static const int MAX_SCENE_DATA = 100;
	Neumont::ShapeData sceneData[MAX_SCENE_DATA];
	glm::vec3 textureCubePos;
	glm::vec3 lightCubePos;
	glm::vec3 ambient;
	ShaderInfo* passThroughShader;
	ShaderInfo* nodeShader;
	ShaderInfo* linkShader;
	ShaderInfo* obstructedShader;
	ShaderInfo* targetShader;
	PassInfo* thePass;
	Character* characters;
	int numCharacters;
	//Renderable* character;
	//glm::vec3 charPos;
	private slots:
		void newFrame();
public:
	bool initialize();
	void runGameLoop();
	bool shutdown();
};

#endif