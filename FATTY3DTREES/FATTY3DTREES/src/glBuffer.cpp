#include "glBuffer.h"

glBuffer::glBuffer(unsigned int bufferID) :bufferID(bufferID) {

}

glBuffer::~glBuffer() {
	glDeleteBuffers(1, &bufferID);
}