#include "ResourceManager.h"

namespace ResourceManager {

	std::unordered_map<unsigned int, glBuffer> glBuffers;

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

	void Clear() {
		glBuffers.clear();
	}

}