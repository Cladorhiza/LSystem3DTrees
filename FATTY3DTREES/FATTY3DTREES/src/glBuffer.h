#pragma once
#include "GL/glew.h"

class glBuffer {
private:
	const unsigned int bufferID;

public:
	glBuffer(unsigned int bufferID);
	~glBuffer();
	glBuffer(const glBuffer&) = delete;
	glBuffer(glBuffer&&) = delete;
};