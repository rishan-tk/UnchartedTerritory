#pragma once

#include "GLTexture.h"

#include <map>
#include <string>

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

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

}