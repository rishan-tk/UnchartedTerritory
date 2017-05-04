#include "MainGame.h"

//Include files from our game engine
#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>

#include <iostream>
#include <string>

//Constructor used to initialize variables
MainGame::MainGame() : 
	_title("Uncharted Territory"), 
	_screenDimensions(1280.0f, 720.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f),
	_currentLevel(0)
{
	_camera.initialise(_screenDimensions);
}


MainGame::~MainGame(){

	for (int i = 0; i < (int)_levels.size(); i++)
		delete _levels[i];

}

void MainGame::run(){
	initialiseSystems();

	initialiseLevel(0);

	gameLoop();
}

	
void MainGame::initialiseSystems(){
	
	GameEngine2D::initialise();

	//Use our window class to create an SDL Windown with the given properties
	_window.createWindow(_title, (int)_screenDimensions.x, (int)_screenDimensions.y, GameEngine2D::WINDOWED);

	initialiseShaders();

	//Initialize spritebatch
	_spriteBatch.initialise();

	//Initialise our FPS Limiter
	_fpsLimiter.initialise(_maxFPS);
}

void MainGame::initialiseLevel(int level){
	_currentLevel = level;
	_levels.push_back(new Level("Levels/level1.txt")); ///< Add the first Level
	
	_player.emplace_back(new Player(_levels[_currentLevel]->getPlayerStartPosition())); ///< Add 1 Player
	_player[0]->initialise(5.0f, &_inputManager); ///< Set the player speed and pass in the adress of inputManager
}

void MainGame::initialiseShaders(){
	_colourProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
	_colourProgram.addAttribute("vertexPosition");
	_colourProgram.addAttribute("vertexColour");
	_colourProgram.addAttribute("vertexUV");
	_colourProgram.linkShaders();
}

void MainGame::updateBullets(){
	for (int i = 0; i < (int)_bullets.size();) {
		if (_bullets[i].update() == true) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
			i++;
	}
}

void MainGame::gameLoop(){
	while(_gameState == GameState::PLAY){
		//Used for frame time measuring 
		_fpsLimiter.begin();

		processInput();
		
		_player[0]->update(_levels[_currentLevel]->getTiles()); ///< Update player 1

		_camera.setPosition(_player[0]->getPosition());
		_camera.update();

		
		updateBullets(); ///< Update the bullets



		drawGame();

		_fps = _fpsLimiter.end();

		//Static variable to tell us when to print fps
		static int counter = 0;

		//Print out the fps
		if(counter++ % 500 == 0)
			std::cout << _fps << std::endl;	
	}
}

void MainGame::processInput(){
	SDL_Event events;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	//Check if there is an event
	while(SDL_PollEvent(&events)){
		//Process the event
		switch(events.type){
			//If the 'X' button is pressed
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				_inputManager.pressKey(events.key.keysym.sym);
				break;

			case SDL_KEYUP:
				_inputManager.releaseKey(events.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(events.button.button);
				break;

			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(events.button.button);
				break;

			//Track mouse motion 
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords((float)events.motion.x, (float)events.motion.y);
				break;
		}
	}

	//Scaling
	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(_camera.getScale() * (1 + SCALE_SPEED));
	else if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(_camera.getScale() * (1 - SCALE_SPEED));

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreentoWorldCoords(mouseCoords);

		glm::vec2 playerPosition((_player[0]->getPosition().x)+(ENTITY_DIMENSION/2),
								(_player[0]->getPosition().y)+(ENTITY_DIMENSION/2));

		glm::vec2 direction(mouseCoords - playerPosition);
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 2.0f, 1000);
	}

	
}

void MainGame::drawGame(){

	//Set the depth to 1.0
	glClearDepth(1.0);

	//Clears the colour buffer and depth buffer with bitwise or
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader
	_colourProgram.useProgram();

	//Get the first texture
	glActiveTexture(GL_TEXTURE0);

	//Get the unform location for texture
	GLuint textureLocation = GameEngine2D::ResourceManager::getLocation("myTexture", &_colourProgram);

	//Send 1 interger to gpu 
	glUniform1i(textureLocation, 0);

	//Get the uniform location for the projection matrix
	GLuint projMatrixLocation = GameEngine2D::ResourceManager::getLocation("projMatrix", &_colourProgram);

	//Set the camera matrix
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//Send a mat4 to the gpu
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	
	_spriteBatch.begin();


	//glm::vec4 uv(0.0f, 2.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f);


	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_levels[_currentLevel]->draw(); ///< Draws to its own spriteBatch

	//Draw all the players
	for (int i = 0; i < (int)_player.size(); i++)
		_player[i]->draw(_spriteBatch);
	
	//Draw all the bullets
	for (int i = 0; i < (int)_bullets.size(); i++) 
		_bullets[i].draw(_spriteBatch);
	
	_spriteBatch.end();

	_spriteBatch.renderBatch();

	//Unbind the player texture before finishing drawing
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader
	_colourProgram.unuseProgram();

	//Swap the buffers
	_window.swapBuffer();

}
