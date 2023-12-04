#pragma once
#include "GL/glew.h"
#include "glm.hpp"
#include <vector>
#include "LSystem.h"
#include "GraphicsTurtle.h"
namespace GLUtil
{
	/*function build2DVAOfromData(const std::vector<float>& vertexes,
								  const std::vector<float>& colours, 
								  const std::vector<unsigned>& indexes)
	* parameters:
	*	vertexes: vector of 2D geometry vertexes
	*	colours: vector of 4 RGBA colour values per vertex
	*	indexes: vector of indexes to be rendered during a draw call.
	*		Can be smaller than vertex count but extra vertexes can never be rendered
	* purpose: To build an openGL vertex array object and store all information passed
	* returns: openGL vertex array object ID
	*/
	unsigned buildVAOfromData(const std::vector<float>& vertexes,
								const std::vector<float>& colours, 
								const std::vector<unsigned>& indexes);

	unsigned buildVAOfromData(GraphicsTurtle::renderData& data);

	unsigned buildCircleVAO(const float position[3],const float normal[3], float radius, int resolution, const float colour[4]);

	unsigned buildCylinderVAO(const float startPos[3], const float endPos[3], const float startNorm[3], const float endNorm[3], float radius1, float radius2, int resolution, const float colour[4]);
};