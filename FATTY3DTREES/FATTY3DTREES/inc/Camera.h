#pragma once
#include "glm.hpp"

class Camera {

public:

	Camera();
	Camera(const glm::vec3& position, const glm::vec3& rotation);
	
	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	void SetRotation(const glm::vec3& euler);
	void Rotate(const glm::vec3& euler);
	void SetPosition(const glm::vec3& position);
	void Translate(const glm::vec3& translation);
	glm::mat4 GetViewMatrix();

private:

	const glm::vec3 DEFAULT_FORWARD {0.0f, 0.0f, -1.0f};
	const glm::vec3 DEFAULT_RIGHT {1.0f, 0.0f, 0.0f};
	const glm::vec3 DEFAULT_UP {0.0f, 1.0f, 0.0f};

	//axis vectors
	glm::vec3 Forward, Right, Up;
	bool UpdateForward, UpdateRight, UpdateUp;
	glm::vec3 Position;
	glm::vec3 EulerRotation;
	glm::mat4 ViewMatrix;
	bool UpdateView;
};