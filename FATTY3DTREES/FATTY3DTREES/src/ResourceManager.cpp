#include "ResourceManager.h"

namespace ResourceManager {

	std::unordered_map<unsigned int, glBuffer> glBuffers;
	std::unordered_map<unsigned int, glVertexArray> glVertexArrays;

	void AddBuffer(unsigned int buffID) {
		if (glBuffers.find(buffID) == glBuffers.end()) {
			glBuffers.emplace(std::piecewise_construct,
							  std::forward_as_tuple(buffID),
							  std::forward_as_tuple(buffID));
		}
		else printf("ERROR (ResourceManager): Attempted to add buffer that already exists");
	}

	void RemoveBuffer(unsigned int buffID) {
		glBuffers.erase(buffID);
	}

	void AddVertexArray(unsigned int vaID) {
		if (glVertexArrays.find(vaID) == glVertexArrays.end()) {
			glVertexArrays.emplace(std::piecewise_construct,
							  std::forward_as_tuple(vaID),
							  std::forward_as_tuple(vaID));
		}
		else printf("ERROR (ResourceManager): Attempted to add vertex array that already exists");
	}

	void RemoveVertexArray(unsigned int vaID) {
		glVertexArrays.erase(vaID);
	}

	void Clear() {
		glBuffers.clear();
		glVertexArrays.clear();
	}

}