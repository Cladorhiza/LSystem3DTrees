#pragma once
#include "InputManager.h"
#include "external_code/glm/glm.hpp"
#include "external_code/glm/gtc/matrix_transform.hpp"
#include "external_code/glm/gtx/rotate_vector.hpp"
namespace Camera
{





	void Update();
	glm::mat4 GetViewMatrix();
};

