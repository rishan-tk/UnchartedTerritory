#pragma once

//Include files from our game engine
#include <2DGameEngine/Sprite.h>
#include <2DGameEngine/GLSLProgram.h>
#include <2DGameEngine/GLTexture.h>
#include <2DGameEngine/Window.h>

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

	Window _window;

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

