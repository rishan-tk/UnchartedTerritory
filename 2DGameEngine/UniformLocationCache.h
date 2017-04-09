#pragma once

#include "GLSLProgram.h"

#include <GL/glew.h>
#include <string>
#include <map>

class UniformLocationCache
{
public:
	UniformLocationCache();
	~UniformLocationCache();

	//Return the uniform location if found in map, if not found then load location and add to cache
	GLuint getUniformLocation(std::string locationOf, GLSLProgram* currentProgram);

private:
	//Key is variable name, value is locationID
	std::map<std::string, GLuint> _uniformLocationCache;

};

