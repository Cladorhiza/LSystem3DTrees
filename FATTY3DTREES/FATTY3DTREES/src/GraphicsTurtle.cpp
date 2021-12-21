#include "GraphicsTurtle.h"

GraphicsTurtle::GraphicsTurtle()
	:currentTransformation(1.f)
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
	}
	void GraphicsTurtle::PopMatrix() {
		//might break
		currentTransformation = matrixStack.back();
		matrixStack.pop_back();
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

	unsigned GraphicsTurtle::GenerateGeometryOfLSystemRuleString(std::string rules) {

		//OpenGL geometry
		std::vector<float> vertexes;
		std::vector<float> colours;
		std::vector<unsigned> indexes;

		//push the origin on to first point before beginning traversal
		std::vector<float> colour{ 1.f, 0.f, 0.f, 1.f };
		glm::vec4 origin(0.f, 0.f, 0.f, 1.f);
		vertexes.push_back(origin.x);
		vertexes.push_back(origin.y);
		vertexes.push_back(origin.y);
		colours.insert(colours.end(), colour.begin(), colour.end());

		//tracks current index we are starting our move operation from
		int indexCount = 0;
		
		//parse every character from rule string into a turtle instruction
		for (char c : rules) {
			std::pair<char, float> turtleInstruction = LSystemToTurtleMappings[c];

			switch (turtleInstruction.first) {
			case 'F':
				MoveForward(turtleInstruction.second, true);
				break;
			case 'f':
				MoveForward(turtleInstruction.second, false);
				break;
			case '+':
				RotateYaw(-turtleInstruction.second);
				break;
			case '-':
				RotateYaw(turtleInstruction.second);
				break;
			case '&':
				RotatePitch(turtleInstruction.second);
				break;
			case '^':
				RotatePitch(-turtleInstruction.second);
				break;
			case '\\':
				RotateRoll(-turtleInstruction.second);
				break;
			case '/':
				RotateRoll(turtleInstruction.second);
				break;
			case '|':
				RotateYaw(180.f);
				RotatePitch(180.f);
				RotateRoll(180.f);
				break;
			case '[':
				PushMatrix();
				break;
			case ']':
				PopMatrix();
				break;
			}

			//apply the new transformation to generate new geometry
			glm::vec4 newPoint = currentTransformation * origin;
			vertexes.push_back(newPoint.x);
			vertexes.push_back(newPoint.y);
			vertexes.push_back(newPoint.z);
			indexCount++;
			colours.insert(colours.end(), colour.begin(), colour.end());
			//if it was a pop we don't want to connect old position to new position
			if (turtleInstruction.first == ']') continue;
			indexes.push_back(indexCount - 1);
			indexes.push_back(indexCount);
		}

		return GLUtil::buildVAOfromData(vertexes, colours, indexes);
	}

	void GraphicsTurtle::Reset() {

		currentTransformation = glm::mat4(1.f);
		std::vector<glm::mat4> m;
		matrixStack = m;



	}