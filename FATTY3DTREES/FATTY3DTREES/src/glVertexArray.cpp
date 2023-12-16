#include "glVertexArray.h"



glVertexArray::glVertexArray(unsigned int vaID) :vaID(vaID) {

}

glVertexArray::~glVertexArray() {
	glDeleteVertexArrays(1, &vaID);
}