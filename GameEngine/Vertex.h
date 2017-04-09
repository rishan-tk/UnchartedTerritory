#pragma once

#include <GL/glew.h>

struct Position{
	float x;
	float y;	
};

//Represents rgba colour with 4 bytes
struct Colour{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;	
};

struct UV{
	float u;
	float v;
};

struct Vertex{

	 Position position;

	 Colour colour;

	 //UV texture coordinates
	 UV uv;

	 void setPosition(float xPos, float yPos){
		position.x = xPos;
		position.y = yPos;
	}

	 void setColour(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha){
		colour.r = red;
		colour.g = green;
		colour.b = blue;
		colour.a = alpha;
	}

	void setUV(float u, float v){
		uv.u = u;
		uv.v = v;
	}
	
};