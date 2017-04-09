#include "ResourceManager.h"

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

	//Static Variables
	TextureCache ResourceManager::_textureCache;
	UniformLocationCache ResourceManager::_locationCache;

	GLTexture ResourceManager::getTexture(std::string filePath) {
		return _textureCache.getTexture(filePath);
	}

	GLuint ResourceManager::getLocation(std::string locationOf, GLSLProgram* currentProgram) {
		return _locationCache.getUniformLocation(locationOf, currentProgram);
	}

}