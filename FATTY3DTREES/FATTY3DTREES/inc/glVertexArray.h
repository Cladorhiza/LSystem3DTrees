#pragma once
#include "GL/glew.h"

class glVertexArray {
private:
	const unsigned int vaID;

public:
	glVertexArray(unsigned int vaID);
	~glVertexArray();
	glVertexArray(const glVertexArray&) = delete;
	glVertexArray(glVertexArray&&) = delete;
};