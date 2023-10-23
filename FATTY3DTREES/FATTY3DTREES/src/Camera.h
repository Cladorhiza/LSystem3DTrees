#pragma once

#include "external_code/glm/glm.hpp"
#include "external_code/glm/gtc/matrix_transform.hpp"
namespace Camera{

	
	void Update();
	glm::mat4 GetViewMatrix();
	float GetZoomSensitivity();
};

