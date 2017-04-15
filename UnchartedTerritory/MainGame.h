#pragma once

//Include files from our game engine
#include <GameEngine2D/GameEngine2D.h>
#include <GameEngine2D/Sprite.h>
#include <GameEngine2D/GLSLProgram.h>
#include <GameEngine2D/GLTexture.h>
#include <GameEngine2D/Window.h>
#include <GameEngine2D/Camera2D.h>
#include <GameEngine2D/SpriteBatch.h>


#include <SDL/SDL.h>
#include <GL/glew.h>

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

	GameEngine2D::Window _window;

	//Window title
	char* _title;

	//Screen width and height
	int _screenWidth;
	int _screenHeight;

	//Keep track of game state
	GameState _gameState;

	GameEngine2D::GLSLProgram _colourProgram;
	GameEngine2D::Camera2D _camera;
	GameEngine2D::SpriteBatch _spriteBatch;

	float _time;
	
	float _maxFPS;
	float _fps;
	GLuint _frameTime;

};

