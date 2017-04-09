#pragma once
#include <map>
#include <string>
#include "GLTexture.h"


class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	//Return texture if found in map(textureCache), if not found then load texture and add to cache
	GLTexture getTexture(std::string filePath);

private:
	//Key is file path, value is texture
	std::map<std::string, GLTexture> _textureCache;
};

