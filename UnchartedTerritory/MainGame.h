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
#include <GameEngine2D/FPSLimiter.h>
#include <GameEngine2D/SpriteFont.h>

#include "Bullet.h"
#include "Coin.h"
#include "Level.h"
#include "Player.h"
#include "Door.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

enum class GameState{PLAY, EXIT, WON};

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

	void updateEntities();

	void gameLoop();

	void processInput();

	void drawGame();

	void drawHud();

	void drawEnd();

	void setCameraPosition();

	GameEngine2D::Window _window;

	//Window title
	char* _title = "Uncharted Territory";

	//Screen width and height
	glm::vec2 _screenDimensions = glm::vec2(1280.0f, 720.0f);

	//Keep track of game state
	GameState _gameState = GameState::PLAY;

	GameEngine2D::GLSLProgram _colourProgram;
	GameEngine2D::Camera2D _camera; ///< Main camera
	GameEngine2D::Camera2D _hudCamera;
	GameEngine2D::SpriteBatch _spriteBatch;
	GameEngine2D::SpriteBatch _fontSpriteBatch;
	GameEngine2D::InputManager _inputManager;
	GameEngine2D::FPSLimiter _fpsLimiter;
	GameEngine2D::SpriteFont* _spriteFont;

	std::vector<Bullet> _bullets;

	Door _door;

	std::vector<Coin*> _coins;

	std::vector<Level*> _levels;

	std::vector<Player*> _player;


	int _currentLevel = 0;
	
	float _maxFPS = 60.0f;
	float _fps = 0.0f;


};

