#define GLM_FORCE_RADIANS
#include <Camera.h>
#include <gtx\transform.hpp>

const float Camera::MOVEMENT_SPEED = 0.3f;

Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), UP(0.0f, 1.0f, 0.0f){
	position = glm::vec3(0.0, 1.0, 7.5);
		strafeDirection = glm::cross(viewDirection, UP);
}

glm::mat4 Camera::getWorldToViewMatrix() const{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition){
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if(glm::length(mouseDelta * 2.0f) > 100.0f){
		oldMousePosition = newMousePosition;
		return;
	}
	strafeDirection = glm::cross(viewDirection, UP);

	viewDirection = glm::mat3(glm::rotate(-mouseDelta.x * 2.0f * 0.005f, UP) * 
					glm::rotate(-mouseDelta.y * 2.0f * 0.005f, strafeDirection)) * viewDirection;

	oldMousePosition = newMousePosition;
}

void Camera::setPosition(glm::vec3 pos){
	position = pos;
}

void Camera::moveForward(){
	position += MOVEMENT_SPEED * viewDirection;
}
void Camera::moveBackward(){
	position += -MOVEMENT_SPEED * viewDirection;
}
void Camera::strafeRight(){
	position += MOVEMENT_SPEED * strafeDirection;
}
void Camera::strafeLeft(){
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Camera::moveUp(){
	position += MOVEMENT_SPEED * UP;
}
void Camera::moveDown(){
	position += -MOVEMENT_SPEED * UP;
}

glm::vec3 Camera::getPosition(){
	glm::vec3 positionCopy(position.x, position.y, position.z);
	return positionCopy;
}