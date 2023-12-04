#include "Camera.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

Camera::Camera()
	:Forward(DEFAULT_FORWARD), Right(DEFAULT_RIGHT), Up(DEFAULT_UP),
	 UpdateForward(false), UpdateRight(false), UpdateUp(false),
	 Position(0.0f), EulerRotation(0.0f),
	 ViewMatrix(1.0f), UpdateView(false)
{
}

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation)
	:Forward(DEFAULT_FORWARD), Right(DEFAULT_RIGHT), Up(DEFAULT_UP),
	 UpdateForward(true), UpdateRight(true), UpdateUp(true),
	 Position(position), EulerRotation(rotation),
	 ViewMatrix(1.0f), UpdateView(true)
{
}

glm::vec3 Camera::GetForward(){
	if (UpdateForward){
		Forward = glm::rotate(glm::rotate(glm::rotate(DEFAULT_FORWARD, glm::radians(EulerRotation.x), glm::vec3{1.0f, 0.0f, 0.0f}), glm::radians(EulerRotation.y), glm::vec3{0.0f, 1.0f, 0.0f}), glm::radians(EulerRotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
	}
	UpdateForward = false;
	return Forward;
}
glm::vec3 Camera::GetRight(){
	if (UpdateRight){
		Right = glm::rotate(glm::rotate(glm::rotate(DEFAULT_RIGHT, glm::radians(EulerRotation.x), glm::vec3{1.0f, 0.0f, 0.0f}), glm::radians(EulerRotation.y), glm::vec3{0.0f, 1.0f, 0.0f}), glm::radians(EulerRotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
	}
	UpdateRight = false;
	return Right;
}
glm::vec3 Camera::GetUp(){
	if (UpdateUp){
		Up = glm::rotate(glm::rotate(glm::rotate(DEFAULT_UP, glm::radians(EulerRotation.x), glm::vec3{1.0f, 0.0f, 0.0f}), glm::radians(EulerRotation.y), glm::vec3{0.0f, 1.0f, 0.0f}), glm::radians(EulerRotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
	}
	UpdateUp = false;
	return Up;
}

void Camera::SetRotation(const glm::vec3& euler){
	
	EulerRotation = euler;
	UpdateView = true;
	UpdateForward = true;
	UpdateRight = true;
	UpdateUp = true;
}

void Camera::Rotate(const glm::vec3& euler){
	
	EulerRotation += euler;
	UpdateView = true;
	UpdateForward = true;
	UpdateRight = true;
	UpdateUp = true;
}

void Camera::SetPosition(const glm::vec3& position){
	
	Position = position;
	UpdateView = true;
}

void Camera::Translate(const glm::vec3& translation){
	
	Position += translation;
	UpdateView = true;
}

glm::mat4 Camera::GetViewMatrix(){

	//Only recalculate if one of our values has changed
	if (UpdateView){
		
		ViewMatrix = glm::lookAt(Position, Position + GetForward(), GetUp());
	}
	UpdateView = false;
	return ViewMatrix;
}