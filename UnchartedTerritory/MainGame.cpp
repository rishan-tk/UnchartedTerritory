#include "MainGame.h"

//Include files from our game engine
#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>

#include <iostream>
#include <string>

//Constructor used to initialize variables
MainGame::MainGame(){
}


MainGame::~MainGame(){
	for (int i = 0; i < (int)_levels.size(); i++)
		delete _levels[i];
	for (int i = 0; i < (int)_player.size(); i++)
		delete _player[i];
}

void MainGame::run(){
	initialiseSystems();
	initialiseLevel(0);
	gameLoop();
}

	
void MainGame::initialiseSystems(){
	GameEngine2D::initialise();

	_window.createWindow(_title, (int)_screenDimensions.x, (int)_screenDimensions.y, GameEngine2D::WINDOWED); ///< Use our window class to create an SDL Window with the given properties

	initialiseShaders();

	_camera.initialise(_screenDimensions);
	_hudCamera.initialise(_screenDimensions);
	_hudCamera.setPosition(glm::vec2(_screenDimensions.x / 2, _screenDimensions.y / 2)); ///< Set the position of the hud


	_spriteBatch.initialise(); ///< Initialise our spritebatch variable

	_fontSpriteBatch.initialise();

	_spriteFont = new GameEngine2D::SpriteFont("Fonts/arialbold.ttf", 32);

	_fpsLimiter.initialise(_maxFPS); ///< Initialise out fps limiter to max fps(60)
}

void MainGame::initialiseLevel(int level){
	_currentLevel = level;

	std::string levelFile = "Levels/level" + std::to_string(level + 1) + ".txt";

	_levels.emplace_back(new Level(levelFile)); ///< Add the current Level

	//Add the coins to the level
	for (int i = 0; i < _levels[_currentLevel]->getNoCoins(); i++)
		_coins.emplace_back(new Coin(_levels[_currentLevel]->getCoinStartPosition(i)));

	_door.initialise(_levels[_currentLevel]->getDoorStartPosition(), (int)_coins.size()); ///< Add the exit door to the level
	
	_player.emplace_back(new Player(_levels[_currentLevel]->getPlayerStartPosition())); ///< Add 1 Player
	_player[0]->initialise(10.0f, 10.0f, &_inputManager); ///< Set the player speed, gravity and pass in the address of inputManager
}

void MainGame::initialiseShaders(){
	_colourProgram.compileShaders("Shaders/basic.shader");
	_colourProgram.addAttribute("vertexPosition");
	_colourProgram.addAttribute("vertexColour");
	_colourProgram.addAttribute("vertexUV");
	_colourProgram.linkShaders();
}

void MainGame::updateEntities(){
	//Update bullets
	for (int i = 0; i < (int)_bullets.size(); i++) {
		for (int j = 0; j < (int)_coins.size(); j++) {
			if (_coins[j]->update(_bullets[i].getPosition())) { ///< Check if bullets are colliding with a coin
				_coins[j] = _coins.back();
				delete _coins.back();
				_coins.pop_back();
			}
		}

		if (_bullets[i].update(_levels[_currentLevel]->getTiles())) { ///< Check if bullets are colliding with the level
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		
	}

	//Check if player is colliding with coins
	for (int i = 0; i < (int)_coins.size(); i++) {
		if (_coins[i]->update(_player[0]->getPosition())) {
			_coins[i] = _coins.back();
			delete _coins.back();
			_coins.pop_back();
		}
	}

	_door.update((int)_coins.size(), _player[0]->getPosition());

	if(_door.isColliding())
		_gameState = GameState::WON;///< Update the doors lifetime
}

void MainGame::gameLoop(){
	//Used to get delta time
	float prevTime = 0;
	float currentTime = (float)SDL_GetTicks();

	while(_gameState == GameState::PLAY){
		//Used for frame time measuring 
		prevTime = currentTime;
		currentTime = _fpsLimiter.begin();
		
		float deltaTime =  currentTime - prevTime;

		//If deltaTime grows too large, lock it down
		if (deltaTime > 0.15f)
			deltaTime = 0.15f;

		_inputManager.update();

		processInput();

		_player[0]->update(_levels[_currentLevel]->getTiles(), deltaTime);
		
		setCameraPosition();
		_camera.update();
		_hudCamera.update();
	
		updateEntities();

		drawGame();

		_fpsLimiter.end();

		_fps = _fpsLimiter.getFPS();

		//Static variable to tell us when to print fps
		static int counter = 0;

		//Print out the fps
		if(counter++ % 500 == 0)
			std::cout << _fps << std::endl;	
	}

	while (_gameState == GameState::WON) {
		_player.pop_back();
		_inputManager.update();

		processInput();

		drawGame();
		if(_inputManager.isKeyPressed(SDLK_SPACE))
			_gameState = GameState::EXIT;
	}
}

void MainGame::processInput(){
	SDL_Event events;

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

	//Shoot bullets
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreentoWorldCoords(mouseCoords);

		glm::vec2 playerPosition((_player[0]->getPosition().x)+(ENTITY_DIMENSION/2),
								(_player[0]->getPosition().y)+(ENTITY_DIMENSION/2));

		glm::vec2 direction(mouseCoords - playerPosition);
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 5.0f, 1000);
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

	//Get the uniform location for texture
	GLuint textureLocation = GameEngine2D::ResourceManager::getLocation("myTexture", &_colourProgram);

	//Send 1 integer to gpu 
	glUniform1i(textureLocation, 0);

	//Get the uniform location for the projection matrix
	GLuint projMatrixLocation = GameEngine2D::ResourceManager::getLocation("projMatrix", &_colourProgram);

	//Set the camera matrix
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//Send a mat4 to the gpu
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//Begin drawing to the sprite batch
	_spriteBatch.begin();

	_levels[_currentLevel]->draw(); ///< Draws to its own spriteBatch

	//Draw all the coins
	for (int i = 0; i < (int)_coins.size(); i++)
		_coins[i]->draw(_spriteBatch);

	//Draw the exit door
	_door.draw(_spriteBatch);

	//Draw all the players
	for (int i = 0; i < (int)_player.size(); i++)
		_player[i]->draw(_spriteBatch);

	//Draw all the bullets
	for (int i = 0; i < (int)_bullets.size(); i++)
		_bullets[i].draw(_spriteBatch);
	
	//Finish drawing to the sprite batch
	_spriteBatch.end();

	//Render the batches
	_spriteBatch.renderBatch();

	if (_gameState == GameState::PLAY)
		drawHud();
	else if (_gameState == GameState::WON)
		drawEnd();

	//Unbind the player texture before finishing drawing
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader
	_colourProgram.unuseProgram();

	//Swap the buffers
	_window.swapBuffer();

}

void MainGame::drawHud(){
	char buffer[256];

	//Get the uniform location for the projection matrix
	GLuint projMatrixLocation = GameEngine2D::ResourceManager::getLocation("projMatrix", &_colourProgram);

	//Set the camera matrix
	glm::mat4 cameraMatrix = _hudCamera.getCameraMatrix();

	//Send a mat4 to the gpu
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_fontSpriteBatch.begin();

	//Fill buffer with string
	sprintf_s(buffer, "Coins remaining: %d/%d", (int)_coins.size(), _levels[_currentLevel]->getNoCoins());

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_spriteFont->draw(_fontSpriteBatch, buffer, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f), 0.0f, colour);

	_fontSpriteBatch.end();

	_fontSpriteBatch.renderBatch();
}

void MainGame::drawEnd(){

	char buffer[256];

	//Get the uniform location for the projection matrix
	GLuint projMatrixLocation = GameEngine2D::ResourceManager::getLocation("projMatrix", &_colourProgram);

	//Set the camera matrix
	glm::mat4 cameraMatrix = _hudCamera.getCameraMatrix();

	//Send a mat4 to the gpu
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_fontSpriteBatch.begin();

	//Fill buffer with string
	sprintf_s(buffer, "You have completed the level!\nPress Space to exit!");

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_spriteFont->draw(_fontSpriteBatch, buffer, glm::vec2(200.0f, 200.0f), glm::vec2(2.0f), 0.0f, colour);

	_fontSpriteBatch.end();

	_fontSpriteBatch.renderBatch();

}

void MainGame::setCameraPosition(){

	float xLow = 640.0f;
	float xHigh = 2560.0f;
	float yLow = 360.0f;
	float yHigh = 2841.0f;

	if(_player[0]->getPosition().x < xLow && _player[0]->getPosition().y < yLow) ///< Bottom left corner
		_camera.setPosition(glm::vec2(xLow, yLow));
	else if(_player[0]->getPosition().x > xHigh && _player[0]->getPosition().y < yLow) ///< Bottom right corner
		_camera.setPosition(glm::vec2(xHigh, yLow));
	else if(_player[0]->getPosition().x < xLow && _player[0]->getPosition().y > yHigh) ///< Top left corner
		_camera.setPosition(glm::vec2(xLow, yHigh));
	else if (_player[0]->getPosition().x > xHigh && _player[0]->getPosition().y > yHigh) ///< Top right corner
		_camera.setPosition(glm::vec2(xHigh, yHigh));
	else if (_player[0]->getPosition().x < xLow && _player[0]->getPosition().y > yLow && _player[0]->getPosition().y < yHigh) ///< If x is too low and y is in the middle
		_camera.setPosition(glm::vec2(xLow, _player[0]->getPosition().y));
	else if (_player[0]->getPosition().x > xHigh && _player[0]->getPosition().y > yLow && _player[0]->getPosition().y < yHigh) ///< If x is too high and y is in the middle
		_camera.setPosition(glm::vec2(xHigh, _player[0]->getPosition().y));
	else if (_player[0]->getPosition().y < yLow && _player[0]->getPosition().x > xLow && _player[0]->getPosition().x < xHigh) ///< If y is too low and x is in the middle
		_camera.setPosition(glm::vec2(_player[0]->getPosition().x, yLow));
	else if(_player[0]->getPosition().y > yHigh && _player[0]->getPosition().x > xLow && _player[0]->getPosition().x < xHigh) ///< If y is too high and x is in the middle
		_camera.setPosition(glm::vec2(_player[0]->getPosition().x, yHigh));
	else ///< In the middle
		_camera.setPosition(_player[0]->getPosition());
}
