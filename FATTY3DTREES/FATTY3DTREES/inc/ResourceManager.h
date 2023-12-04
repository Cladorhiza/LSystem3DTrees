#pragma once
#include "GL/glew.h"
#include <unordered_map>
#include "glBuffer.h"

namespace ResourceManager
{
	
	void AddBuffer(unsigned int buffID);
	void RemoveBuffer(unsigned int buffID);
	void Clear();
};

