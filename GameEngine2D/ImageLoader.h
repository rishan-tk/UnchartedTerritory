#pragma once

#include "GLTexture.h"

#include <string>

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}