#ifndef CAMERA_H
#define CAMERA_H
#include <glm.hpp>

class Camera
{
	static const float MOVEMENT_SPEED;	
	
	glm::vec2 oldMousePosition;
public:
	Camera();
	const glm::vec3 UP;
	glm::vec3 strafeDirection;
	glm::vec3 position;
	glm::vec3 viewDirection;
	Camera& operator=(const Camera &tmp);
	ENGINE_SHARED glm::mat4 getWorldToViewMatrix() const;
	ENGINE_SHARED void mouseUpdate(const glm::vec2& newMousePosition);
	ENGINE_SHARED void moveForward();
	ENGINE_SHARED void moveBackward();
	ENGINE_SHARED void strafeRight();
	ENGINE_SHARED void strafeLeft();
	ENGINE_SHARED void moveUp();
	ENGINE_SHARED void moveDown();
	ENGINE_SHARED glm::vec3 getPosition();
	ENGINE_SHARED void setPosition(glm::vec3 pos);
};

#endif