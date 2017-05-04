#pragma once

//Include files from our game engine
#include <GameEngine2D/GameEngine2D.h>
#include <GameEngine2D/Sprite.h>
#include <GameEngine2D/GLSLProgram.h>
#include <GameEngine2D/GLTexture.h>
#include <GameEngine2D/Window.h>
#include <GameEngine2D/Camera2D.h>
#include <GameEngine2D/SpriteBatch.h>
#include <GameEngine2D/InputManager.h>
#include <GameEngine2D/Timing.h>

#include "Bullet.h"
#include "Level.h"
#include "Player.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

enum class GameState{PLAY, EXIT, MAIN_MENU, PAUSE};

class MainGame
{
public:
	MainGame();
	~MainGame();

	//Method used to run the game
	void run();

	

private:
	//Initialise the game 
	void initialiseSystems();

	void initialiseLevel(int level);

	void initialiseShaders();

	void updateBullets();

	void gameLoop();

	void processInput();

	void drawGame();

	GameEngine2D::Window _window;

	//Window title
	char* _title;

	//Screen width and height
	glm::vec2 _screenDimensions;

	//Keep track of game state
	GameState _gameState;

	GameEngine2D::GLSLProgram _colourProgram;
	GameEngine2D::Camera2D _camera;
	GameEngine2D::SpriteBatch _spriteBatch;
	GameEngine2D::InputManager _inputManager;
	GameEngine2D::FPSLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	std::vector<Level*> _levels;

	std::vector<Player*> _player;


	int _currentLevel;
	
	float _maxFPS;
	float _fps;


};

