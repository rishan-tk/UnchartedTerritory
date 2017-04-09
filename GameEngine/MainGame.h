#pragma once

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"


#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

enum class GameState{PLAY, EXIT};


class MainGame
{
public:
	MainGame();
	~MainGame();

	//Method used to run the game
	void run();

	

private:
	//Initialise the game 
	void initSystems();

	void initShaders();

	void gameLoop();

	void processInput();

	void drawGame();

	void getFPS();

	//Hold pointer to window
	SDL_Window* _window;

	//Window title
	char* _title;

	//Screen width and height
	int _screenWidth;
	int _screenHeight;

	//Keep track of game state
	GameState _gameState;

	std::vector<Sprite*> _sprites;

	GLSLProgram _colourProgram;

	float _time;
	
	float _maxFPS;
	float _fps;
	GLuint _frameTime;

};

