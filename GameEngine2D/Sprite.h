#pragma once

#include "GLTexture.h"

#include <GL/glew.h>
#include <string>

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		//Initialize the sprite
		void initialize(float x, float y, float width, float height, std::string texturePath);

		//Draw the sprite
		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;

		//32bit int for vertex buffer object
		GLuint _vboID;

		//Store the texture in the class
		GLTexture _texture;
	};

}