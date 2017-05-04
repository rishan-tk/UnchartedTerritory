#include "GameEngine2D.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

namespace GameEngine2D {

	int GameEngine2D::initialise() {

		//TODO: Error checking

		//Initialise SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Tells opengl we want a double buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Do this before window is created

		return 0;
	}

}