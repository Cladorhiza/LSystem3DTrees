#include "GraphicsTurtle.h"
#include <iostream>

GraphicsTurtle::GraphicsTurtle()
	:currentTransformation(1.f),distanceFromOrigin(0.f), stackIndex(0), poppedLastInstruction(false), branchCount(0)
	{
	}

	void GraphicsTurtle::MoveForward(float amount, bool penDown) {
		currentTransformation = glm::translate(currentTransformation, glm::vec3(0.f, amount, 0.f));
	}
	void GraphicsTurtle::RotateYaw(float rotation) {
		currentTransformation = glm::rotate(currentTransformation, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
	}
	void GraphicsTurtle::RotatePitch(float rotation) {
		currentTransformation = glm::rotate(currentTransformation, glm::radians(rotation), glm::vec3(1.f, 0.f, 0.f));
	}
	void GraphicsTurtle::RotateRoll(float rotation) {
		currentTransformation = glm::rotate(currentTransformation, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));
	}
	void GraphicsTurtle::PushMatrix() {
		matrixStack.push_back(currentTransformation);
		distanceFromOriginStack.push_back(distanceFromOrigin);
		indexOfVertexStack.push_back(stackIndex);
		branchCount++;
	}
	void GraphicsTurtle::PopMatrix() {
		if (matrixStack.size() > 0 && distanceFromOriginStack.size() > 0 && indexOfVertexStack.size() > 0) {
			currentTransformation = matrixStack.back();
			matrixStack.pop_back();
			distanceFromOrigin = distanceFromOriginStack.back();
			distanceFromOriginStack.pop_back();
			stackIndex = indexOfVertexStack.back();
			indexOfVertexStack.pop_back();
			branchCount--;
		}
		else {
			printf("WARNING: Attempted to popMatrix in graphicsturtle without sufficient stack data");
		}
	}

	void GraphicsTurtle::BuildLSystemToTurtleMappings(std::string turtleRules) {

        std::stringstream ruleStream(turtleRules);
        std::string turtleRuleString;

        while (std::getline(ruleStream, turtleRuleString, ',')) {
            std::stringstream subRule(turtleRuleString);
			
			//prefix example : A-F
			//A = Lsystem symbol, F = Turtle mapping
            std::string prefix;

            std::pair<char, float> mapping;
            subRule >> prefix;
            mapping.first = prefix[2];
            float transformationMagnitude;
            
			switch (prefix[2]) {
			case 'F' :
			case 'f' :
			case '+' :
			case '-' :
			case '&' :
			case '^' :
			case '\\':
			case '/' :
				subRule >> transformationMagnitude;
				mapping.second = transformationMagnitude;
				break;
			case '|' :
			case '[' :
			case ']' :
				mapping.second = 0.f;
				break;
			}
            LSystemToTurtleMappings[prefix[0]] = mapping;
        }
	}

	GraphicsTurtle::renderData GraphicsTurtle::GenerateGeometryOfLSystemRuleString(std::string rules) {

		//OpenGL geometry

		renderData lSystemRenderData;
		lSystemRenderData.maximumDistanceFromOrigin = 0.f;

		//push the origin on to first point before beginning traversal
		vertex origin;
		origin.position[0] = 0.f;
		origin.position[1] = 0.f;
		origin.position[2] = 0.f;
		origin.colour[0] = 1.f;
		origin.colour[1] = 0.f;
		origin.colour[2] = 0.f;
		origin.colour[3] = 1.f;
		origin.distanceFromOrigin = distanceFromOrigin;
		origin.branchCount = 0;
		origin.circleNormal[0] = 0.f;
		origin.circleNormal[1] = 1.f;
		origin.circleNormal[2] = 0.f;
		origin.isLeaf = false;
		lSystemRenderData.vertexes.push_back(origin);
		glm::vec4 newPoint(0.f, 0.f, 0.f, 1.f);
		glm::vec4 oldPoint(0.f, 0.f, 0.f, 1.f);
		unsigned int indexCount = 0;
		//parse every character from rule string into a turtle instruction
		for (char c : rules) {
			std::pair<char, float> turtleInstruction = LSystemToTurtleMappings[c];

			vertex v;
			switch (turtleInstruction.first) {
			case 'F':
				MoveForward(turtleInstruction.second, true);

				//apply the new transformation to generate new vertex
				oldPoint = newPoint;
				newPoint = currentTransformation * glm::vec4(0.f, 0.f, 0.f, 1.f);
				v.position[0] = newPoint.x;
				v.position[1] = newPoint.y;
				v.position[2] = newPoint.z;

				v.colour[0] = 1.f;
				v.colour[1] = 0.f;
				v.colour[2] = 0.f;
				v.colour[3] = 1.f;

				distanceFromOrigin += glm::length(newPoint - oldPoint);
				if (distanceFromOrigin > lSystemRenderData.maximumDistanceFromOrigin)
					lSystemRenderData.maximumDistanceFromOrigin = distanceFromOrigin;
				v.distanceFromOrigin = distanceFromOrigin;
				glm::vec4 circleNormal(glm::normalize(newPoint - oldPoint));
				v.circleNormal[0] = circleNormal.x;
				v.circleNormal[1] = circleNormal.y;
				v.circleNormal[2] = circleNormal.z;
				v.isLeaf = false;

				v.branchCount = branchCount;

				lSystemRenderData.vertexes.push_back(v);
				indexCount++;
				if (poppedLastInstruction) {
					lSystemRenderData.indexes.push_back(stackIndex);
					lSystemRenderData.indexes.push_back(indexCount);

					poppedLastInstruction = false;
				}
				else {
					lSystemRenderData.indexes.push_back(indexCount - 1);
					lSystemRenderData.indexes.push_back(indexCount);
				}
				stackIndex = indexCount;

				break;
			case 'f':
				MoveForward(turtleInstruction.second, false);
				continue;
			case '+':
				RotateYaw(-turtleInstruction.second);
				continue;
			case '-':
				RotateYaw(turtleInstruction.second);
				continue;
			case '&':
				RotatePitch(turtleInstruction.second);
				continue;
			case '^':
				RotatePitch(-turtleInstruction.second);
				continue;
			case '\\':
				RotateRoll(-turtleInstruction.second);
				continue;
			case '/':
				RotateRoll(turtleInstruction.second);
				continue;
			case '|':
				RotateYaw(180.f);
				RotatePitch(180.f);
				RotateRoll(180.f);
				continue;
			case '[':
				PushMatrix();
				continue;
			case ']':
				PopMatrix();
				newPoint = currentTransformation * glm::vec4(0.f, 0.f, 0.f, 1.f);
				if (!poppedLastInstruction) {
					lSystemRenderData.vertexes.back().isLeaf = true;
				}
				poppedLastInstruction = true;
				continue;
			}
			
		}
		//set last point to leaf as there is no pop command at end of string
		lSystemRenderData.vertexes.back().isLeaf = true;
		return lSystemRenderData;
	}

	void GraphicsTurtle::Reset() {

		currentTransformation = glm::mat4(1.f);
		matrixStack.clear();
		distanceFromOriginStack.clear();
		distanceFromOrigin = 0.f;
		indexOfVertexStack.clear();
		stackIndex = 0;
		poppedLastInstruction = false;


	}