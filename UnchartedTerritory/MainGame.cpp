#include "MainGame.h"
#include "Errors.h"
#include "ResourceManager.h"

#include <iostream>
#include <string>

//Constructor used to initialize variables
MainGame::MainGame() : 
	_window(nullptr), 
	_title("Uncharted Territory"), 
	_screenWidth(1280), 
	_screenHeight(720), 
	_gameState(GameState::PLAY), 
	_time(0.0f),
	_maxFPS(60.0f)
{
}


MainGame::~MainGame()
{
}

void MainGame::run(){
	initSystems();

	//Add a sprite
	_sprites.push_back(new Sprite());

	_sprites.back()->initialize(0.0f, 0.0f, 0.5f, 0.5f, "Textures/SpriteSheet/Froglvl1.png");

	_sprites.push_back(new Sprite());

	_sprites.back()->initialize(-0.5f, -0.5f, 0.5f, 0.5f, "Textures/SpriteSheet/Froglvl1.png");

	gameLoop();
}

	
void MainGame::initSystems(){
	//Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Tells opengl we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Do this before window is created

	//Create and open a SDL window
	_window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	//Error checking on window
	if(_window == nullptr)
		fatalError("SDL Window could not be created!");

	//Creating a context 
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);

	//Error checking on context
	if(glContext == nullptr)
		fatalError("SDL_GL context could not be created!");

	//Initialize glew 
	GLenum error = glewInit(); //glewInit() throws a GLenum

	//Error checking on glew
	if(error != GLEW_OK)
		fatalError("Could not initialize glew!");

	//Print out the OpenGL version
	printf("**** OpenGL Version: %s ****\n", glGetString(GL_VERSION));

	//Set background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Turns off V-Sync
	SDL_GL_SetSwapInterval(0);

	initShaders();
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
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		
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

			//Track mouse motion - might be used later in game
			case SDL_MOUSEMOTION:
				//std::cout << "X: " << events.motion.x << " Y: " << events.motion.y << std::endl;
				break;
		}

	}
}

void MainGame::drawGame(){

	//Set the depth to 1.0
	glClearDepth(1.0);

	//Clears the colour buffer and depth buffer with bitwise or
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colourProgram.useProgram();

	//Get the first texture
	glActiveTexture(GL_TEXTURE0);

	//Get the unform location for texture
	GLuint textureLocation = ResourceManager::getLocation("myTexture", &_colourProgram);

	//Send 1 interger to gpu 
	glUniform1i(textureLocation, 0);

	//Get the uniform location for time
	GLuint timeLocation = ResourceManager::getLocation("time", &_colourProgram);

	//Send 1 float to gpu
	glUniform1f(timeLocation, _time);

	for (int i = 0; i < (int)_sprites.size(); i++) {
		//Draw sprites
		_sprites[i]->draw();
	}

	//Unbind the player texture before finishing drawing
	glBindTexture(GL_TEXTURE_2D, 0);
	_colourProgram.unuseProgram();

	//Pushes everything to window
	SDL_GL_SwapWindow(_window);
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