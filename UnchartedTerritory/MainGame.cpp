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
		float startTicks = (float)SDL_GetTicks();

		processInput();
		_time += 0.01f;

		_camera.update();

		drawGame();
		getFPS();

		//Static variable to tell us when to print fps
		static int counter = 0;
		//Print out the fps
		if(counter++ % 10 == 0)
			std::cout << _fps << std::endl;

		float frameTicks = (float)SDL_GetTicks() - startTicks;

		//Limit the fps to max fps
		//Check if fps needs limiting
		if(1000.0f / _maxFPS > frameTicks)
			SDL_Delay((GLuint)(1000.0f / _maxFPS - frameTicks));
		
	}
}

void MainGame::processInput(){
	SDL_Event events;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.5f;

	//Check if there is an event
	while(SDL_PollEvent(&events)){
		//Process the event
		switch(events.type){
			//If the 'X' button is pressed
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			//Track mouse motion - might be used later in game
			case SDL_MOUSEMOTION:
				//std::cout << "X: " << events.motion.x << " Y: " << events.motion.y << std::endl;
				break;

			case SDL_KEYDOWN:
				_inputManager.pressKey(events.key.keysym.sym);
				break;

			case SDL_KEYUP:
				_inputManager.releaseKey(events.key.keysym.sym);
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
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	else if (_inputManager.isKeyPressed(SDLK_e)) 
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	
		

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

	for (int i = 0; i < 1000; i++) 
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

void MainGame::getFPS(){
	//Number of frames we are going to average over
	static const int NUM_SAMPLES = 10;

	//Frame time buffer
	static GLuint frameTimes[NUM_SAMPLES];

	static GLuint currentFrame = 0;

	//Get previous and current ticks;
	static GLuint prevTicks = SDL_GetTicks();

	GLuint currentTicks;
	currentTicks = SDL_GetTicks();

	//Calculate the frametime
	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;

	//Set prevTicks
	prevTicks = currentTicks;

	GLuint count;

	if(currentFrame++ < NUM_SAMPLES)
		count = currentFrame;
	else
		count = NUM_SAMPLES;
	

	float frameTimeAvg = 0;

	for(int i = 0; i < (int)count; i++)
		frameTimeAvg += (float)frameTimes[i];
	
	//Get the average frame time
	frameTimeAvg /= count;

	//As long as frame time average is greater than 0 calculate the FPS
	if (frameTimeAvg > 0)
		_fps = 1000.0f / frameTimeAvg;
	else
		_fps = 60.0f;

}