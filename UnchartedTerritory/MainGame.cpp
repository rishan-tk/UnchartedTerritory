#include "MainGame.h"

//Include files from our game engine
#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>

#include <iostream>
#include <string>

//Constructor used to initialize variables
MainGame::MainGame() : 
	_title("Uncharted Territory"), 
	_screenWidth(1280), 
	_screenHeight(720), 
	_gameState(GameState::PLAY), 
	_time(0.0f),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}

void MainGame::run(){
	initSystems();

	gameLoop();
}

	
void MainGame::initSystems(){
	
	GameEngine2D::init();

	//Use our window class to create an SDL Windown with the given properties
	_window.createWindow(_title, _screenWidth, _screenHeight, GameEngine2D::BORDERLESS);

	initShaders();

	//Initialize spritebatch
	_spriteBatch.init();

	//Initialise our FPS Limiter
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders(){
	_colourProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
	_colourProgram.addAttribute("vertexPosition");
	_colourProgram.addAttribute("vertexColour");
	_colourProgram.addAttribute("vertexUV");
	_colourProgram.linkShaders();
}

void MainGame::gameLoop(){
	while(_gameState != GameState::EXIT){
		//Used for frame time measuring 
		_fpsLimiter.begin();

		processInput();
		_time += 0.01f;

		_camera.update();

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

	if (_inputManager.isKeyPressed(SDLK_w)) 
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	 if (_inputManager.isKeyPressed(SDLK_s)) 
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	
	if (_inputManager.isKeyPressed(SDLK_a)) 
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	 if (_inputManager.isKeyPressed(SDLK_d)) 
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	
	if (_inputManager.isKeyPressed(SDLK_q)) 
		_camera.setScale(_camera.getScale() * (1 + SCALE_SPEED));
	else if (_inputManager.isKeyPressed(SDLK_e)) 
		_camera.setScale(_camera.getScale() * (1 - SCALE_SPEED));

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreentoWorldCoords(mouseCoords);
		std::cout << mouseCoords.x << " / " << mouseCoords.y << std::endl;

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

	//Get the uniform location for time
	//GLuint timeLocation = GameEngine2D::ResourceManager::getLocation("time", &_colourProgram);

	//Send 1 float to gpu
	//glUniform1f(timeLocation, _time);

	//Get the uniform location for the projection matrix
	GLuint projMatrixLocation = GameEngine2D::ResourceManager::getLocation("projMatrix", &_colourProgram);

	//Set the camera matrix
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//Send a mat4 to the gpu
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	
	_spriteBatch.begin();

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);

	glm::vec4 uv(0.0f, 2.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f);

	GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/Froglvl1.png");

	GameEngine2D::Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;

	
	_spriteBatch.draw(position, uv, texture.id, 0.0f, colour);
	

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	//Unbind the player texture before finishing drawing
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader
	_colourProgram.unuseProgram();

	//Swap the buffers
	_window.swapBuffer();

}
